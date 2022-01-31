#version 330 core

struct DirectionalLight {
	vec3 direction;
	vec3 color;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float radius;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float cutoff;
};

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2
uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform vec3 ambientColor;
uniform vec3 cameraPosition;

in vec3 fragPos;
in vec2 uv;
in mat3 TBN;

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
	vec3 diffuseColor = texture(tex0, uv).xyz;

	vec3 n = texture(tex2, uv).rgb;
	n = normalize(n * 2.0 - 1.0);
	n = normalize(TBN * n);
	vec3 toCamera = normalize(cameraPosition - fragPos);
	float specMask = texture(tex1, uv).x;
	vec3 envSample = texture(tex3, reflect(-toCamera, n)).xyz;

	vec3 finalColor = vec3(0,0,0);

	for (int i = 0; i < NUM_DIR_LIGHTS; ++i) {
		DirectionalLight light = directionalLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		float diffAmt = diffuse(light.direction, n);
		float specAmt = specular(light.direction, toCamera, n, 4.0) * specMask;
	
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffuseColor * diffAmt * sceneLight;
		finalColor += specCol * light.color;
	}

	for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
		PointLight light = pointLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		vec3 toLight = light.position - fragPos; 
		vec3 lightDir = normalize(toLight);
		float distToLight = length(toLight); 
		float falloff = 1.0 - (distToLight / light.radius);

		float diffAmt = diffuse(lightDir, n) * falloff;
		float specAmt = specular(lightDir, toCamera, n, 4.0) * specMask * falloff;
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i){
		SpotLight light = spotLights[i];
		vec3 sceneLight = mix(light.color, envSample + light.color * 0.5, 0.5);	

		vec3 toLight = light.position - fragPos; 
		vec3 lightDir = normalize(toLight);
		float angle = dot(light.direction, -lightDir);
		float falloff = (angle > light.cutoff) ? 1.0 : 0.0;

		float diffAmt = diffuse(lightDir, n) * falloff;
		float specAmt = specular(lightDir, toCamera, n, 4.0) * specMask * falloff;

		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}

	FragColor = vec4(finalColor + ambientColor, 1);
}