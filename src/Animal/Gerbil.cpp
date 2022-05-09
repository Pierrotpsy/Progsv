/*!
 * Gerbil
 */


#include <Application.hpp>
#include <Stats/StatsAccessor.hpp>

#include "Gerbil.hpp"


Gerbil::Gerbil(Vec2d position, double energy, bool isFemale, const Animal* mother)
  : Animal(position, getSize(), energy, isFemale, mother) {}

Gerbil::Gerbil(Vec2d position, const Animal* mother, double energy)
  : Gerbil(position, energy, getAppEnv().getRandomGenerator().uniform(0, 1) == 0, mother) {}

bool Gerbil::communicative(const OrganicEntity* other) const {
  return other->canCommunicate(this);
}

bool Gerbil::canCommunicate(const Gerbil*) const {
  return state != State::RUNNING_AWAY;
}

bool Gerbil::eatable(const OrganicEntity* other) const {
  return other->eatableBy(this);
}

bool Gerbil::eatableBy(const Scorpion*) const {
  return true;
}

void Gerbil::fetchStats(StatsAccessor* accessor) const {
  accessor->processEntity(this);
}

double Gerbil::getBaseViewDistance() const {
  return getAppConfig().gerbil_view_distance;
}

double Gerbil::getEnergyLossFactor() const {
  return getAppConfig().gerbil_energy_loss_factor;
}

double Gerbil::getEnergyLossFemalePerChild() const {
  return getAppConfig().gerbil_energy_loss_female_per_child;
}

double Gerbil::getEnergyLossMaleMating() const {
  return getAppConfig().gerbil_energy_loss_mating_male;
}

sf::Time Gerbil::getGestationTime() const {
  return sf::seconds(getAppConfig().gerbil_gestation_time);
}

double Gerbil::getInitialEnergy() const {
  return getAppConfig().gerbil_energy_initial;
}

sf::Time Gerbil::getLongevity() const {
  return getAppConfig().gerbil_longevity;
}

double Gerbil::getMass() const {
  return getAppConfig().gerbil_mass;
}

int Gerbil::getMaxChildren() const {
  return getAppConfig().gerbil_max_children;
}

int Gerbil::getMinChildren() const {
  return getAppConfig().gerbil_min_children;
}

sf::Time Gerbil::getMinMatingAge() const {
  return sf::seconds(getAppConfig().gerbil_min_age_mating);
}

double Gerbil::getMinMatingEnergy() const {
  return isFemale
    ? getAppConfig().gerbil_energy_min_mating_female
    : getAppConfig().gerbil_energy_min_mating_male;
}

double Gerbil::getRandomWalkDistance() const {
  return getAppConfig().gerbil_random_walk_distance;
}

double Gerbil::getRandomWalkJitter() const {
  return getAppConfig().gerbil_random_walk_jitter;
}

double Gerbil::getRandomWalkRadius() const {
  return getAppConfig().gerbil_random_walk_radius;
}

double Gerbil::getSize() const {
  return getAppConfig().gerbil_size;
}

double Gerbil::getStandardMaxSpeed() const {
  return getAppConfig().gerbil_max_speed;
}

std::string Gerbil::getTexture() const {
  return isFemale
    ? getAppConfig().gerbil_texture_female
    : getAppConfig().gerbil_texture_male;
}

double Gerbil::getViewRange() const {
  return getAppConfig().gerbil_view_range;
}

void Gerbil::giveBirth() const {
  getAppEnv().addEntity(new Gerbil(getPosition(), getAnimal()));
}

bool Gerbil::matable(const OrganicEntity* other) const {
  return other->canMate(this);
}

bool Gerbil::canMate(const Gerbil* gerbil) const {
  return gerbil->nbChildren <= 0
    && gerbil->isFemale != isFemale
    && gerbil->state != State::MATING
    && gerbil->state != State::GIVING_BIRTH
    && gerbil->energy >= gerbil->getMinMatingEnergy()
    && gerbil->age >= gerbil->getMinMatingAge();
}

