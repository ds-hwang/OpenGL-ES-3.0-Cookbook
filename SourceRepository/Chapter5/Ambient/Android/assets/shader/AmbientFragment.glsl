#version 300 es

precision mediump float;

in vec4 FinalColor;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = FinalColor;
}

