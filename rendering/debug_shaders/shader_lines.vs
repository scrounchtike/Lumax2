#version 330

layout (location = 0) in vec3 position;

uniform vec3 start;
uniform vec3 end;

uniform mat4 projection;
uniform mat4 view;

void main(){
	vec3 pos = start*(1.0 - position) + end*position;
	gl_Position = projection * view * vec4(pos, 1.0);
}
