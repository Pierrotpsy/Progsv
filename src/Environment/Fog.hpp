/*!
 * Fog
 */


#pragma once


#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Utility/Vec2d.hpp>


class Fog : public Drawable, public Updatable {
 public:
  Fog();
  ~Fog();

  void draw(sf::RenderTarget& targetWindow) const override;
  void update(sf::Time dt) override;

  // Retourne une valeur entre 0 et 1 correspondant à l'intensité du brouillard
  // au point donné à partir de coordonnées entre 0 et worldSize().
  double computeFogIntensity(Vec2d position) const;
  void reset();
  bool isFogActive() const;
  void setActiveFog(bool active);
  
 private:
  bool enableFog = true;
  double angle;
  Vec2d offset;

  // Stocke l'image de bruit de Perlin.
  sf::Image noiseImage;

  // Stocke le conteneur de la texture du brouillard.
  sf::RectangleShape textureContainer;

  // Stocke le shader pour calculer le brouillard.
  sf::Shader shader;

  // Stocke la texture du brouillard.
  sf::Texture texture;

  // Retourne une valeur entre 0 et 1 correspondant à la couleur (monochrome)
  // de l'image de bruit à partir de coordonnées entre 0 et 1.
  double getImageValue(Vec2d position) const;

  double getWorldSize() const;
};

