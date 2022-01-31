#version 330 core

in vec3 fragPos;
in vec2 uv;
in mat3 TBN;

uniform vec3 light;
uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform samplerCube tex3;

out vec4 FragColor;

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	vec3 n = texture(tex2, uv).rgb;
	n = normalize(n * 2.0 - 1.0);
	n = normalize(TBN * n);
	
	vec3 l = normalize(light);
	float diffuseIntensity = clamp(dot(n, l), 0, 1);
		
	vec3 toCamera = normalize(cameraPosition - fragPos);
	vec3 halfVec = normalize(toCamera + light);
	float specularLight = clamp(dot(halfVec, n), 0, 1);
	float specularIntensity = pow(specularLight, 16.0);

	vec4 envSample = texture(tex3, reflect(-toCamera, n));
	vec4 lightColor = mix(lightColor, envSample + lightColor * 0.5, 0.5);

	vec4 diffuse = diffuseColor * lightColor * diffuseIntensity;
	vec4 specular = texture(tex1, uv) * lightColor * specularIntensity;
	vec4 ambient = diffuseColor * ambientColor;

	FragColor = ambient + diffuse + specular;
}