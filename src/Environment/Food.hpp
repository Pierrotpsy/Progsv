/*!
 * Food
 */


#pragma once


#include <Environment/OrganicEntity.hpp>


class Food : public OrganicEntity {
 public:
  Food(Vec2d position);
  ~Food();
  std::string getTexture() const;

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& targetWindow) const override;

  bool communicative(const OrganicEntity* other) const override;

  bool eatable(const OrganicEntity* entity) const override;
  bool eatableBy(const Gerbil* gerbil) const override;

  bool matable(const OrganicEntity* other) const override;

  void meet(OrganicEntity* mate) override;
  void metBy(Animal* mate) override;
  void metBy(Food* mate) override;

  double getMinEnergy() const override;
  sf::Time getLongevity() const override;
  
  void fetchStats(StatsAccessor* accessor) const override;
};

