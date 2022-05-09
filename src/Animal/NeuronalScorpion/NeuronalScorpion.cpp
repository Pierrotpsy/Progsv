/*!
 * NeuronalScorpion
 */


#include <Animal/NeuronalScorpion/Sensor.hpp>

#include "NeuronalScorpion.hpp"


NeuronalScorpion::NeuronalScorpion(Vec2d position, const Animal* mother, double energy)
  : NeuronalScorpion(position, energy, getAppEnv().getRandomGenerator().uniform(0, 1) == 0, mother) {}

NeuronalScorpion::NeuronalScorpion(Vec2d position, double energy, bool isFemale, const Animal* mother)
  : Scorpion(position, energy, isFemale, mother) {
  vector<double> angles({18, 54, 90, 140, -140, -90, -54, -18});

  for (double angle : angles) {
    sensors.push_back(Sensor(this, angle * DEG_TO_RAD));
  }

  for (size_t index = 0; index < sensors.size(); index++) {
    Sensor& target = sensors[index];

    for (size_t i = 3; i <= 5; i++) {
      target.addLinkedSensor(&sensors[(index + i) % angles.size()]);
    }
  }
}


Vec2d NeuronalScorpion::getPositionOfSensor(double angle) const {
  return convertToGlobalCoord(Vec2d(
        cos(angle),
        sin(angle))
      * getSensorRadius());
}

Vec2d NeuronalScorpion::getForce() const {
  switch (state) {
    case NeuronalState::MOVING:
      return abs(getDirectionToTarget().angle() - getRotation()) > getRotationAnglePrecision()
        ? getStoppingForce()
        : Vec2d(1.0, 0);

    case NeuronalState::TARGET_IN_SIGHT:
    case NeuronalState::WANDERING:
      return getTargetingForce();

    default:
      return Vec2d(0, 0);
  }
}

double NeuronalScorpion::getMaxSpeed() const {
  double standardSpeed = getStandardMaxSpeed();

  switch (state) {
    case NeuronalState::TARGET_IN_SIGHT:
      return standardSpeed * 2.0;

    default:
      return standardSpeed;
  }
}

Vec2d NeuronalScorpion::getSensorsTargetDirection() const {
  Vec2d direction(0.0, 0.0);

  for (const auto& sensor : sensors) {
    direction += sensor.getTargetDirection();
  }

  return direction;
}

bool NeuronalScorpion::isSensorActive() const {
  for (const Sensor& sensor : sensors) {
    if (sensor.isActive()) return true;
  }

  return false;
}


void NeuronalScorpion::update(sf::Time dt) {
  // OrganicEntity gère uniquement l'âge, donc on l'appelle le plus tôt possible.
  OrganicEntity::update(dt);

  // On stocke l'état précédent pour voir s'il aura changé.
  bool wasSensorActive = isSensorActive();

  // 1. mise à jour des senseurs
  for (auto& sensor : sensors) {
    sensor.update(dt);
  }

  // 2. gestion de l'horloge de réceptivité aux senseurs
  if (!wasSensorActive && isSensorActive()) {
    sensorActivityEndAge = age + getSensorActivationDuration();
  }

  // 3. calcul de la direction vers la cible
  if (isSensorActive() && age > sensorActivityEndAge) {
    setTargetPosition(getPosition() + getSensorsTargetDirection());

    for (Sensor& sensor : sensors) {
      sensor.reset();
    }
  }

  // 4. mise à jour de l'état
  updateState();

  // 5. mise à jour du déplacement
  if (state == NeuronalState::WANDERING) {
    randomWalk();
  }

  updatePosition(dt);

  if (state == NeuronalState::MOVING && abs(getDirectionToTarget().angle() - getRotation()) > getRotationAnglePrecision()) {
    setRotation(getDirectionToTarget().angle());
  }
}

void NeuronalScorpion::updateState() {
  vector<OrganicEntity*> visibleEntities = getAppEnv()
    .getEntitiesInSightForAnimal(this);

  OrganicEntity* nearestTarget(nullptr);
  double nearestTargetDistance = std::numeric_limits<double>::infinity();

  for (OrganicEntity* entity : visibleEntities) {
    double distance = distanceTo(*entity);

    if (eatable(entity) && distance < nearestTargetDistance) {
      nearestTarget = entity;
      nearestTargetDistance = distance;
    }
  }

  if (nearestTarget != nullptr) {
    if (isColliding(*nearestTarget)) {
      state = NeuronalState::WANDERING;
      energy += 0.7 * nearestTarget->energy;
      nearestTarget->energy = 0.0;
    } else {
      state = NeuronalState::TARGET_IN_SIGHT;
      setTargetPosition(nearestTarget->getPosition());
    }

    return;
  }

  switch (state) {
    case NeuronalState::MOVING: {
      if (age > interactionEndAge) {
        state = NeuronalState::IDLE;
        interactionEndAge = age + sf::seconds(5);
      }

      break;
    }

    case NeuronalState::WANDERING: {
      if (isSensorActive()) {
        state = NeuronalState::IDLE;
        interactionEndAge = age + sf::seconds(5);
      }

      break;
    }

    case NeuronalState::IDLE: {
      if (getSensorsTargetDirection().length() >= getMinimalScoreForAction()) {
        state = NeuronalState::MOVING;
        interactionEndAge = age + sf::seconds(3);
      }

      if (age > interactionEndAge) {
        state = NeuronalState::WANDERING;
      }

      break;
    }

    case NeuronalState::TARGET_IN_SIGHT: {
      state = NeuronalState::IDLE;

      break;
    }
  }
}

void NeuronalScorpion::draw(sf::RenderTarget& targetWindow) const {
  Scorpion::draw(targetWindow);
}

void NeuronalScorpion::drawDebugInfo(sf::RenderTarget& targetWindow) const {
  // Dessin du collider.
  targetWindow.draw(buildCircle(
        getPosition(),
        getSensorRadius(),
        sf::Color::Black));
  
  // Dessin de chaque senseur.
  for (const Sensor& sensor : sensors) {
    sensor.draw(targetWindow);
  }


  std::string stateText;

  switch (state) {
    case NeuronalState::IDLE:
      stateText = "IDLE";
      break;

    case NeuronalState::MOVING:
      stateText = "MOVING";
      break;

    case NeuronalState::TARGET_IN_SIGHT:
      stateText = "TARGET IN SIGHT";
      break;

    case NeuronalState::WANDERING:
      stateText = "WANDERING";
      break;

    default:
      stateText = "UNKNOWN";
  }

  // Dessin de text indiquant l'état du scorpion.
  targetWindow.draw(buildText(
      stateText,
      convertToGlobalCoord(Vec2d(getRandomWalkDistance(), 0)),
      getAppFont(),
      getDefaultDebugTextSize(),
      getDebugTextColor(),
      getRotation() / DEG_TO_RAD + 90));
}

double NeuronalScorpion::getMinimalScoreForAction() const {
  return getAppConfig().scorpion_minimal_score_for_action;
}

double NeuronalScorpion::getRotationAnglePrecision() const {
  return getAppConfig().scorpion_rotation_angle_precision;
}

sf::Time NeuronalScorpion::getSensorActivationDuration() const {
  return sf::seconds(getAppConfig().sensor_activation_duration);
}

double NeuronalScorpion::getSensorRadius() const {
  return getAppConfig().scorpion_sensor_radius;
}

