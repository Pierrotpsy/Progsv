/*!
 * StatsAccessor
 */


#pragma once


#include <Environment/Environment.hpp>


class Food;
class Gerbil;
class Scorpion;

class StatsAccessor {
 public:
  struct AnimalData {
    double femaleProportion;

    double ageMean;
    double energyMean;

    double matableFemaleProportion;
    double matableMaleProportion;

    unsigned int number;
  };

  struct FoodData {
    unsigned int number;
  };

  struct WaveData {
    unsigned int number;
  };

  struct Data {
    FoodData food;
    AnimalData gerbil;
    AnimalData scorpion;
    WaveData wave;
  };

  const Data& updateData();
  void processEntity(const Food*);
  void processEntity(const Gerbil* food);
  void processEntity(const Scorpion*);

 private:
  Data data;

  double gerbilAgeSum = 0.0;
  double gerbilEnergySum = 0.0;
  unsigned int gerbilFemaleNumber;
  unsigned int gerbilMatableFemaleNumber;
  unsigned int gerbilMatableMaleNumber;
};

