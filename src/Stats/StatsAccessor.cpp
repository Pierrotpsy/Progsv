/*!
 * StatsAccessor
 */


#include "StatsAccessor.hpp"
#include <Animal/Gerbil.hpp>


void StatsAccessor::processEntity(const Food*) {
  data.food.number += 1;
}

void StatsAccessor::processEntity(const Gerbil* gerbil) {
  gerbilAgeSum += gerbil->age.asSeconds();
  gerbilEnergySum += gerbil->energy;

  data.gerbil.number += 1;

  if (gerbil->isFemale) {
    gerbilFemaleNumber += 1;
  }

  if (gerbil->age >= gerbil->getMinMatingAge()
      && gerbil->energy >= gerbil->getMinMatingEnergy()) {
    if (gerbil->isFemale) {
      gerbilMatableFemaleNumber += 1;
    } else {
      gerbilMatableMaleNumber += 1;
    }
  }
}

void StatsAccessor::processEntity(const Scorpion*) {
  data.scorpion.number += 1;
}

const StatsAccessor::Data& StatsAccessor::updateData() {
  gerbilAgeSum = 0;
  gerbilEnergySum = 0;
  gerbilFemaleNumber = 0;
  gerbilMatableFemaleNumber = 0;
  gerbilMatableMaleNumber = 0;

  data.food.number = 0;
  data.gerbil.number = 0;
  data.scorpion.number = 0;
  data.wave.number = getAppEnv().waves.size();

  for (const auto& entity : getAppEnv().entities) {
    entity->fetchStats(this);
  }

  data.gerbil.ageMean = gerbilAgeSum / data.gerbil.number;
  data.gerbil.energyMean = gerbilEnergySum / data.gerbil.number;
  data.gerbil.femaleProportion = (double) gerbilFemaleNumber / data.gerbil.number;
  data.gerbil.matableFemaleProportion = (double) gerbilMatableFemaleNumber / data.gerbil.number;
  data.gerbil.matableMaleProportion = (double) gerbilMatableMaleNumber / data.gerbil.number;

  return data;
}

