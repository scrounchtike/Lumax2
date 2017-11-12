
#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <map>
#include <vector>

#include "Texture.hpp"
#include "../RL/Model2DDX11.hpp"
#include "../RL/Model2D.hpp"
#include "Shader.hpp"

// For DX11 text support
#include "ShaderDX11.hpp"
#include "TextureDX11.hpp"

struct Letter{
	int code;
	float posX;
	float posY;
	float width;
	float height;
	float xOffset;
	float yOffset;
	float xAdvance;
};

class Text{
public:
	Text(const std::string& text, float xPos, float yPos);

	void bindForRender() const;
	// Temporary
	void renderDX11(RenderingContextDX11* dx11) const;
	void render() const;
	void unbindForRender() const;

	void fullRender() const;

	void clear();
	void append(unsigned char character);
	void backspace();

	float xPos, yPos;

	const std::string& getText() const{ return text; }

	// Temporary
	static void initTextDX11(RenderingContextDX11* dx11);
	static void initText();
private:
	std::string text;
	
	static Letter* letters;
	static std::map<char, Letter*> characters;

	static Model2D* letter_model;
	static Model2DDX11* letterModelDX11;

	static Shader* shader;
	static ShaderDX11* shaderDX11;

	static Texture* font_image;
	static TextureDX11* fontImageDX11;
};

#endif
