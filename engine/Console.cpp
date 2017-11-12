
#include "Console.hpp"
#include "../RAL/Window.hpp"
#include "../OBJ_Loader.hpp"

#include "../main.hpp"

#include "../RAL/UsingGL.hpp"

Console::Console(){
	float vertices[12] = {0,1,0,0,1,0,1,0,1,1,0,1};
	input_box = new Model2D(vertices, 12);

	shader = new Shader("shaders/2DShaderColor");
	shader->addUniform("transform");
	shader->addUniform("color");

	text = new Text("wall 0.5f 9", -0.92f, -0.91f);
}

void Console::input(){
	if(wasKeyJustPressed(GLFW_KEY_BACKSPACE))
		text->backspace();
	if(wasKeyJustPressed(GLFW_KEY_ENTER)){
		//Process command
		std::string command = text->getText();
		issueCommand(command);
		//Clear text
		text->clear();
	}
	//unsigned char lastCharacter = getLastCharacter();
	unsigned char lastCharacter = 0;
	if(lastCharacter && lastCharacter != 'c')
		text->append(lastCharacter);
	if(wasKeyJustPressed(GLFW_KEY_C))
		text->append('c');
}

void Console::update(){
	
}

void Console::render(){
	shader->bind();
	float matrix[16] = {1.9f,0,0,-0.95f,
											0,1.0f/12.0f,0,-0.92f,
											0,0,1,0,
											0,0,0,1};
	shader->setUniformMatrix("transform", matrix);
	shader->setUniform3f("color", 0.5f, 0.5f, 0.5f);

	glDisable(GL_DEPTH_TEST);
	input_box->fullRender();
	text->fullRender();
	glEnable(GL_DEPTH_TEST);
}

void Console::issueCommand(std::string command){
	if(command.length() == 0)
		return;
	std::vector<std::string> commandParams = OBJ_Loader::split(command, ' ');
	std::cout << "issuing command : " << command << std::endl;
	Callback fp = commands[commandParams[0]];
	if(fp)
		fp(command);
	else
		std::cout << "Unknown command " << commandParams[0] << std::endl;
}

void Console::logCommand(std::string commandName, Callback callback){
	commands.insert(std::pair<std::string, Callback>(commandName, callback));
}

void Console::clear(){
	text->clear();
}
