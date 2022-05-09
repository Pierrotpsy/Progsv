/*!
 * FoodGenerator
 */


#include <Application.hpp>
#include <Environment/Environment.hpp>
#include <Environment/Food.hpp>
#include <Random/Normal.hpp>

#include "FoodGenerator.hpp"


FoodGenerator::FoodGenerator() {}
FoodGenerator::~FoodGenerator() {}

sf::Time FoodGenerator::getDelta() const {
  return sf::seconds(getAppConfig().food_generator_delta);
}

void FoodGenerator::update(sf::Time dt) {
  time += dt;

  if (time > getDelta()) {
    time = sf::Time::Zero;

    double worldSize = getAppConfig().simulation_world_size;
    double mu = worldSize / 2.0;
    double sigma = pow(worldSize, 2.0) / 16.0;

    RandomGenerator& rand = getAppEnv().getRandomGenerator();
    Vec2d position(rand.normal(mu, sigma), rand.normal(mu, sigma));

    getAppEnv().addEntity(new Food(position));
  }
}

