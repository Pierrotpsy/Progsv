/*!
 * Animal
 */


#include <cmath>
#include <tuple>

#include <Application.hpp>

#include "Animal.hpp"


Animal::Animal(Vec2d position, double size, double energy, bool isFemale, const Animal* mother) 
  : OrganicEntity(position, size, energy),
  isFemale(isFemale),
  direction(Vec2d(1, 0)),
  targetPosition(position),
  mother(mother),
  deceleration((double) Speed::high * .3),
  speed(0.0) {}

Animal::~Animal() {}

std::tuple<OrganicEntity*, OrganicEntity*> Animal::analyzeEnvironment() {
  
  vector<OrganicEntity*> visibleEntities = getAppEnv()
    .getEntitiesInSightForAnimal(this);
    
  OrganicEntity* nearestFood(nullptr);
  OrganicEntity* nearestMate(nullptr);
  
  // On règle ces valeurs à l'infini pour s'assurer que la première entité
  // trouvée soit forcément (car infiniment) plus proche.
  double nearestFoodDistance = std::numeric_limits<double>::infinity();
  double nearestMateDistance = std::numeric_limits<double>::infinity();
  
  for (auto entity : visibleEntities) {
    double distance = distanceTo(*entity);

    if (entity->eatable(this)) {
      enemies.push_back(entity);
    }

    if (eatable(entity) && distance < nearestFoodDistance) {
      nearestFood = entity;
      nearestFoodDistance = distance;
    }

    if (matable(entity)
        && entity->matable(this)
        && distance < nearestMateDistance) {
      nearestMate = entity;
      nearestMateDistance = distance;
    }
  }
  
  return std::make_tuple(nearestFood, nearestMate);
}

void Animal::communicateEnemies(const list<OrganicEntity*>& communicatedEnemies) {
  state = State::RUNNING_AWAY;

  for (OrganicEntity* enemy : communicatedEnemies) {
    enemies.push_back(enemy);
  }
}

void Animal::clean() {
  OrganicEntity::clean();

  for (OrganicEntity* enemy : enemies) {
    // Les ennemis marqués nullptr vont être supprimés avant d'être utilisés,
    // au prochain cycle.
    if (enemy->isDead()) enemy = nullptr;
  }
  
  if (mother != nullptr && mother->isDead()) mother = nullptr;
}

Vec2d Animal::convertToGlobalCoord(const Vec2d& local, bool noTranslate) const {
  sf::Transform transform;

  if (!noTranslate) {
    transform.translate(getPosition());
  }

  transform.rotate(getRotation() / DEG_TO_RAD);

  return transform.transformPoint(local);
}

void Animal::draw(sf::RenderTarget& targetWindow) const {
  if (isDebugOn()) {
    drawDebugInfo(targetWindow);
  }

  // Dessin du sprite de l'animal.
  targetWindow.draw(buildSprite(
        getPosition(),
        getRadius() * 2 * getGrowthFactor(),
        getAppTexture(getTexture()),
        getRotation() / DEG_TO_RAD));
}

void Animal::drawDebugInfo(sf::RenderTarget& targetWindow) const {
  double angle = getRotation() / DEG_TO_RAD;
  double range = getViewRange() / DEG_TO_RAD;

  // Dessin d'un cercle marquant si l'animal est enceinte.
  // Il est important que ceci soit avant le dessin du collider pour être en
  // dessous.
  if (isPregnant()) {
    targetWindow.draw(buildCircle(
        getPosition(),
        getRadius() + 5.0,
        sf::Color::Magenta));
  }

  // Dessin de l'angle de vue de l'animal.
  targetWindow.draw(buildArc(
        angle - range / 2.0,
        angle + range / 2.0,
        getViewDistance(),
        getPosition(),
        sf::Color(0, 0, 0, 0x0f)));

  // Dessin du collider.
  targetWindow.draw(buildCircle(
        getPosition(),
        getRadius(),
        sf::Color::White));


  std::string stateText;

  switch (state) {
    case State::GIVING_BIRTH: {
      stateText = "GIVING BIRTH";
      break;
    }

    case State::RUNNING_AWAY: {
      stateText = "RUNNING AWAY";
      break;
    }

    case State::WANDERING: {
      stateText = "WANDERING";

      // Dessin d'un cercle matérialisant le randomWalk().
      targetWindow.draw(buildAnnulus(
          convertToGlobalCoord(Vec2d(getRandomWalkDistance(), 0.0)),
          getRandomWalkRadius(),
          sf::Color(255, 150, 0), 2.0));

      // Dessin de la cible du randomWalk().
      targetWindow.draw(buildCircle(
          getPosition() + getDirectionToTarget(),
          5.0, sf::Color::Blue));

      break;
    }

    case State::FEEDING: {
      stateText = "FEEDING";
      break;
    }

    case State::FOOD_IN_SIGHT: {
      stateText = "FOOD IN SIGHT";

      // Dessin d'une ligne vers la cible.
      targetWindow.draw(buildLine(
        getPosition(),
        getPosition() + getDirectionToTarget(),
        sf::Color::Red, 1.0));

      break;
    }

    case State::MATE_IN_SIGHT:
      stateText = "MATE IN SIGHT";
      break;

    case State::MATING:
      stateText = "MATING";
      break;

    default:
      stateText = "UNKNOWN";
  }

  std::string text = stateText
    + "\n" + to_nice_string(energy - getMinEnergy())
    + "\n" + (isFemale ? "Female" : "Male");

  // Dessin de texte indiquant diverses informations.
  targetWindow.draw(buildText(
      text,
      convertToGlobalCoord(Vec2d(getRandomWalkDistance(), 0)),
      getAppFont(),
      getDefaultDebugTextSize(),
      getDebugTextColor(),
      getRotation() / DEG_TO_RAD + 90));
  
  // Dessin d'une ligne entre l'animal et sa potentielle mère.
  if (isFollowingMother() && targetPosition == mother->getPosition()) {
    targetWindow.draw(buildLine(
          getPosition(),
          getPosition() + getDirectionToTarget(),
          sf::Color::Blue, 1.0));
  }
}

