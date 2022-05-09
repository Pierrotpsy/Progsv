/*!
 * NeuronalScorpion
 */


#pragma once


#include <Animal/Scorpion.hpp>


class Sensor;

class NeuronalScorpion : public Scorpion {
 public:
  enum NeuronalState {
    IDLE,
    MOVING,
    TARGET_IN_SIGHT,
    WANDERING
  };

  NeuronalScorpion(Vec2d position, const Animal* mother = nullptr,
    double energy = getAppConfig().scorpion_energy_initial);
  NeuronalScorpion(Vec2d position, double energy, bool isFemale, const Animal* mother = nullptr);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& targetWindow) const override;

  Vec2d getPositionOfSensor(double angle) const;
  double getSensorRadius() const;

 private:
  Vec2d targetDirection;
  sf::Time sensorActivityEndAge;
  vector<Sensor> sensors;
  NeuronalState state = NeuronalState::WANDERING;

  Vec2d getForce() const override;
  double getMaxSpeed() const override;

  Vec2d getSensorsTargetDirection() const;
  bool isSensorActive() const;

  void drawDebugInfo(sf::RenderTarget& targetWindow) const override;
  void updateState();

  double getMinimalScoreForAction() const;
  double getRotationAnglePrecision() const;
  sf::Time getSensorActivationDuration() const;
};

