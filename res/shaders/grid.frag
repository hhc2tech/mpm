#version 330 core

in vec2 vertex;
out vec4 outputColor;

uniform vec3 color;

void main() {
    outputColor = vec4(color, 1.0);
}