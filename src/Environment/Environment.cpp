/*!
 * Environment
 */


#include <algorithm>

#include <Animal/Animal.hpp>
#include <Environment/FoodGenerator.hpp>

#include "Environment.hpp"

using std::list;


Environment::Environment() {}
Environment::Environment(int seed) : randomGenerator(seed) {}

Environment::~Environment() {
  clean();
}

void Environment::addEntity(OrganicEntity* entity) {
  entities.push_back(entity);
}

void Environment::addGenerator(FoodGenerator* generator) {
  generators.push_back(generator);
}

void Environment::addObstacle(CircularCollider* obstacle) {
  obstacles.push_back(obstacle);
}

void Environment::addWave(Wave* wave) {
  waves.push_back(wave);
}

void Environment::clean() {
  clearPointerList(generators);
  clearPointerList(entities);
  clearPointerList(obstacles);
  clearPointerList(waves);

  Time = sf::Time::Zero;
  fog.reset();
}

void Environment::draw(sf::RenderTarget& targetWindow) const {
  for (auto& entity : entities) {
    entity->draw(targetWindow);
  }

  for (auto& obstacle : obstacles) {
    obstacle->draw(targetWindow);
  }

  for (auto& wave : waves) {
    wave->draw(targetWindow);
  }

  if (fog.isFogActive()) {
    fog.draw(targetWindow);
  }
}

vector<Wave*> Environment::getCollidingWavesForSensor(const Sensor* sensor) const {
  return filterImmutableList<Wave*>(waves, [&sensor](Wave* wave) {
    return sensor->isWaveColliding(wave);
  });
}

vector<OrganicEntity*> Environment::getEntitiesInSightForAnimal(const Animal* animal) const {
  return filterImmutableList<OrganicEntity*>(entities, [&animal](OrganicEntity* entity) {
    return animal->isTargetInSight(entity->getPosition());
  });
}

vector<OrganicEntity*> Environment::getEntitiesInRangeForAnimal(const Animal* animal) const {
  return filterImmutableList<OrganicEntity*>(entities, [&animal](OrganicEntity* entity) {
    return animal->isTargetInRange(entity->getPosition());
  });
}

vector<CircularCollider*> Environment::getFragmentingCollidersForWave(const Wave* wave) const {
  return filterImmutableList<CircularCollider*>(obstacles, [&wave](CircularCollider* collider) {
    return wave->isColliderFragmenting(collider);
  });
}

sf::Time Environment::getTime() const {
  return Time;
}

double Environment::getFogIntensity(Vec2d position) const {
  return fog.isFogActive() ? fog.computeFogIntensity(position) : 0.0;
}

RandomGenerator& Environment::getRandomGenerator() {
  return randomGenerator;
}

void Environment::update(sf::Time dt) {
  Time += dt;
	
  for (auto& generator : generators) {
    generator->update(dt);
  }

  for (auto& entity : entities) {
    entity->update(dt);
  }

  for (auto& wave : waves) {
    wave->update(dt);
  }


  for (OrganicEntity* entity : entities) {
    entity->clean();
  }

  filterPointerList<OrganicEntity>(entities, [](const OrganicEntity* entity) {
    return !entity->isDead();
  });
  
  filterPointerList<Wave>(waves, [](const Wave* wave) {
    return !wave->isDead();
  });

  if (fog.isFogActive()) {
    fog.update(dt);
  }
}

void Environment::toggleFog() {
  if(fog.isFogActive()) {
    fog.setActiveFog(false);
  } else {
    fog.setActiveFog(true);
  }
}

bool Environment::getEnvActiveFog() {
  return fog.isFogActive();
}
