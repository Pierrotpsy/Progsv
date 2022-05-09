/*!
 * WaveGerbil
 */


#pragma once


#include <Animal/Gerbil.hpp>


class WaveGerbil : public Gerbil {
 public:
  WaveGerbil(Vec2d position, const Animal* mother = nullptr);
  WaveGerbil(Vec2d position, const Animal* mother, double energy);
  WaveGerbil(Vec2d position, const Animal* mother, double energy, bool isFemale);

  ~WaveGerbil();

  void update(sf::Time dt) override;

 private:
  sf::Time waveEmissionAge = sf::Time::Zero;

  double getWaveDefaultEnergy() const;
  double getWaveDefaultMu() const;
  double getWaveDefaultRadius() const;
  double getWaveDefaultSpeed() const;
  double getWaveFrequency() const;
};

