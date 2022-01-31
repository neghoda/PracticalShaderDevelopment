#version 330 core

in vec2 uv;

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 FragColor;

void main() {
	vec4 color0 = texture(tex0, uv);
	vec4 color1 = texture(tex1, uv);

	FragColor = mix(color1, color0, color1.r);
}
