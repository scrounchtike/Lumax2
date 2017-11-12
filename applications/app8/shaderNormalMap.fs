
#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform sampler2D diffuse;
uniform sampler2D normalMap;

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
		vec3 reflected = reflect(-direction, normal);
		float dot2 = dot(reflected, toCameraVector);
		if(dot2 > 0){
			float specularFactor = pow(dot2, dampFactor) * reflectivity;
			brightness += lightColor * specularFactor * intensity;
		}
	}
	return brightness;
}

void main(){
  vec4 textureColor = texture(diffuse, texCoord0);

	vec3 toCameraVector = cameraPos - position0;
	toCameraVector = normalize(toCameraVector);

	vec3 normal = normalize(TBN * (255.0 / 128.0 * texture(normalMap, texCoord0).xyz - 1));
	vec3 brightness = ambient;
	brightness += calcDirectionalLight(vec3(1,1,1), normal, normalize(vec3(-1,-1,-1)), toCameraVector, 0.5);

	FragColor = textureColor * vec4(brightness, 1.0);
	//FragColor = vec4(1,0,0,1);
}
