
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec2 texCoord0;
out vec3 normal0;
out vec3 position0;
out mat3 TBN;

void main(){
	vec4 worldPos = transform * vec4(position, 1.0);
	gl_Position = projection * view * worldPos;

	position0 = worldPos.xyz;
	texCoord0 = texCoord;

	mat4 normalTransform = transpose(inverse(transform));
	
	vec3 n = normalize((normalTransform * vec4(normal, 0.0)).xyz);
	
	//tbn Matrix:
	vec3 t = normalize((normalTransform * vec4(tangent, 0.0)).xyz);
	vec3 biTangent = cross(t, n);
	t = normalize(t - dot(t, n) * n);
	
	TBN = mat3(t, biTangent, n);
}
