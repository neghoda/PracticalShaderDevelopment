#version 330 core

in vec3 fragPos;
in vec2 uv;
in mat3 TBN;

uniform vec3 lightPos;
uniform float lightRadius;
uniform vec4 lightColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform samplerCube tex3;

out vec4 FragColor;

float diffuse(vec3 lightDir, vec3 nrm) {
	float diffAmt = clamp(dot(nrm, lightDir), 0, 1);
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess) {
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = clamp(dot(halfVec, nrm), 0, 1);
	return pow(specAmt, shininess);
}

void main() {
	vec4 diffuseColor = texture(tex0, uv);

	vec3 n = texture(tex2, uv).rgb;
	n = normalize(n * 2.0 - 1.0);
	n = normalize(TBN * n);
	
	vec3 toLight = lightPos - fragPos;
	float distToLight = length(toLight);
	float falloff = max(0.0, 1.0 - (distToLight / lightRadius));
	
	vec3 l = normalize(toLight);
	vec3 toCamera = normalize(cameraPosition - fragPos);

	float diffuseIntensity = diffuse(l, n);
	float specularIntensity = specular(l, toCamera, n, 16.0);

	vec4 envSample = texture(tex3, reflect(-toCamera, n));
	vec4 lightColor = mix(lightColor, envSample + lightColor * 0.5, 0.5);

	vec4 diffuse = diffuseColor * lightColor * diffuseIntensity * falloff;
	vec4 specular = texture(tex1, uv) * lightColor * specularIntensity * falloff;

	FragColor = diffuse + specular;
}