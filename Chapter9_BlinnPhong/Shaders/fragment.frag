#version 330 core

in vec3 norm;
in vec3 fragPos;
in vec2 uv;

uniform vec3 light;
uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	vec3 n = normalize(norm);
	vec3 l = normalize(light);
	float diffuseIntensity = clamp(dot(n, l), 0, 1);
		
	vec3 toCamera = normalize(cameraPosition - fragPos);
	vec3 halfVec = normalize(toCamera + light);
	float specularLight = clamp(dot(halfVec, n), 0, 1);
	float specularIntensity = pow(specularLight, 16.0);

	vec4 diffuse = diffuseColor * lightColor * diffuseIntensity;
	vec4 specular = diffuseColor * lightColor * specularIntensity;
	vec4 ambient = diffuseColor * ambientColor;

	FragColor = ambient + diffuse + specular;
}