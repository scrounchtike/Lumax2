
#ifndef RENDER_API_HPP
#define RENDER_API_HPP

class RenderAPI {
public:
	RenderAPI();
	RenderAPI(const RenderAPI&);
	~RenderAPI();

	bool initOpenGL();
	bool shutdownOpenGL();

	bool initDirectX11();
	bool shutdownDirectX11();
private:
};

#endif