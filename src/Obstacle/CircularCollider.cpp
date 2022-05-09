/*!
 * Obstacle / CircularCollider
 */


#include <iostream>

#include <Utility/Vec2d.hpp>
#include <Application.hpp>

#include "CircularCollider.hpp"


CircularCollider::CircularCollider(Vec2d position, double radius)
  : position(clamping(position, 0, getWorldSize())), radius(radius) {
  if (radius < 0) {
    throw "Negative radius";
  }
}

CircularCollider::CircularCollider(const CircularCollider& body)
  : position(body.position), radius(body.radius) {}

Vec2d CircularCollider::directionTo(const CircularCollider& to) const {
  return directionTo(to.position);
}

Vec2d CircularCollider::directionTo(const Vec2d& to) const {
  double halfSize = getWorldSize() / 2.0;
  return clamping(to - position, -halfSize, +halfSize);
}

double CircularCollider::distanceTo(const CircularCollider& to) const {
  return distanceTo(to.position);
}

double CircularCollider::distanceTo(const Vec2d& to) const {
  return directionTo(to).length();
}

void CircularCollider::draw(sf::RenderTarget&) const {}


bool CircularCollider::isCircularColliderInside(const CircularCollider& other) const {
  return getRadius() >= other.getRadius() && distanceTo(other) <= (getRadius() - other.getRadius());
}

bool CircularCollider::isColliding(const CircularCollider& other) const {
  return distanceTo(other) <= (getRadius() + other.getRadius());
}

bool CircularCollider::isPointInside(const Vec2d& point) const {
  return distanceTo(point) <= getRadius();
}

void CircularCollider::move(const Vec2d& dx) {
  position = clamping(position + dx, 0, getWorldSize());
}

const Vec2d& CircularCollider::getPosition() const {
  return position;
}

double CircularCollider::getRadius() const {
  return radius;
}

double CircularCollider::getWorldSize() const {
	return getAppConfig().simulation_world_size;
}

bool CircularCollider::operator>(const Vec2d& point) const {
  return isPointInside(point);
}

CircularCollider& CircularCollider::operator=(const CircularCollider& body) {
  position = body.position;
  radius = body.radius;

  return *this;
}

CircularCollider& CircularCollider::operator+=(const Vec2d& dx) {
  move(dx);
  return *this;
}

bool operator|(const CircularCollider& body1, const CircularCollider& body2) {
  return body1.isColliding(body2);
}


bool operator>(const CircularCollider& body1, const CircularCollider& body2) {
  return body1.isCircularColliderInside(body2);
}

ostream& operator<<(ostream& out, const CircularCollider& body) {
  out << "CircularCollider: position = " << body.getPosition()
      << " radius = " << body.getRadius();
  return out;
}


double clamping(double comp, double min, double max) {
  while (comp < min) {
    comp += (max - min);
  }

  while (comp > max) {
    comp -= (max - min);
  }

  return comp;
}

Vec2d clamping(Vec2d comp, double min, double max) {
  return Vec2d(
      clamping(comp.x, min, max),
      clamping(comp.y, min, max));
}

