/*!
 * Food
 */


#include <Application.hpp>
#include <Stats/StatsAccessor.hpp>

#include "Food.hpp"


Food::Food(Vec2d position)
  : OrganicEntity(position, getAppConfig().food_size, 
  getAppConfig().food_energy) {}

Food::~Food() {}

bool Food::communicative(const OrganicEntity* other) const {
  return other->canCommunicate(this);
}

void Food::draw(sf::RenderTarget& targetWindow) const {
  targetWindow.draw(buildSprite(
        getPosition(), getRadius(),
        getAppTexture(getTexture())));
}

bool Food::eatable(OrganicEntity const* entity) const {
  return entity->eatableBy(this);
}

bool Food::eatableBy(Gerbil const*) const {
  return true;
}

void Food::fetchStats(StatsAccessor* accessor) const {
  accessor->processEntity(this);
}

sf::Time Food::getLongevity() const {
  return sf::seconds(52);
}

double Food::getMinEnergy() const {
  return 0;
}

std::string Food::getTexture() const {
  return getAppConfig().food_texture;
}

bool Food::matable(const OrganicEntity* other) const {
  return other->canMate(this);
}

void Food::meet(OrganicEntity* mate) {
  mate->metBy(this);
}

void Food::metBy(Animal*) {}

void Food::metBy(Food*) {}

void Food::update(sf::Time dt) {
  OrganicEntity::update(dt);
}

