#version 330 core

in vec3 norm;
in vec2 uv;

uniform vec3 light;  
uniform sampler2D tex0;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	vec3 n = normalize(norm);
	vec3 l = normalize(light);
	float diffuseIntensity = clamp(dot(n, l) + 0.1, 0, 1);

	if (diffuseColor.r > 0 || diffuseColor.g > 0 || diffuseColor.b > 0) {
		diffuseColor = vec4(n, 1.0);
	}


	FragColor = diffuseColor * diffuseIntensity;
}