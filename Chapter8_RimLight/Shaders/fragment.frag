#version 330 core

in vec3 norm;
in vec3 fragPos;
in vec2 uv;

uniform vec3 light;
uniform vec4 lightColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	vec3 n = normalize(norm);
	vec3 l = normalize(light);
	float diffuseIntensity = clamp(dot(n, l), 0, 1);

	vec3 toCamera = normalize(cameraPosition - fragPos);
	float rimIntensity = 1.0 - clamp(dot(n, toCamera), 0, 1);
	rimIntensity = pow(rimIntensity, 2);

	FragColor = diffuseColor * lightColor * diffuseIntensity + rimIntensity;
}