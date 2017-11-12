
#ifndef DX11_APP_4
#define DX11_APP_4

#include "DX11App.hpp"

#include "../../rendering/Text.hpp"

class DX11App4 : public DX11App {
public:
	DX11App4();
	DX11App4(const DX11App4&);
	~DX11App4();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* shaderText;
	TextureDX11* font;
	Text* text;
};

#endif