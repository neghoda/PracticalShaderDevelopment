#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 size;
uniform vec2 offset;

in vec3 position;
in vec2 texCoord;

out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    uv = texCoord * size + (size * offset);
}
