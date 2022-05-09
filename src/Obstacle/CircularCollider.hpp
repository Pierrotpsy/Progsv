/*!
 * Obstacle / CircularCollider
 */


#pragma once


#include <iostream>

#include <Interface/Drawable.hpp>
#include <Utility/Vec2d.hpp>

using std::ostream;


class CircularCollider : Drawable {
 public:
  virtual double getWorldSize() const;
 
  CircularCollider(Vec2d vPosition, double vRadius);
  CircularCollider(const CircularCollider& body);

  Vec2d directionTo(const Vec2d& to) const;
  Vec2d directionTo(const CircularCollider& to) const;

  double distanceTo(const Vec2d& to) const;
  double distanceTo(const CircularCollider& to) const;

  void draw(sf::RenderTarget& targetWindow) const override;

  const Vec2d& getPosition() const;
  virtual double getRadius() const;

  // Appplique l'algorithme de clamping sur un Vec2d.
  void move(const Vec2d& dx);
  
  // Vérifie si l'autre CircularCollider est à l'intérieur de celui-ci.
  bool isCircularColliderInside(const CircularCollider& other) const;

  // Vérifie si l'autre CircularCollider est en collision avec celui-ci.
  bool isColliding(const CircularCollider& other) const;

  // Vérifie si le point se situe à l'intérieur du CircularCollider.
  bool isPointInside(const Vec2d& point) const;
  
  CircularCollider& operator+=(const Vec2d& dx);
  CircularCollider& operator=(const CircularCollider& body);
  bool operator>(const Vec2d& point) const;

 private:
  Vec2d position;
  double radius;
};

bool operator>(const CircularCollider& body1, const CircularCollider& body2);
bool operator|(const CircularCollider& body1, const CircularCollider& body2);
ostream& operator<<(ostream& out, const CircularCollider& body);


// Corrige la valeur entrée pour qu'elle soit comprise entre min et max.
double clamping(double comp, double min, double max);
Vec2d clamping(Vec2d comp, double min, double max);

