#version 330

out vec4 FragColor;

uniform vec3 color;

void main(){
	FragColor = vec4(color, 0.5f);
}