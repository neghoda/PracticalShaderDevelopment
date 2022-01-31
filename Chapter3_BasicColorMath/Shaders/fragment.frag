#version 330 core

in vec2 uv;

uniform sampler2D tex0;
uniform vec4 multiply;
uniform vec4 add;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	FragColor = diffuseColor * multiply + add;
}
