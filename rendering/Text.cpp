
#include "Text.hpp"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>

#include "../RAL/Window.hpp"

#include "../resource_management/TextureLoader.hpp"
#include "../OBJ_Loader.hpp"

Letter*                 Text::letters;
std::map<char, Letter*> Text::characters;

Model2D* Text::letter_model;
Model2DDX11* Text::letterModelDX11;
Shader*  Text::shader;
ShaderDX11* Text::shaderDX11;
Texture* Text::font_image;
TextureDX11* Text::fontImageDX11;

Text::Text(const std::string& text, float xPos, float yPos) : text(text), xPos(xPos), yPos(yPos){
	
}

void Text::clear(){
	text.clear();
}

void Text::append(unsigned char character){
	std::stringstream ss;
	ss << character;
	text += ss.str();
}

void Text::backspace(){
	size_t length = text.length();
	if(length > 0){
		//Erase one character
		text.erase(length-1, 1);
	}
}

void Text::bindForRender() const{
	glDisable(GL_DEPTH_TEST);
	Text::font_image->bind();
	letter_model->bindForRender();
}

void Text::renderDX11(RenderingContextDX11* dx11) const {
	float width = 800;
	float height = 600;

	shaderDX11->bind(dx11);
	ID3D11ShaderResourceView* textures[1] = { fontImageDX11->getTexture() };
	shaderDX11->bindTexture(dx11, textures, 1);

	float fontSize = 6.0;
	float text_x = xPos;
	float text_y = yPos;

	// Create the uniform buffer
	struct UniformBuffer {
		float* matrix;
		float xPos;
		float yPos;
		float width;
		float height;
	};
	UniformBuffer uniforms = UniformBuffer();

	float xTranslation = 0;
	for (int i = 0; i < text.length(); ++i) {
		auto search = characters.find(text[i]);
		Letter* letter = search->second;
		//std::cout << "Printing letter " << letter->code << std::endl;

		//shader->setUniform1f("width", letter->width);
		//shader->setUniform1f("height", letter->height);
		uniforms.width = letter->width;
		uniforms.height = letter->height;

		float x = letter->posX;
		float y = letter->posY + letter->height;
		//std::cout << "POS = " << x << " " << y << std::endl;
		float xOffset = fontSize * letter->xOffset;
		float yOffset = fontSize * letter->yOffset;
		float xAdvance = fontSize * letter->xAdvance;

		//Default scale:
		float scale = 0.08f;
		float xScale = fontSize * scale * letter->width;
		float yScale = fontSize * scale * letter->height;

		float ar = width / height;
		float matrix[16] = { xScale / ar, 0,      0,     text_x + (xOffset + xTranslation) * scale / ar,
			0,         yScale, 0,     text_y + 0 * scale,
			0,         0,      scale, 0,
			0,         0,      0,     1 };
		uniforms.matrix = matrix;
		uniforms.xPos = xPos;
		uniforms.yPos = yPos;

		// Send the uniforms to the shader
		shaderDX11->bindUniforms(dx11, uniforms.matrix);

		//shaderDX11->setUniformMatrix("transform", matrix);

		//shaderDX11->setUniform1f("xPos", x);
		//shaderDX11->setUniform1f("yPos", y);
		letterModelDX11->render();
		xTranslation += xAdvance / 1.1;
	}
}

void Text::render() const{
	float width = 800;
	float height = 600;
	
	shader->bind();
	
	float fontSize = 6.0;
	float text_x = xPos;
	float text_y = yPos;

	float xTranslation = 0;
	for(int i = 0; i < text.length(); ++i){
		auto search = characters.find(text[i]);
		Letter* letter = search->second;
		//std::cout << "Printing letter " << letter->code << std::endl;
		
		shader->setUniform1f("width", letter->width);
		shader->setUniform1f("height", letter->height);
		
		float x = letter->posX;
		float y = letter->posY + letter->height;
		//std::cout << "POS = " << x << " " << y << std::endl;
		float xOffset = fontSize * letter->xOffset;
		float yOffset = fontSize * letter->yOffset;
		float xAdvance = fontSize * letter->xAdvance;

		//Default scale:
		float scale = 0.08f;
		float xScale = fontSize * scale * letter->width;
		float yScale = fontSize * scale * letter->height;

		float ar = width / height;
		float matrix[16] = {xScale/ar, 0,      0,     text_x + (xOffset + xTranslation) * scale / ar,
												0,         yScale, 0,     text_y + 0 * scale,
												0,         0,      scale, 0,
												0,         0,      0,     1};
		shader->setUniformMatrix("transform", matrix);
		
		shader->setUniform1f("xPos", x);
		shader->setUniform1f("yPos", y);
		letter_model->render();
		xTranslation += xAdvance/1.1;
	}
}

void Text::unbindForRender() const{
	letter_model->unbindForRender();
	glEnable(GL_DEPTH_TEST);
}

void Text::fullRender() const{
	bindForRender();
	render();
	unbindForRender();
}

