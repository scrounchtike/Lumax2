
#version 330

in vec2 texCoord0;

out vec4 FragColor;

uniform sampler2D sample;

void main(){
	FragColor = texture(sample, texCoord0);
	//FragColor = vec4(1,0,0,1);
}
