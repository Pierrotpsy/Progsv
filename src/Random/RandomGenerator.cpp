/*!
 * RandomGenerator
 */


#include "RandomGenerator.hpp"

#include <iostream>


RandomGenerator::RandomGenerator() : RandomGenerator(createSeed()) {}

RandomGenerator::RandomGenerator(int seed) {
  engine.seed(seed);
}

double RandomGenerator::exponential(double lambda) {
  std::exponential_distribution<> dist(lambda);
  return dist(engine);
}

double RandomGenerator::normal(double mu, double sigma2) {
  std::normal_distribution<> dist(mu, std::sqrt(sigma2));
  return dist(engine);
}

int RandomGenerator::uniform(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(engine);
}

double RandomGenerator::uniform(double min, double max) {
  std::uniform_real_distribution<double> dist(min, max);
  return dist(engine);
}

Vec2d RandomGenerator::uniform(Vec2d bounds) {
  return Vec2d(uniform(bounds.x, bounds.y), uniform(bounds.x, bounds.y));
}

int RandomGenerator::createSeed() {
  std::random_device rd;
  return rd();
}

