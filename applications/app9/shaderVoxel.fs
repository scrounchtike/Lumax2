
#version 330

out vec4 FragColor;

in vec3 normal0;

in float AO0;

vec3 calcDirectionalLight(vec3 lightColor, vec3 normal, vec3 direction){
	normal = normalize(normal);
	direction = normalize(direction);
	float dot1 = dot(normal, direction);
	dot1 = clamp(dot1, 0.0, 1.0);
	vec3 brightness = lightColor * dot1;
	return brightness;
}

void main(){
	vec3 brightness = vec3(0.3f, 0.3f, 0.3f);
	brightness += calcDirectionalLight(vec3(1,1,1), normal0, vec3(-1,1,-1));
	brightness *= AO0;
	
	FragColor = vec4(brightness, 1.0) * vec4(0.5, 0.2, 0.2, 1.0);
}
