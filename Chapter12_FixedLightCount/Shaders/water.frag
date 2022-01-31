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
in vec2 uv0;
in vec2 uv1;
in mat3 TBN;

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
	//vec3 toLight = lightPos - fragPos;
	//float distToLight = length(toLight);
	
	//vec3 l = normalize(toLight);

	vec3 n0 = texture(tex0, uv0).rgb;
	n0 = (n0 * 2.0 - 1.0);   

	
	vec3 n1 = texture(tex0, uv1).rgb;
	n1 = (n1 * 2.0 - 1.0);   

	vec3 n = normalize(TBN * (n0 + n1)); 

	vec3 toCamera = normalize(cameraPosition - fragPos);
	vec3 envSample = texture(tex1, reflect(-toCamera, n)).xyz;
	vec3 finalColor = vec3(0,0,0);

	for (int i = 0; i < NUM_DIR_LIGHTS; ++i) {
		DirectionalLight light = directionalLights[i];

		float diffAmt = diffuse(light.direction, n);
		float specAmt = specular(light.direction, toCamera, n, 512);
		
		vec3 specCol = light.color * specAmt;
		vec3 diffCol = envSample * light.color * diffAmt;

		finalColor += diffCol;
		finalColor += specCol;
	}

	for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
		PointLight light = pointLights[i];

		vec3 toLight = light.position - fragPos; 
		vec3 lightDir = normalize(toLight);
		float distToLight = length(toLight); 
		float falloff = 1.0 - (distToLight / light.radius);

		float diffAmt = diffuse(lightDir, n) * falloff;
		float specAmt = specular(lightDir, toCamera, n, 4.0) * falloff;
	
		vec3 specCol = light.color * specAmt;
		vec3 diffCol = envSample * light.color * diffAmt;

		finalColor += diffCol;
		finalColor += specCol;
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) {
		SpotLight light = spotLights[i];

		vec3 toLight = light.position - fragPos; 
		vec3 lightDir = normalize(toLight);
		float angle = dot(light.direction, -lightDir);
		float falloff = (angle > light.cutoff) ? 1.0 : 0.0;

		float diffAmt = diffuse(lightDir, n) * falloff;
		float specAmt = specular(lightDir, toCamera, n, 4.0) * falloff;

		vec3 specCol = light.color * specAmt;
		vec3 diffCol = envSample * light.color * diffAmt;

		finalColor += diffCol;
		finalColor += specCol;
	}

	FragColor = vec4(finalColor + ambientColor, 1.0);
}