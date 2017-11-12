#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 color0;

uniform vec2 translation;
uniform float z;

void main(){
	gl_Position = vec4(translation + position, z, 1.0);
	
	color0 = color;
}



