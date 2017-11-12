#version 330

in vec2 texCoord0;
in vec3 normal0;

out vec4 FragColor;

uniform sampler2D sample;

void main(){
	FragColor = texture(sample, texCoord0);
}
