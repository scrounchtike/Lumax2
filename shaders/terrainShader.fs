
#version 330

out vec4 FragColor;

uniform float LOD;

void main(){
	FragColor = vec4(1,0,0,1);
	if(LOD == 2)
		FragColor = vec4(0,1,0,1);
	if(LOD == 3)
		FragColor = vec4(0,0,1,1);
}
