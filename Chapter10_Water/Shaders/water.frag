#version 330 core

in vec3 fragPos;
in vec2 uv0;
in vec2 uv1;
in mat3 TBN;

uniform vec3 light;
uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;

out vec4 FragColor;

void main() {
	vec3 n0 = texture(tex0, uv0).rgb;
	n0 = (n0 * 2.0 - 1.0);   

	
	vec3 n1 = texture(tex0, uv1).rgb;
	n1 = (n1 * 2.0 - 1.0);   

	vec3 n = normalize(TBN * (n0 + n1)); 
	vec3 l = normalize(light);

	float diffuseIntensity = clamp(dot(n, l), 0, 1);
		
	vec3 toCamera = normalize(cameraPosition - fragPos);
	vec3 halfVec = normalize(toCamera + light);
	float specularLight = clamp(dot(halfVec, n), 0, 1);
	float specularIntensity = pow(specularLight, 512);

	vec4 diffuse = vec4(0.3, 0.3, 0.4, 1.0) * lightColor * diffuseIntensity;
	vec4 specular = lightColor * specularIntensity;

	FragColor = ambientColor + diffuse + specular;
}