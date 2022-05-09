/*!
 * Wave
 */


#include <Application.hpp>

#include "Wave.hpp"


Wave::Wave(Vec2d origin, double initialEnergy, double initialRadius,
  double propagationFactor,
  double propagationSpeed)
  : CircularCollider(origin, initialRadius),
  initialEnergy(initialEnergy),
  propagationFactor(propagationFactor),
  propagationSpeed(propagationSpeed)
{}

void Wave::draw(sf::RenderTarget& targetWindow) const {
  vector<int> shiftFactors { -1, 0, 1 };

  // Le mode de debug annule l'affichage sur le monde torique car celui-ci peut
  // être particulièrement gênant.
  if (isDebugOn()) shiftFactors = { 0 };

  // Pour dessiner l'onde sur le monde torique, on la dessine 9 fois avec un
  // décalage de worldSize variant entre -1, 0 et 1 sur chaque coordonnée.
  // Il faudrait en théorie la dessiner une infinité de fois car l'onde peut
  // faire le tour du tore une infinité de fois, mais en pratique elle
  // disparaît bien avant d'avoir pu en fair le tour deux fois.
  // À noter que ceci ne concerne que l'affichage, la détection des collisions
  // fonctionne de façon non-réaliste.
  for (int shiftX : shiftFactors) {
    for (int shiftY : shiftFactors) {
      for (const auto& fragment : fragments) {
        Vec2d position = getPosition() + Vec2d(shiftX, shiftY) * getAppConfig().simulation_world_size;

        Arc arc = buildArc(
            fragment.first / DEG_TO_RAD,
            fragment.second / DEG_TO_RAD,
            getRadius(), position,
            sf::Color::Black, 0.0,
            getIntensity() * getIntensityThicknessRatio());

        targetWindow.draw(arc);
      }
    }
  }
}

bool Wave::isColliderFragmenting(CircularCollider* collider) const {
  return isPointInside(collider->getPosition());
}

bool Wave::isDead() const {
  return getIntensity() < getIntensityThreshold();
}

bool Wave::isPointContainedInPath(const Vec2d& point) const {
  double angle = directionTo(point).angle();

  for (auto& fragment : fragments) {
    if (angle >= fragment.first && angle <= fragment.second) {
      return true;
    }
  }

  return false;
}

double Wave::getEnergy() const {
  return initialEnergy * exp(-getRadius() / propagationFactor);
}

double Wave::getIntensity() const {
  return getEnergy() / (2 * PI * getRadius());
}

double Wave::getIntensityThicknessRatio() const {
  return getAppConfig().wave_intensity_thickness_ratio;
}

double Wave::getIntensityThreshold() const {
  return getAppConfig().wave_intensity_threshold;
}

double Wave::getRadius() const {
  return age.asSeconds() * propagationSpeed;
}

void Wave::update(sf::Time dt) {
  age += dt;

  vector<CircularCollider*> fragmentingColliders = getAppEnv()
    .getFragmentingCollidersForWave(this);

  for (const auto& collider : fragmentingColliders) {
    double centerAngle = directionTo(*collider).angle();
    double halfViewAngle = std::atan2(collider->getRadius(), getRadius() + collider->getRadius());

    for (auto& fragment : fragments) {
      if (centerAngle >= fragment.first && centerAngle <= fragment.second) {
        fragments.push_back(std::make_pair(centerAngle + halfViewAngle, fragment.second));
        fragment.second = centerAngle - halfViewAngle;
      }
    }
  }
}

