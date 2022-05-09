/*!
 * Wave
 */


#pragma once


#include <vector>
#include <utility>

#include <Obstacle/CircularCollider.hpp>
#include <Interface/Updatable.hpp>

using std::pair;
using std::vector;


class Wave : public CircularCollider, public Updatable {
 public:
  Wave(Vec2d origin, double initialEnergy, double initialRadius,
    double propagationFactor,
    double propagationSpeed);

  void draw(sf::RenderTarget& targetWindow) const override;
  void update(sf::Time time) override;

  double getIntensity() const;
  double getEnergy() const;

  // On réécrit la méthode getRadius(), de cette façon on ne touche à aucun
  // attribut tout en gardant fonctionnelles les méthodes de CircularCollider.
  double getRadius() const override;

  bool isColliderFragmenting(CircularCollider* collider) const;
  bool isPointContainedInPath(const Vec2d& point) const;
  bool isDead() const;

 private:
  sf::Time age = sf::Time::Zero;
  vector<pair<double, double>> fragments = { std::make_pair(-PI, PI) };

  double initialEnergy;
  double propagationFactor;
  double propagationSpeed;

  double getIntensityThicknessRatio() const;
  double getIntensityThreshold() const;
};

