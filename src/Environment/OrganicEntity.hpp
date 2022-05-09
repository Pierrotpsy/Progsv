/*!
 * OrganicEntity
 */


#pragma once


#include <list>
#include <Interface/Updatable.hpp>
#include <Obstacle/CircularCollider.hpp>
#include <SFML/Graphics.hpp>


class Animal;
class Food;
class Gerbil;
class Scorpion;
class StatsAccessor;


class OrganicEntity : public CircularCollider, public Updatable {
  friend class StatsAccessor;

 public:
  enum State {
    FOOD_IN_SIGHT,
    FEEDING,
    RUNNING_AWAY,
    MATE_IN_SIGHT,
    MATING,
    GIVING_BIRTH,
    WANDERING
  };
  
  OrganicEntity(Vec2d position1, double size, double energy1);
  virtual ~OrganicEntity();
  
  virtual void communicateEnemies(const std::list<OrganicEntity*>& enemies);

  virtual bool eatable(OrganicEntity const* entity) const = 0;
  virtual bool eatableBy(Food const* food) const;
  virtual bool eatableBy(Gerbil const* gerbil) const;
  virtual bool eatableBy(Scorpion const* scorpion) const;
	
  virtual void fetchStats(StatsAccessor* accessor) const = 0;

  bool isDead() const;
  
  virtual bool isTargetInRange(Vec2d) const;

  virtual bool communicative(const OrganicEntity* other) const = 0;
  virtual bool canCommunicate(const Food* food) const;
  virtual bool canCommunicate(const Gerbil* gerbil) const;
  virtual bool canCommunicate(const Scorpion* scorpion) const;

  virtual bool matable(const OrganicEntity* other) const = 0;
  virtual bool canMate(const Scorpion* scorpion) const;
  virtual bool canMate(const Gerbil* gerbil) const;
  virtual bool canMate(const Food* food) const;

  virtual void meet(OrganicEntity* mate) = 0;
  virtual void metBy(Animal* mate) = 0;
  virtual void metBy(Food* mate) = 0;

  virtual void clean();
  void update(sf::Time dt) override;
  
  double energy;

 protected:
  sf::Time age = sf::Time::Zero;

 private:
  virtual sf::Time getLongevity() const;
  virtual double getMinEnergy() const = 0;
};

