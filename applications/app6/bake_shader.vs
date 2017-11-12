#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 bakedTexCoord;

out vec2 texCoord0;
out vec3 normal0;
out vec3 position0;
out float indexf;
out vec2 bakedTexCoord0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main(){
	vec4 worldPos = transform * vec4(position, 1.0);
	gl_Position = projection * view * worldPos;

	highp int index = int(gl_VertexID);
	index /= 3;
	indexf = float(index);

	position0 = worldPos.xyz;
	texCoord0 = texCoord;
	normal0 = normal;
	bakedTexCoord0 = bakedTexCoord;
}