const Animal* Animal::getAnimal() const {
  return this;
}

double Animal::getAnimalBaseEnergyConsumption() const {
	return getAppConfig().animal_base_energy_consumption;
}

sf::Color Animal::getDebugTextColor() const {
  return getAppConfig().debug_text_color;
}

double Animal::getDefaultDebugTextSize() const {
  return getAppConfig().default_debug_text_size;
}

Vec2d Animal::getDirectionToTarget() const {
  return directionTo(targetPosition);
}

double Animal::getGrowthFactor() const {
	return 1.0 / (1.0 + 0.8 * exp(-0.1 * age.asSeconds()));
}

Vec2d Animal::getForce() const {
  switch (state) {
    case State::RUNNING_AWAY: {
      Vec2d force(0, 0);

      for (auto enemy : enemies) {
        Vec2d dir = enemy->directionTo(*this);
        force += 500 * dir / pow(dir.length(), 1.2);
      }

      return force;
    }

    case State::GIVING_BIRTH:
    case State::MATING:
    case State::FEEDING:
      return getStoppingForce();

    case State::FOOD_IN_SIGHT:
    case State::MATE_IN_SIGHT:
    case State::WANDERING:
      return getTargetingForce();

    default:
      return Vec2d(0, 0);
  }
}

double Animal::getMaxSpeed() const {
  double speed = getGrowthFactor() * getStandardMaxSpeed();

  switch (state) {
    case State::GIVING_BIRTH:
    case State::MATING:
      return speed;

    case State::FOOD_IN_SIGHT:
      return speed * 3;

    case State::MATE_IN_SIGHT:
      return speed * 2;

    case State::RUNNING_AWAY:
      return speed * 4;

    default:
      if (energy < getInitialEnergy() / 4.0) {
        return speed / 2.0;
      }

      return speed;
  }
}

double Animal::getMinEnergy() const {
  return getAppConfig().animal_min_energy;
}

double Animal::getRadius() const {
	return CircularCollider::getRadius() * getGrowthFactor();
}

double Animal::getRotation() const {
  return direction.angle();
}

Vec2d Animal::getSpeedVector() const {
  return speed * direction;
}

Vec2d Animal::getStoppingForce() const {
  return speed < 5
    ? Vec2d(0.0, 0.0)
    : convertToGlobalCoord(Vec2d(-1.0, 0.0), true) * 100;
}

Vec2d Animal::getTargetingForce() const {
  return getDirectionToTarget().normalised() * std::min(
      getDirectionToTarget().length() / deceleration,
      getMaxSpeed());
}

double Animal::getViewDistance() const {
  return getBaseViewDistance() * getGrowthFactor() * (1.0 - 0.7 * getAppEnv().getFogIntensity(getPosition()));
}

double Animal::getWorldSize() const {
  return getAppConfig().simulation_world_size;
}

bool Animal::isFollowingMother() const {
  return mother != nullptr && distanceTo(*mother) > getWorldSize() / 16.0;
}

bool Animal::isPregnant() const {
  return nbChildren > 0;
}

bool Animal::isTargetInSight(Vec2d target) const {
  Vec2d d = directionTo(target);

  return isEqual(d.lengthSquared(), 0.0)
    || (d.length() <= getViewDistance()
        && d.normalised().dot(direction) >= cos((getViewRange() + 0.001) / 2.0));
}

bool Animal::isTargetInRange(Vec2d target) const {
  return directionTo(target).length() <= getViewDistance() * 1.5;
}

bool Animal::isYoung() const {
  return getGrowthFactor() < 0.9;
}

void Animal::meet(OrganicEntity* mate) {
  mate->metBy(this);
}

