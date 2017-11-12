
#include "TextureDX11.hpp"

TextureDX11::TextureDX11() {
	texture = 0;
}

TextureDX11::TextureDX11(const TextureDX11& other) { }
TextureDX11::~TextureDX11() { }

void TextureDX11::bind() const {

}

bool TextureDX11::initialize(RenderingContextDX11* dx11, LPCSTR fileName) {
	HRESULT result;

	D3DX11_IMAGE_LOAD_INFO textureInfo;
	textureInfo.Width = D3DX11_DEFAULT;
	textureInfo.Height = D3DX11_DEFAULT;
	textureInfo.Depth = D3DX11_DEFAULT;
	textureInfo.FirstMipLevel = D3DX11_DEFAULT;
	textureInfo.MipLevels = 0;
	//textureInfo.Usage = D3DX11_DEFAULT;
	textureInfo.BindFlags = D3DX11_DEFAULT;
	textureInfo.CpuAccessFlags = D3DX11_DEFAULT;
	textureInfo.MiscFlags = D3DX11_DEFAULT;
	//textureInfo.Format = D3DX11_DEFAULT;
	textureInfo.Filter = D3DX11_DEFAULT;
	textureInfo.MipFilter = D3DX11_DEFAULT;

	result = D3DX11CreateShaderResourceViewFromFile(dx11->getDevice(), fileName, &textureInfo, NULL, &texture, NULL);
	if (FAILED(result)) {
		OutputDebugString("Error: Texture could not be loaded from file\n");
		// For now, again, crash if we do not have the texture.
		// But missing a texture should not be a life-death scenario for the engine.
		assert(false);
		return false;
	}

	return true;
}

bool TextureDX11::cleanUp() {
	if (texture) {
		texture->Release();
		texture = 0;
	}

	return true;
}