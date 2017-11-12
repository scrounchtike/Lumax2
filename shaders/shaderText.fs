#version 330

out vec4 FragColor;

in vec2 texCoord0;

uniform sampler2D sample;

void main(){
	FragColor = texture(sample, texCoord0);
}