
#include "DebugRenderer.hpp"

std::vector<Point> DebugRenderer::points;
std::vector<Line>  DebugRenderer::lines;
std::vector<Quad>  DebugRenderer::quads;

Shader* DebugRenderer::shaderPoints;
GLuint  DebugRenderer::vaoID_points;
GLuint  DebugRenderer::vboID_points;
GLuint  DebugRenderer::iboID_points;

Shader* DebugRenderer::shaderLines;
GLuint  DebugRenderer::vaoID_lines;
GLuint  DebugRenderer::vboID_lines;

Shader* DebugRenderer::shaderQuads;
GLuint  DebugRenderer::vaoID_quads;
GLuint  DebugRenderer::vboID_quads;

void DebugRenderer::init(){
	//Points
	glPointSize(10.0f);
	shaderPoints = new Shader("rendering/debug_shaders/shader_points");
	shaderPoints->addUniform("color");
	shaderPoints->addUniform("transform");
	shaderPoints->addUniform("projection");
	shaderPoints->addUniform("view");
	
	float cube_vertices[24] = {-0.5f, 0.5f,-0.5f,
												 0.5f, 0.5f,-0.5f,
												 0.5f,-0.5f,-0.5f,
												-0.5f,-0.5f,-0.5f,
												-0.5f, 0.5f, 0.5f,
												 0.5f, 0.5f, 0.5f,
												 0.5f,-0.5f, 0.5f,
												-0.5f,-0.5f, 0.5f};

	int cube_indices[36] = {3,2,1,1,0,3,   //Front face
										 0,1,5,5,4,0,   //Top face
										 2,6,5,5,1,2,   //Right face
										 7,3,0,0,4,7,   //Left face
										 6,7,4,4,5,6,   //Back face
										 7,6,2,2,3,7};  //Bottom face

	glGenBuffers(1, &vboID_points);
	glBindBuffer(GL_ARRAY_BUFFER, vboID_points);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*24, cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &iboID_points);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID_points);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*36, cube_indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID_points);
	glBindVertexArray(vaoID_points);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Lines
	shaderLines = new Shader("rendering/debug_shaders/shader_lines");
	shaderLines->addUniform("start");
	shaderLines->addUniform("end");
	shaderLines->addUniform("color");
	shaderLines->addUniform("projection");
	shaderLines->addUniform("view");

	float line[6] = {0,0,0,1,1,1};

	glGenBuffers(1, &vboID_lines);
	glBindBuffer(GL_ARRAY_BUFFER, vboID_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, line, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID_lines);
	glBindVertexArray(vaoID_lines);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Quads
	shaderQuads = new Shader("rendering/debug_shaders/shader_quads");
	shaderQuads->addUniform("transform");
	shaderQuads->addUniform("projection");
	shaderQuads->addUniform("view");
	shaderQuads->addUniform("color");

	float quad[18] = {-1,-1,0,1,-1,0,1,1,0,1,1,0,-1,1,0,-1,-1,0};

	glGenBuffers(1, &vboID_quads);
	glBindBuffer(GL_ARRAY_BUFFER, vboID_quads);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*18, quad, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID_quads);
	glBindVertexArray(vaoID_quads);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DebugRenderer::cleanUp(){
	
}

void DebugRenderer::update(){
	
}

void DebugRenderer::render(Camera* camera){
	shaderPoints->bind();
	passCameraUniforms(shaderPoints, camera);
	renderPoints();

	shaderLines->bind();
	passCameraUniforms(shaderLines, camera);
	renderLines();

	shaderQuads->bind();
	passCameraUniforms(shaderQuads, camera);
	renderQuads();
}

void DebugRenderer::passCameraUniforms(Shader* shader, Camera* camera){
	shader->setUniformMatrix("projection", camera->getProjection().getHeadPointer());
	shader->setUniformMatrix("view", camera->getView().getHeadPointer());
}

void DebugRenderer::renderPoints(){
	glBindVertexArray(vaoID_points);
	glEnableVertexAttribArray(0);
	for(int i = 0; i < points.size(); ++i){
		Point point = points[i];
		//shaderPoints->setUniform3f("point", point.position.x, point.position.y, point.position.z);
		shaderPoints->setUniform3f("color", point.color.x, point.color.y, point.color.z);
		Mat4 transform = Mat4().initTranslation(point.position.x,point.position.y,point.position.z);
		transform.mul(Mat4().initScale(0.01f, 0.01f, 0.01f));
		shaderPoints->setUniformMatrix("transform", transform.getHeadPointer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID_points);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void DebugRenderer::renderLines(){
	glBindVertexArray(vaoID_lines);
	glEnableVertexAttribArray(0);
	for(int i = 0; i < lines.size(); ++i){
		Line line = lines[i];
		shaderLines->setUniform3f("start", line.start.x, line.start.y, line.start.z);
		shaderLines->setUniform3f("end", line.end.x, line.end.y, line.end.z);
		shaderLines->setUniform3f("color", line.color.x, line.color.y, line.color.z);
		glDrawArrays(GL_LINES, 0, 2);
	}
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void DebugRenderer::renderQuads(){
	glBindVertexArray(vaoID_quads);
	glEnableVertexAttribArray(0);
	for(int i = 0; i < quads.size(); ++i){
		Quad quad = quads[i];
		shaderQuads->setUniform3f("color", quad.color.x, quad.color.y, quad.color.z);
		Mat4 transform = Mat4().initTranslation(quad.position.x, quad.position.y, quad.position.z);
		transform.mul(Mat4().initScale(quad.scale.x, quad.scale.y, quad.scale.z));
		shaderQuads->setUniformMatrix("transform", transform.getHeadPointer());
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
