/*!
 * Fog shader
 */


uniform bool debug;
uniform vec2 offset;
uniform sampler2D texture;


// Cette fonction crée une onde triangulaire d'amplitude 1.0 et de période 2.0.
// Elle sert à répeter l'image de bruit initiale alternativement identiquement
// et en miroir pour éviter des effets visuellement désagréables, l'image
// n'étant pas continue entre bords.
vec2 triangle(vec2 x) {
  return abs(2.0 * fract(x / 2.0) - 1.0);
}

void main() {
  vec2 uv = gl_TexCoord[0].xy;

  // L'uniform 'offset' permet d'animer le brouillard en décalant le point de
  // lecture de l'image.
  // On utilise une image de bruit de Perlin prédéfinie pour éviter les calculs
  // trop compliqués et coûteux.
  // Source de l'image: https://code.tutsplus.com/tutorials/create-an-organic-dissolve-with-perlin-noise--active-8237
  vec4 a = texture2D(texture, triangle((uv + offset).xy));
  vec4 b = texture2D(texture, triangle((uv - offset).yx));

  // L'image étant de bruit étant en noir et blanc, les trois composantes
  // R, G et V sont identiques, on peut utiliser n'importe laquelle.
  // On limite le canal alpha à 0.8 pour éviter des blancs trop forts.
  vec4 pixel = vec4(1.0, 1.0, 1.0, min((a - b).x * 3.0, 0.8));

  // On utilise un effet de cel shading lorsqu'en mode debug.
  pixel = debug
    ? floor(pixel * 5.0) / 5.0
    : pixel;

  gl_FragColor = gl_Color * pixel;
}

