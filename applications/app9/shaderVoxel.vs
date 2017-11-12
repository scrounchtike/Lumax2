
#version 330

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in float AO;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec3 normal0;

out float AO0;

void main(){
	gl_Position = projection * view * transform * vec4(position, 1.0);

	AO0 = (AO + 2.0) / 5.0;

	//TODO: Transform the normals according to the transform uniform
	normal0 = normal;
}
