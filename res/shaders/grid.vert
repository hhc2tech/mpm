#version 330 core

layout (location = 0) in vec2 position;

out vec2 vertex;

uniform mat4 modelViewProjection;

void main() {
    gl_Position = modelViewProjection * vec4(position.x, 0, position.y, 1.0);
    vertex = position;
}