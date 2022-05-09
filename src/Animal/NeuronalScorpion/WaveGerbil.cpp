/*!
 * WaveGerbil
 */


#include "WaveGerbil.hpp"


WaveGerbil::WaveGerbil(Vec2d position, const Animal* mother)
  : Gerbil(position, mother) {}

WaveGerbil::WaveGerbil(Vec2d position, const Animal* mother, double energy)
  : Gerbil(position, mother, energy) {}

WaveGerbil::WaveGerbil(Vec2d position, const Animal* mother, double energy, bool isFemale)
  : Gerbil(position, energy, isFemale, mother) {}

WaveGerbil::~WaveGerbil() {}

void WaveGerbil::update(sf::Time dt) {
  Gerbil::update(dt);

  if (age >= waveEmissionAge) {
    getAppEnv().addWave(new Wave(
          getPosition(),
          getWaveDefaultEnergy(),
          getWaveDefaultRadius(),
          getWaveDefaultMu(),
          getWaveDefaultSpeed()));

    waveEmissionAge += sf::seconds(1.0 / getWaveFrequency());
  }
}

double WaveGerbil::getWaveDefaultEnergy() const {
  return getAppConfig().wave_default_energy;
}

double WaveGerbil::getWaveDefaultMu() const {
  return getAppConfig().wave_default_mu;
}

double WaveGerbil::getWaveDefaultRadius() const {
  return getAppConfig().wave_default_radius;
}

double WaveGerbil::getWaveDefaultSpeed() const {
  return getAppConfig().wave_default_speed;
}

double WaveGerbil::getWaveFrequency() const {
  return getAppConfig().wave_gerbil_frequency;
}

