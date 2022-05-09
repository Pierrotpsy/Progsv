/*!
 * Scorpion
 */


#include <Application.hpp>
#include <Stats/StatsAccessor.hpp>

#include "Scorpion.hpp"


Scorpion::Scorpion(Vec2d position, double energy, bool isFemale, const Animal* mother)
  : Animal(position, getSize(), energy, isFemale, mother) {}

Scorpion::Scorpion(Vec2d position, const Animal* mother, double energy)
  : Scorpion(position, energy, getAppEnv().getRandomGenerator().uniform(0, 1) == 0, mother) {}

bool Scorpion::communicative(const OrganicEntity* other) const {
  return other->canCommunicate(this);
}

bool Scorpion::canCommunicate(const Scorpion*) const {
  return state != State::RUNNING_AWAY;
}

bool Scorpion::eatable(const OrganicEntity* other) const {
  return other->eatableBy(this);
}

void Scorpion::fetchStats(StatsAccessor* accessor) const {
  accessor->processEntity(this);
}

double Scorpion::getBaseViewDistance() const {
  return getAppConfig().scorpion_view_distance;
}

double Scorpion::getEnergyLossFactor() const {
  return getAppConfig().scorpion_energy_loss_factor;
}

double Scorpion::getEnergyLossFemalePerChild() const {
  return getAppConfig().scorpion_energy_loss_female_per_child;
}

double Scorpion::getEnergyLossMaleMating() const {
  return getAppConfig().scorpion_energy_loss_mating_male;
}

sf::Time Scorpion::getGestationTime() const {
  return sf::seconds(getAppConfig().scorpion_gestation_time);
}

double Scorpion::getInitialEnergy() const {
  return getAppConfig().scorpion_energy_initial;
}

sf::Time Scorpion::getLongevity() const {
  return getAppConfig().scorpion_longevity;
}

double Scorpion::getMass() const {
  return getAppConfig().scorpion_mass;
}

int Scorpion::getMaxChildren() const {
  return getAppConfig().scorpion_max_children;
}

int Scorpion::getMinChildren() const {
  return getAppConfig().scorpion_min_children;
}

sf::Time Scorpion::getMinMatingAge() const {
  return sf::seconds(getAppConfig().scorpion_min_age_mating);
}

double Scorpion::getMinMatingEnergy() const {
  return isFemale
    ? getAppConfig().scorpion_energy_min_mating_female
    : getAppConfig().scorpion_energy_min_mating_male;
}

double Scorpion::getRandomWalkDistance() const {
  return getAppConfig().scorpion_random_walk_distance;
}

double Scorpion::getRandomWalkJitter() const {
  return getAppConfig().scorpion_random_walk_jitter;
}

double Scorpion::getRandomWalkRadius() const {
  return getAppConfig().scorpion_random_walk_radius;
}

double Scorpion::getSize() const {
  return getAppConfig().scorpion_size;
}

double Scorpion::getStandardMaxSpeed() const {
  return getAppConfig().scorpion_max_speed;
}

std::string Scorpion::getTexture() const {
  return getAppConfig().scorpion_texture;
}

double Scorpion::getViewRange() const {
  return getAppConfig().scorpion_view_range;
}

void Scorpion::giveBirth() const {
  getAppEnv().addEntity(new Scorpion(getPosition(), getAnimal()));
}

bool Scorpion::matable(const OrganicEntity* other) const {
  return other->canMate(this);
}

bool Scorpion::canMate(const Scorpion* scorpion) const {
  return scorpion->nbChildren <= 0
    && scorpion->isFemale != isFemale
    && scorpion->state != State::MATING
    && scorpion->state != State::GIVING_BIRTH
    && scorpion->energy >= scorpion->getMinMatingEnergy()
    && scorpion->age >= scorpion->getMinMatingAge();
}

