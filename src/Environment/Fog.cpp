/*!
 * Fog
 */


#include "Fog.hpp"

#include <cmath>
#include <iostream>

#include <Application.hpp>
#include <Random/Uniform.hpp>


// Retourne la partie fractionnaire d'un nombre.
// std::modf() est plus compliquée à utiliser.
double fract(double x) {
  return x - floor(x);
}

// Crée une onde triangulaire d'amplitude 1.0 et de période 2.0.
double triangle(double x) {
  // On utilise fabs() pour éviter de tomber sur abs() de math.h qui ne
  // supporte que les entiers et renvoie donc tout le temps 0.
  return fabs(2.0 * fract(x / 2.0) - 1.0);
}

Vec2d triangle(Vec2d input) {
  return Vec2d(triangle(input.x), triangle(input.y));
}

// Échange les composantes d'un Vec2d.
Vec2d swapComponents(Vec2d input) {
  return Vec2d(input.y, input.x);
}


Fog::Fog() {
  reset();

  if (!shader.loadFromFile("res/fog.frag", sf::Shader::Fragment)) {
    std::cerr << "Could not load fog shader" << std::endl;
  }

  textureContainer.setSize(Vec2d(getWorldSize(), getWorldSize()));

  texture = getAppTexture("fog.jpg");
  textureContainer.setTexture(&texture, true);
  noiseImage = texture.copyToImage();
}

Fog::~Fog() {}

// Cette méthode reproduit le shader (quasiment) à l'identique pour ensuite
// procéder à la modification de distance de vue des animaux.
// Il faut toutefois adapter les coordonnées pour correspondre à l'intervalle
// (0.0, 1.0) utilisé dans le shader.
double Fog::computeFogIntensity(Vec2d position) const {
  Vec2d uv = Vec2d(position.x / getWorldSize(), position.y / getWorldSize());

  double a = getImageValue(triangle(uv + offset));
  double b = getImageValue(swapComponents(triangle(uv - offset)));

  return std::max(std::min((a - b) * 3.0, 0.8), 0.0) / 0.8 * 1.0;
}

void Fog::draw(sf::RenderTarget& targetWindow) const {
  if (enableFog) targetWindow.draw(textureContainer, &shader);
}

double Fog::getImageValue(Vec2d position) const {
  // On utilise fract() pour récuperer une valeur entre 0 et 1 correspondant
  // aux coordonnées de l'image. On aurait aussi pu utiliser clamping(x, 0.0, 1.0).
  // Les coordonnées sont approximées par un arrondi. On aurait pu calculer
  // la moyenne des 4 pixels adjacents au point exact pondérée par la distance
  // à ces pixels pour améliorer la précision.
  return noiseImage.getPixel(
      floor(fract(position.x) * noiseImage.getSize().x),
      floor(fract(position.y) * noiseImage.getSize().y)).r / 255.0;
}

double Fog::getWorldSize() const {
  return getAppConfig().simulation_world_size;
}

void Fog::reset() {
  angle = PI / 4.0;
  offset.x = 0.0;
  offset.y = 0.0;
}

void Fog::update(sf::Time dt) {
  // La direction du décalage est modifiée aléatoirement à chaque itération
  // d'une façon similaire au randomWalk().
  angle += getAppEnv().getRandomGenerator().uniform(-0.05, 0.05) * PI * dt.asSeconds();

  offset.x += cos(angle) * dt.asSeconds() * 0.01;
  offset.y += sin(angle) * dt.asSeconds() * 0.01;

#if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
  shader.setUniform("offset", (sf::Glsl::Vec2) offset);
  shader.setUniform("debug", isDebugOn());
#else
  shader.setParameter("offset", offset);
  shader.setParameter("debug", isDebugOn());
#endif

  /*
   * Ceci ne sert que pour vérifier le bon fonctionnement de la méthode
   * computeFogIntensity() et affiche le brouillard sans utiliser le shader.
   * La création de la texture rend ce code très lent.

    int width = getWorldSize();
    int height = getWorldSize();

    sf::Uint8* pixels = new sf::Uint8[width * height * 4];

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int index = (y * width + x) * 4;

        pixels[index + 0] = 255;
        pixels[index + 1] = 255;
        pixels[index + 2] = 255;
        pixels[index + 3] = computeFogIntensity(Vec2d(x, y)) * 0.8 * 255;
      }
    }

    sf::Image image;
    image.create(width, height, pixels);

    texture.loadFromImage(image);
    textureContainer.setTexture(&texture, true);

    delete [] pixels;
  */
}

bool Fog::isFogActive() const {
  return enableFog;
}

void Fog::setActiveFog(bool active) {
  enableFog = active;
}
