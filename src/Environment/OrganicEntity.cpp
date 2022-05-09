/*!
 * OrganicEntity
 */


#include "OrganicEntity.hpp"


OrganicEntity::OrganicEntity(Vec2d position1, double size, double energy1)
  : CircularCollider(position1, size / 2.0),
  energy(energy1) {}

OrganicEntity::~OrganicEntity() {}

bool OrganicEntity::canCommunicate(const Food*) const {
  return false;
}

bool OrganicEntity::canCommunicate(const Gerbil*) const {
  return false;
}

bool OrganicEntity::canCommunicate(const Scorpion*) const {
  return false;
}

bool OrganicEntity::canMate(const Food*) const {
  return false;
}

bool OrganicEntity::canMate(const Gerbil*) const {
  return false;
}

bool OrganicEntity::canMate(const Scorpion*) const {
  return false;
}

void OrganicEntity::communicateEnemies(const std::list<OrganicEntity*>&) {}

void OrganicEntity::clean() {}

bool OrganicEntity::eatableBy(const Food*) const {
  return false;
}

bool OrganicEntity::eatableBy(const Gerbil*) const {
  return false;
}

bool OrganicEntity::eatableBy(const Scorpion*) const {
  return false;
}

bool OrganicEntity::isDead() const {
  // On met <= car l'energie est exactement egale a 0 lorsque
  // une entite organique est mangee.
  return age > getLongevity() || energy <= getMinEnergy();
}

sf::Time OrganicEntity::getLongevity() const {
  return sf::seconds(1E9);
}

bool OrganicEntity::isTargetInRange(Vec2d) const {
  return false;
}

void OrganicEntity::update(sf::Time dt) {
  age += dt;
}
