/*!
 * Scorpion
 */


#pragma once


#include <Animal/Animal.hpp>
#include <Application.hpp>


class Scorpion : public Animal {
 public:
  Scorpion(Vec2d position, double energy, bool isFemale, const Animal* mother);
  Scorpion(Vec2d position, const Animal* mother = nullptr,
    double energy = getAppConfig().scorpion_energy_initial);

  double getBaseViewDistance() const override;
  double getInitialEnergy() const override;
  double getMass() const override;
  double getRandomWalkDistance() const override;
  double getRandomWalkJitter() const override;
  double getRandomWalkRadius() const override;
  double getSize() const override;
  double getStandardMaxSpeed() const override;
  std::string getTexture() const override;
  double getViewRange() const override;
  sf::Time getLongevity() const override;
  double getEnergyLossFactor() const override;
  double getMinMatingEnergy() const override;
  sf::Time getMinMatingAge() const override;
  int getMinChildren() const override;
  int getMaxChildren() const override;
  double getEnergyLossFemalePerChild() const override;
  double getEnergyLossMaleMating() const override;
  sf::Time getGestationTime() const override;

  bool communicative(const OrganicEntity* other) const override;
  bool canCommunicate(const Scorpion* scorpion) const override;

  bool eatable(const OrganicEntity* entity) const override;

  bool matable(const OrganicEntity* other) const override;
  bool canMate(const Scorpion* scorpion) const override;

  void giveBirth() const override;
  void fetchStats(StatsAccessor* accessor) const override;
};

