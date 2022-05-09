/*!
 * Environment
 */


#pragma once


#include <functional>
#include <list>
#include <vector>

#include <Animal/NeuronalScorpion/Sensor.hpp>
#include <Environment/Fog.hpp>
#include <Environment/FoodGenerator.hpp>
#include <Environment/OrganicEntity.hpp>
#include <Environment/Wave.hpp>
#include <Random/RandomGenerator.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>

using std::list;
using std::vector;


class Animal;

class Environment : public Drawable, public Updatable {
  friend class StatsAccessor;

 public:
  Environment();
  Environment(int seed);
  ~Environment();

  void addEntity(OrganicEntity* entity);
  void addGenerator(FoodGenerator* generator);
  void addObstacle(CircularCollider* obstacle);
  void addWave(Wave* wave);
  
  void draw(sf::RenderTarget& targetWindow) const override;

  //Chaque Wave est testée par le senseur pour voir si elle entre en collision avec lui.
  vector<Wave*> getCollidingWavesForSensor(const Sensor* sensor) const;
  //Chaque rocher est testé par la Wave pour voir si son centre entre en collision avec elle.
  vector<CircularCollider*> getFragmentingCollidersForWave(const Wave* wave) const;
  //Chaque entité est testée par l'animal pour voir si elle est dans son champ de vision.
  vector<OrganicEntity*> getEntitiesInSightForAnimal(const Animal* animal) const;
    //Chaque entité est testée par l'animal pour voir si elle est dans son rayon de vue
  vector<OrganicEntity*> getEntitiesInRangeForAnimal(const Animal* animal) const;
  sf::Time getTime() const;
  RandomGenerator& getRandomGenerator();
  
  void clean();
  void update(sf::Time dt) override;
  void toggleFog();
  
  double getFogIntensity(Vec2d position) const;
  bool getEnvActiveFog();
 private:
  list<OrganicEntity*> entities;
  list<FoodGenerator*> generators;
  list<CircularCollider*> obstacles;
  list<Wave*> waves;

  Fog fog;
  sf::Time Time = sf::Time::Zero;
  RandomGenerator randomGenerator;

  Environment(const Environment&);
};

