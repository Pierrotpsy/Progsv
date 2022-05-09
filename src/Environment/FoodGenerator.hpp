/*!
 * FoodGenerator
 */


#pragma once


#include <Interface/Updatable.hpp>
#include <SFML/Graphics.hpp>


class Environment;

class FoodGenerator : public Updatable {
 public:
  FoodGenerator();
  ~FoodGenerator();

  sf::Time getDelta() const;
  void update(sf::Time dt) override;

 private:
  sf::Time time = sf::Time::Zero;
};

