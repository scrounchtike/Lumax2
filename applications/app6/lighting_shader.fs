#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;

out vec4 FragColor;

uniform sampler2D sample;
uniform vec3 eyePos;

//Lighting
vec3 ambient = vec3(0.3f, 0.3f, 0.3f);

const float reflectivity = 0.8f;
const int   dampFactor = 4;

const float exponent = 0.5f;
const float linear = 0.05f;
const float constant = 0.01f;

vec3 calcDirectionalLight(vec3 lightColor, vec3 normal, vec3 direction, vec3 toCameraVector, float intensity){
	float dot1 = dot(normal, direction);
	vec3 brightness = vec3(0,0,0);
	if(dot1 > 0){
		brightness += lightColor * dot1 * intensity;
		//vec3 reflected = reflect(-direction, normal);
		//float dot2 = dot(reflected, toCameraVector);
		//if(dot2 > 0){
			//float specularFactor = pow(dot2, dampFactor) * reflectivity;
			//brightness += lightColor * specularFactor * intensity;
		//}
	}
	return brightness;
}

vec3 calcPointLight(vec3 lightColor, vec3 normal, vec3 lightPos, vec3 toCameraVector, float intensity){
	vec3 direction = (position0 - lightPos);
	float distance = length(direction);
	vec3 light = normalize(direction);
	float dot1 = dot(normal, -light);
	vec3 brightness = vec3(0,0,0);
	if(dot1 > 0){
		vec3 factor = lightColor * dot1 * intensity;
		float attenuation = (exponent*distance*distance) + (linear*distance) + constant;
		brightness += factor / attenuation;
	}
	return brightness;
}

void main(){
	vec4 color = texture(sample, texCoord0);
	vec3 lighting = ambient;

	vec3 toCameraVector = eyePos - position0;
	toCameraVector = normalize(toCameraVector);

	//lighting += calcDirectionalLight(vec3(1,1,1), normal0, vec3(-1,-1,-1), toCameraVector, 0.6f);
	//lighting += calcPointLight(vec3(1,0,0), normal0, vec3(1.5f,1.5f,3.5f), toCameraVector, 0.8f);
	//lighting += calcPointLight(vec3(1,1,0), normal0, vec3(0,2,5), toCameraVector, 0.7f);

	lighting += calcPointLight(vec3(1,1,1), normal0, vec3(0,1.5f,-1.5f), toCameraVector, 0.8f);

	//lighting += calcPointLight(vec3(1,0,0), normal0, vec3(3, 1, -1), toCameraVector, 1.0f);

	lighting = clamp(lighting, 0, 1);
	FragColor = vec4(color.rgb * lighting, color.a);
}