void Animal::metBy(Animal*) {
  state = State::MATING;
  interactionEndAge = age + sf::seconds(3.0);

  if (isFemale) {
    nbChildren = getAppEnv().getRandomGenerator().uniform(getMinChildren(), getMaxChildren());
    energy -= getEnergyLossFemalePerChild() * nbChildren;

    gestationEndAge = interactionEndAge + getGestationTime();
  } else {
    energy -= getEnergyLossMaleMating();
  }
}

void Animal::metBy(Food*) {}

void Animal::randomWalk() {
  Vec2d randomVec(getAppEnv().getRandomGenerator().uniform(Vec2d(-1.0, 1.0)));
  relativeWanderingTarget += randomVec * getRandomWalkJitter();
  relativeWanderingTarget = relativeWanderingTarget.normalised() * getRandomWalkRadius();

  setTargetPosition(convertToGlobalCoord(
        relativeWanderingTarget + Vec2d(getRandomWalkDistance(), 0)));
}

void Animal::setRotation(const double& angle) {
  direction.x = cos(angle);
  direction.y = sin(angle);
}

void Animal::setTargetPosition(const Vec2d& target) {
  targetPosition = target;
}

void Animal::update(sf::Time dt) {
  OrganicEntity::update(dt);

  updateState();
  
  if (state == State::WANDERING) {
    if (isYoung() && isFollowingMother()) {
      setTargetPosition(mother->getPosition());
    } else {
      randomWalk();
    }
  }

  updatePosition(dt);
}

void Animal::updatePosition(sf::Time dt) {
  Vec2d acceleration = getForce() / getMass();
  Vec2d velocity = getSpeedVector() + acceleration * dt.asSeconds();

  direction = velocity.normalised();
  speed = std::min(
      velocity.length(),
      getMaxSpeed());

  move(getSpeedVector() * dt.asSeconds());

  energy -= getAnimalBaseEnergyConsumption()
    + speed * getEnergyLossFactor() * dt.asSeconds();
}

void Animal::updateState() {
  if (state == State::RUNNING_AWAY) {
    // On retire les ennemis morts ou trop loins.
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [this](const OrganicEntity* enemy) {
          return enemy == nullptr || distanceTo(*enemy) > getWorldSize() / 4.0;
    }), enemies.end());

    if (enemies.empty()) {
      state = State::WANDERING;
    }
  }

  if (state == State::GIVING_BIRTH && age > interactionEndAge) {
    nbChildren--;
    giveBirth();

    // L'état sera réglé à WANDERING immédiatement dessous. S'il y a un autre
    // enfant à faire naître, on repassera à l'état GIVING_BIRTH à la condition
    // d'après.
  }

  if ((state == State::FEEDING
        || state == State::GIVING_BIRTH
        || state == State::MATING)
      && age > interactionEndAge) {
    state = State::WANDERING;
  }

  if (state == State::WANDERING
      && isPregnant()
      && age > gestationEndAge) {
    state = State::GIVING_BIRTH;
    interactionEndAge = age + sf::seconds(2);
  }

  // Si l'on est dans état actif, il est inutile de chercher un partenaire ou
  // de la nourriture.

  if (state == State::FEEDING
      || state == State::GIVING_BIRTH
      || state == State::MATING) {
    return;
  }
  

  std::tuple<OrganicEntity*, OrganicEntity*> foodAndMate = analyzeEnvironment();
  
  OrganicEntity* nearestFood = std::get<0>(foodAndMate);
  OrganicEntity* nearestMate = std::get<1>(foodAndMate);
  

  if (!enemies.empty()) {
    state = State::RUNNING_AWAY;
  
    vector<OrganicEntity*> entitiesInRange = getAppEnv()
      .getEntitiesInRangeForAnimal(this);
    
    for (OrganicEntity* entity : entitiesInRange) {
      if (communicative(entity)) {
        entity->communicateEnemies(enemies);
      }
    }
    
  } else if (nearestMate != nullptr) {
    if (isColliding(*nearestMate)) {
      meet(nearestMate);
      nearestMate->meet(this);
    } else {
      state = State::MATE_IN_SIGHT;
      setTargetPosition(nearestMate->getPosition());
    }
  } else if (nearestFood != nullptr) {
    if (isColliding(*nearestFood)) {
      state = State::FEEDING;
      interactionEndAge = age + sf::seconds(1.5);

      energy += 0.7 * nearestFood->energy;
      nearestFood->energy = 0.0;
    } else {
      state = State::FOOD_IN_SIGHT;
      setTargetPosition(nearestFood->getPosition());
    }
  } else {
    // Cet assignment est impératif pour s'assurer qu'un animal qui était à
    // l'état MATE_IN_SIGHT ou FOOD_IN_SIGHT repasse à l'état WANDERING s'il
    // perd sa cible de vue.
    state = State::WANDERING;
  }
}
