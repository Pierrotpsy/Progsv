/*!
 * Sensor
 */


#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>
#include <Application.hpp>

#include "Sensor.hpp"


Sensor::Sensor(NeuronalScorpion* scorpion, double angle,
  double score, double inhibitor, bool active)
  : scorpion(scorpion),
  angle(angle),
  active(active),
  score(score),
  inhibitor(inhibitor) {}
   

Sensor::~Sensor() {}

void Sensor::addLinkedSensor(Sensor* sensor) {
  linkedSensors.push_back(sensor);
}

bool Sensor::isActive() const {
  return active;
}

bool Sensor::isWaveColliding(Wave* wave) const {
  return wave->isPointContainedInPath(getLocation())
    && abs(wave->distanceTo(getLocation()) - wave->getRadius())
    <= getWaveOnWaveMarging();
}

sf::Color Sensor::getColor() const {
  bool inhibited = inhibitor > 0.2;

  // Table de vérité pour les couleurs du senseur:
  // active  && inhibited  -> 0xff00ff (magenta)
  // active  && !inhibited -> 0xff0000 (rouge)
  // !active && inhibited  -> 0x0000ff (bleu)
  // !active && !inhibited -> 0x00ff00 (vert)
  return sf::Color(
      active ? 0xff : 0x00,
      !active && !inhibited ? 0xff : 0x00,
      inhibited ? 0xff : 0x00);
}


Vec2d Sensor::getLocation() const {
  return scorpion->getPositionOfSensor(angle);
}

Vec2d Sensor::getTargetDirection() const {
  return scorpion->getSensorRadius() * score * Vec2d(
      cos(angle + scorpion->getRotation()),
      sin(angle + scorpion->getRotation()));
}

void Sensor::increaseInhibitor(double value) {
  inhibitor = std::min(
      inhibitor + value,
      getInhibitionMax());
}


void Sensor::draw(sf::RenderTarget& targetWindow) const {
  // On dessine les liens entre un senseurs par quatre lignes de chaque  senseur
  // à un point central qui leur est commun et dont les coordonnées sont la
  // moyenne des coordonnées des senseurs.
  Vec2d center = (getLocation()
      + linkedSensors[0]->getLocation()
      + linkedSensors[1]->getLocation()
      + linkedSensors[2]->getLocation()) * 0.25;

  // Ici on est obligés de multiplier d'abord par 0.25, car sinon l'addition de
  // quatre couleurs atteint rapidement une valeur supérieure à 255 et est donc
  // limitée à 255 par SFML. La multiplication par 0.25 donne alors une valeur
  // de 64, fausse et trop sombre.
  sf::Color centerColor = (getColor() * 0.25
      + linkedSensors[0]->getColor() * 0.25
      + linkedSensors[1]->getColor() * 0.25
      + linkedSensors[2]->getColor() * 0.25);

  for (const Sensor* sensor : linkedSensors) {
    // Dessin des 3 premières lignes des autres senseurs au point central.
    targetWindow.draw(buildLine(
        sensor->getLocation(),
        center,
        sensor->getColor(), 0.2));
  }

  // Dessin du senseur courant au point central.
  targetWindow.draw(buildLine(
      getLocation(),
      center,
      getColor(), 0.2));

  // Dessin du point central.
  targetWindow.draw(buildCircle(
      center,
      1.6, centerColor));


  // Dessin du senseur.
  targetWindow.draw(buildCircle(getLocation(),
      scorpion->getRadius() / 4.0, getColor()));


  // Dessin de la cible estimée par le senseur.
  targetWindow.draw(buildLine(
        getLocation(),
        getLocation() + getTargetDirection(),
        sf::Color::Red, 2.0));
}

void Sensor::reset() {
  active = false;
  inhibitor = 0.0;
  score = 0.0;
}

void Sensor::update(sf::Time dt) {
  vector<Wave*> waves = getAppEnv()
    .getCollidingWavesForSensor(this);

  if (!active) {
    double totalIntensity(0.0);

    for (const Wave* wave : waves) {
      totalIntensity += wave->getIntensity();
    }

    if (totalIntensity >= getIntensityThreshold()) {
      active = true;
    }
  }

  // Il n'y a volontairement pas de if/else ici car 'active' peut passer à
  // 'true' entre temps.
  if (active) {
    score += 2.0 * (1.0 - inhibitor) * dt.asSeconds() * 10.0;

    for (Sensor* sensor : linkedSensors) {
      sensor->increaseInhibitor(score * getInhibitionFactor() * dt.asSeconds());
    }
  }
}

double Sensor::getInhibitionFactor() const {
  return getAppConfig().sensor_inhibition_factor;
}

double Sensor::getInhibitionMax() const {
  return getAppConfig().sensor_inhibition_max;
}

double Sensor::getIntensityThreshold() const {
  return getAppConfig().sensor_intensity_threshold;
}

double Sensor::getWaveOnWaveMarging() const {
  return getAppConfig().wave_on_wave_marging;
}


sf::Color operator*(const sf::Color& color, const double& factor) {
  return sf::Color(
      color.r * factor,
      color.g * factor,
      color.b * factor);
}

