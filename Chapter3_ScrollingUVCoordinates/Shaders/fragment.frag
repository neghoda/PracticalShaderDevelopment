#version 330 core

in vec2 uv;

uniform sampler2D tex0;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	FragColor = diffuseColor;
}