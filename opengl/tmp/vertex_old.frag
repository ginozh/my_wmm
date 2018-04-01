attribute vec2 position;
attribute vec2 a_texCoord;

uniform vec2 u_resolution;
varying vec2 texCoord;
varying vec2 textureCoord;
void main(void) {
  gl_Position = vec4(position, 0, 1);
  //texCoord = position;
  //textureCoord = position;
  texCoord = position* 0.5 + 0.5;
  textureCoord = position* 0.5 + 0.5;
}
