#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 color;

out vec3 fragmentColor; 

void main() {
    fragmentColor = color;
    gl_Position = vec4(position, 1.0);
}
