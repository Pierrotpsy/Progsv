/*!
 * Rock
 */


#pragma once


#include <Obstacle/CircularCollider.hpp>


class Rock : public CircularCollider {
 public:
  Rock(Vec2d position);

  void draw(sf::RenderTarget& targetWindow) const override;

 private:
  double rotation;

  std::string getTexture() const;
};

