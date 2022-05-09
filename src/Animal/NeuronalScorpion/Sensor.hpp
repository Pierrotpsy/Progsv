/*!
 * Sensor
 */


#pragma once


#include <Environment/Wave.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>


class NeuronalScorpion;

class Sensor : public Drawable, public Updatable {
 public:
  Sensor(NeuronalScorpion* scorpion, double angle,
    double score = 0, double inhibitor = 0, bool active = false);

  ~Sensor();

  Vec2d getTargetDirection() const;
  
  void addLinkedSensor(Sensor* sensor);
  bool isActive() const;
  bool isWaveColliding(Wave* wave) const;
  //Resests all the values of a sensor to their original state.
  void reset();

  void draw(sf::RenderTarget& targetWindow) const override;
  void update(sf::Time dt) override;

 private:
  NeuronalScorpion* scorpion;
  double angle;
  vector<Sensor*> linkedSensors;

  bool active;
  double score;
  double inhibitor;

	//Checks if a sensor is active and its inhbitor
	//and returns the corresponding color.
  sf::Color getColor() const;
  //Returns the global location of a sensor.
  Vec2d getLocation() const;
  //Increases the inhibitor by a certain value, 
  //but checks that it never crosses a certain threshold.
  void increaseInhibitor(double value);

  double getInhibitionFactor() const;
  double getInhibitionMax() const;
  double getIntensityThreshold() const;
  double getWaveOnWaveMarging() const;
};


sf::Color operator*(const sf::Color& color, const double& factor);

