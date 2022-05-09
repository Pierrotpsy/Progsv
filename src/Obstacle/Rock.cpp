/*!
 * Rock
 */


#include <Application.hpp>
#include <Random/Uniform.hpp>

#include "Rock.hpp"


Rock::Rock(Vec2d position)
  : CircularCollider(position, std::max(1.0,
      uniform(
        getAppConfig().simulation_world_size / 50.0,
        getAppConfig().simulation_world_size / 50.0 * 2.0))),
  rotation(uniform(-PI, PI)) {}

void Rock::draw(sf::RenderTarget& targetWindow) const {
  targetWindow.draw(buildSprite(
      getPosition(),
      getRadius() * 2.0,
      getAppTexture(getTexture()),
      rotation / DEG_TO_RAD));
}

std::string Rock::getTexture() const {
  return "rock.png";
}