void Text::initTextDX11(RenderingContextDX11* dx11) {
	fontImageDX11 = new TextureDX11();
	fontImageDX11->initialize(dx11, "res/fonts/font.dds");

	std::ifstream file;
	file.open("res/fonts/font.fnt");
	if (!file.is_open()) {
		std::cerr << "Error while loading font: file could not be opened." << std::endl;
		return;
	}
	if (!file.good()) {
		std::cerr << "Error while loading font: file could not be read." << std::endl;
		return;
	}

	std::string line;
	int charCount = 0;
	int charIndex = 0;
	while (file.good()) {
		std::getline(file, line);
		if (!line.substr(0, 5).compare("chars")) {
			std::vector<std::string> data = OBJ_Loader::split(line, '=');
			charCount = std::stoi(data[1]);
			letters = new Letter[charCount];
			std::cout << "char count = " << charCount << std::endl;
		}
		else if (!line.substr(0, 5).compare("char ")) {
			std::vector<std::string> data;
			std::string currentInfo = "";
			for (int i = 5; i < line.length(); ++i) {
				if (line[i] != ' ')
					currentInfo += line[i];
				else if (line[i] == ' ' && currentInfo.length() > 0) {
					data.push_back(OBJ_Loader::split(currentInfo, '=')[1]);
					currentInfo = "";
				}
			}

			Letter letter;
			letter.code = std::stoi(data[0]);
			letter.posX = std::stoi(data[1]) / 512.0;
			letter.posY = std::stoi(data[2]) / 512.0;
			letter.width = std::stoi(data[3]) / 512.0;
			letter.height = std::stoi(data[4]) / 512.0;
			letter.xOffset = std::stoi(data[5]) / 512.0;
			letter.yOffset = std::stoi(data[6]) / 512.0;
			letter.xAdvance = std::stoi(data[7]) / 512.0;
			letters[charIndex] = letter;
			++charIndex;
		}
	}

	for (int i = 0; i < charCount; ++i) {
		int code = letters[i].code;
		char ascii = (char)code;
		characters.insert(std::pair<char, Letter*>(ascii, &letters[i]));
	}

	float vertices[8] = { 0,0,0,1,1,1,1,0 };
	int indices[6] = { 0,1,2,2,3,0 };

	//letterModelDX11 = DX11System::createModel2D(vertices, 8, indices, 6);

	shaderDX11 = new ShaderDX11();
	shaderDX11->addAttribute("POSITION", 0, 3, false);
	shaderDX11->addAttribute("TEXCOORD", 0, 2, true);
	shaderDX11->initialize(dx11, "applications/DX11/app4/shaderText", 20);
}

void Text::initText(){
	font_image = new Texture("res/fonts/font.png");
	
	std::ifstream file;
	file.open("res/fonts/font.fnt");
	if(!file.is_open()){
		std::cerr << "Error while loading font: file could not be opened." << std::endl;
		return;
	}
	if(!file.good()){
		std::cerr << "Error while loading font: file could not be read." << std::endl;
		return;
	}
	
	std::string line;
	int charCount = 0;
	int charIndex = 0;
	while(file.good()){
		std::getline(file, line);
		if(!line.substr(0,5).compare("chars")){
			std::vector<std::string> data = OBJ_Loader::split(line, '=');
			charCount = std::stoi(data[1]);
			letters = new Letter[charCount];
			std::cout << "char count = " << charCount << std::endl;
		}else if(!line.substr(0,5).compare("char ")){
			std::vector<std::string> data;
			std::string currentInfo = "";
			for(int i = 5; i < line.length(); ++i){
				if(line[i] != ' ')
					currentInfo += line[i];
				else if(line[i] == ' ' && currentInfo.length() > 0){
					data.push_back(OBJ_Loader::split(currentInfo, '=')[1]);
					currentInfo = "";
				}
			}

			Letter letter;
			letter.code = std::stoi(data[0]);
			letter.posX = std::stoi(data[1])/512.0;
			letter.posY = std::stoi(data[2])/512.0;
			letter.width = std::stoi(data[3])/512.0;
			letter.height = std::stoi(data[4])/512.0;
			letter.xOffset = std::stoi(data[5])/512.0;
			letter.yOffset = std::stoi(data[6])/512.0;
			letter.xAdvance = std::stoi(data[7])/512.0;
			letters[charIndex] = letter;
			++charIndex;
		}
	}

	for(int i = 0; i < charCount; ++i){
		int code = letters[i].code;
		char ascii = (char)code;
		characters.insert(std::pair<char, Letter*>(ascii, &letters[i]));
	}

	float vertices[8] = {0,0,0,1,1,1,1,0};
	int indices[6] = {0,3,2,2,1,0};

	letter_model = new Model2D(vertices, 8, indices, 6);

	shader = new Shader("shaders/shaderText");
	shader->addUniform("transform");
	shader->addUniform("xPos");
	shader->addUniform("yPos");
	shader->addUniform("width");
	shader->addUniform("height");
}
