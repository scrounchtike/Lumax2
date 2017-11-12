#version 330

out vec4 FragColor;

in vec3 color0;

void main(){
	FragColor = vec4(color0, 1.0);
}
