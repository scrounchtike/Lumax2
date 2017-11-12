
#include "App4.hpp"

#include "../main.hpp"

Shader* App4::shader;
GLuint  App4::vaoID;
GLuint  App4::vboID;
GLuint  App4::iboID;

Camera App4::camera(nullptr, Vec3(0,0,0));

void App4::init(){
	camera.setWindow(getStaticWindow());
	//App4::projection[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	//Init shader
	shader = new Shader("applications/app4/shader");
	shader->addUniform("transform");
	shader->addUniform("projection");
	shader->addUniform("view");

	//Cube data
	float vertices[24] = {-0.5f, 0.5f,-0.5f,
												 0.5f, 0.5f,-0.5f,
												 0.5f,-0.5f,-0.5f,
												-0.5f,-0.5f,-0.5f,
												-0.5f, 0.5f, 0.5f,
												 0.5f, 0.5f, 0.5f,
												 0.5f,-0.5f, 0.5f,
												-0.5f,-0.5f, 0.5f};

	int indices[36] = {3,2,1,1,0,3,   //Front face
										 0,1,5,5,4,0,   //Top face
										 2,6,5,5,1,2,   //Right face
										 7,3,0,0,4,7,   //Left face
										 6,7,4,4,5,6,   //Back face
										 7,6,2,2,3,7};  //Bottom face

	//Init cube buffers
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*24, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*36, indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void App4::input(){
	camera.input();
}

float theta = 0;

void App4::update(){
	theta = 0.01f;
	camera.update();
}

float App4::temp = 0;

void App4::render(){
	App4::temp += 0.01f;
	
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	
	shader->bind();

	//Set transform uniform
	float translation[16] = {1,0,0,0,
	                    0,1,0,0,
								      0,0,1,5.0f,
	                    0,0,0,1};
	float rotation[16] = {(float)cos((double)App4::temp), 0, (float)(-sin((double)App4::temp)), 0,
	                      0,         1,  0,         0,
												(float)sin((double)App4::temp), 0,  (float)cos((double)App4::temp), 0,
												0,         0,  0,         1};
	float* transform = mul(translation, rotation);
	//float* transform = translation;

	shader->setUniformMatrix("projection", camera.getProjection().getHeadPointer());
	shader->setUniformMatrix("transform", transform);
	shader->setUniformMatrix("view", camera.getView().getHeadPointer());
	
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void App4::cleanUp(){
 	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &iboID);
	glDeleteVertexArrays(1, &vaoID);

	delete shader;
}

float* App4::mul(float *mat1, float *mat2){
	float* matrix = new float[16];
	for(int j = 0; j < 4; ++j){
		for(int i = 0; i < 4; ++i){
			matrix[j*4+i] = mat1[j*4+0]*mat2[i] + mat1[j*4+1]*mat2[4+i] + mat1[j*4+2]*mat2[8+i] + mat1[j*4+3]*mat2[12+i];
		}
	}
	return matrix;
}
