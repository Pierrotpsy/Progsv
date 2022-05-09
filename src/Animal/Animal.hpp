/*!
 * Animal
 */


#pragma once


#include <cmath>
#include <iostream>

#include <Environment/OrganicEntity.hpp>
#include <Random/Uniform.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Arc.hpp>
#include <Utility/Constants.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>


class Animal : public OrganicEntity {
  friend class StatsAccessor;

 public:
  enum Speed {
    high = 1,
    medium = 2,
    low = 3
  };

  Animal(Vec2d position, double size, double energy, bool isFemale, const Animal* mother);

  ~Animal();

  void clean() override;
  void communicateEnemies(const list<OrganicEntity*>& enemies) override;
  void setTargetPosition(const Vec2d& target);

  Vec2d getSpeedVector() const;
  Vec2d getDirectionToTarget() const;

  void draw(sf::RenderTarget& targetWindow) const override;
  void update(sf::Time dt) override;
  void updateState();

  double getRotation() const;
  bool isTargetInSight(Vec2d target) const;
  bool isTargetInRange(Vec2d target) const override;

  void meet(OrganicEntity* mate) override;
  void metBy(Animal* mate) override;
  void metBy(Food* mate) override;

  Vec2d convertToGlobalCoord(const Vec2d& local, 
      bool noTranslate = false) const;

  double getRadius() const override;
  double getWorldSize() const override;
  virtual double getInitialEnergy() const = 0;
  
  const Animal* getAnimal() const;
  bool isFollowingMother() const;
  
 protected:
  bool isFemale;
  unsigned int nbChildren = 0;
  State state = State::WANDERING;

  //It keeps the end age of a fixed duration state, 
  //(FEEDING, MATING, GIVING_BIRTH). When the age is reached, 
  //the animal changes state with the updateState() method,
  //but this variable isn't overwritten yet.
  sf::Time interactionEndAge;
  
  std::tuple<OrganicEntity*, OrganicEntity*> analyzeEnvironment();
  
  double getDefaultDebugTextSize() const;
  sf::Color getDebugTextColor() const;
  virtual double getStandardMaxSpeed() const = 0;

  Vec2d getStoppingForce() const;
  Vec2d getTargetingForce() const;

  void updatePosition(sf::Time dt);
  void setRotation(const double& angle);
  void randomWalk();
  
 private:
  Vec2d direction;
  Vec2d targetPosition;
  Vec2d relativeWanderingTarget = Vec2d(1, 0);
  std::list<OrganicEntity*> enemies;
  const Animal* mother;
  //It shows the age at which the gestation will be done. This one is independent
  //from the global age, eventhough it's fixed at the end of the MATING state.
  //If the indicated age is reached while the animal is in a state other than WANDERING,
  //the delay linked to that state slows down the change to GIVING_BIRTH.
  sf::Time gestationEndAge;

  double deceleration;
  double speed;
  
	bool isYoung() const;
  
  double getGrowthFactor() const;
  double getViewDistance() const;
  bool isPregnant() const;
	
  virtual double getBaseViewDistance() const = 0;
  virtual double getSize() const = 0;
  virtual double getMass() const = 0;
  virtual double getViewRange() const = 0;
  virtual double getRandomWalkDistance() const = 0;
  virtual double getRandomWalkJitter() const = 0;
  virtual double getRandomWalkRadius() const = 0;
  virtual std::string getTexture() const = 0;
  virtual double getEnergyLossFactor() const = 0;
  virtual double getMinMatingEnergy() const = 0;
  virtual sf::Time getMinMatingAge() const = 0;
  virtual int getMinChildren() const = 0;
  virtual int getMaxChildren() const = 0;
  virtual double getEnergyLossFemalePerChild() const = 0;
  virtual double getEnergyLossMaleMating() const = 0;
  virtual sf::Time getGestationTime() const = 0;

  virtual Vec2d getForce() const;
  virtual double getMaxSpeed() const;

  double getMinEnergy() const override;
  double getAnimalBaseEnergyConsumption() const;

  //It adds a new entity (same as the mother) to the mothers position.
  virtual void giveBirth() const = 0;
  virtual void drawDebugInfo(sf::RenderTarget& targetWindow) const;
};

