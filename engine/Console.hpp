
#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>
#include <map>

#include "../rendering/Text.hpp"
#include "../rendering/Model2D.hpp"
#include "../rendering/Shader.hpp"

typedef void (*Callback)(std::string);

class Console{
public:
	Console();

	void input();
	void update();
	void render();

	void clear();

	void issueCommand(std::string command);
	void logCommand(std::string commandName, Callback callback);
private:

	Shader* shader;
	Model2D* input_box;
	Text* text;

	std::map<std::string, Callback> commands;
};

#endif
