/*!
 * RandomGenerator
 */


#pragma once


#include <random>
#include <Utility/Vec2d.hpp>


class RandomGenerator {
 public:
  RandomGenerator();
  RandomGenerator(int seed);

  double exponential(double lambda);
  double normal(double mu, double sigma2);

  int uniform(int min, int max);
  double uniform(double min, double max);
  Vec2d uniform(Vec2d bounds);

  static int createSeed();

 private:
  std::default_random_engine engine;
  RandomGenerator(const RandomGenerator&);
};

