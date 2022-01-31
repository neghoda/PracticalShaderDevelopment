#version 330 core

in vec3 fragPos;
 
uniform samplerCube tex0;

out vec4 FragColor;

void main() {
	FragColor = texture(tex0, fragPos);
}