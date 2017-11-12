#version 330

layout (location = 0) in vec2 position;

out vec2 texCoord0;

uniform mat4 transform;
uniform float xPos;
uniform float yPos;
uniform float width;
uniform float height;

void main(){
	gl_Position = transform * vec4(position, 0.0, 1.0);

	float xTex = xPos + position.x * width;
	float yTex = yPos - position.y * height;
	texCoord0 = vec2(xTex, yTex);
}
