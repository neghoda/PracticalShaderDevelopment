#version 330 core

in vec3 fragPos;
in vec2 uv0;
in vec2 uv1;
in mat3 TBN;

uniform vec3 lightPos;
uniform float lightRadius;
uniform vec4 lightColor;
uniform vec3 cameraPosition;

uniform sampler2D tex0;
uniform samplerCube tex1;

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
	vec3 n0 = texture(tex0, uv0).rgb;
	n0 = (n0 * 2.0 - 1.0);   

	
	vec3 n1 = texture(tex0, uv1).rgb;
	n1 = (n1 * 2.0 - 1.0);   

	vec3 n = normalize(TBN * (n0 + n1)); 
	vec3 toLight = lightPos - fragPos;
	float distToLight = length(toLight);
	float falloff = max(0.0, 1.0 - (distToLight / lightRadius));
	
	vec3 l = normalize(toLight);
	vec3 toCamera = normalize(cameraPosition - fragPos);

	float diffuseIntensity = diffuse(l, n);
	float specularIntensity = specular(l, toCamera, n, 512);

	vec4 diffuse = texture(tex1, (reflect(-toCamera, n))) * lightColor * diffuseIntensity * falloff;
	vec4 specular = lightColor * specularIntensity * falloff;

	FragColor = diffuse + specular;
}