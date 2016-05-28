#version 300 es

precision mediump float;
in vec4 Color;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(Color.x, Color.y, Color.z, 1.0);
}
