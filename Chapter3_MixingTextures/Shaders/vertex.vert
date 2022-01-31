#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec2 texCoord;

out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    uv = texCoord;
}
