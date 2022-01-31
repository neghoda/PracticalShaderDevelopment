#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;

out vec3 fragPos;

void main() {
    gl_Position = vec4(projection * view * model * vec4(position, 1.0)).xyww;
    
    fragPos = position;
}
