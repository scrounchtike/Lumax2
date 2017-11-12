
#ifndef SHADER_DX11_HPP
#define SHADER_DX11_HPP

#include <vector>

#include "../RAL/RenderingContextDX11.hpp"

#include "../math.hpp"

class ShaderDX11 {
public:
	ShaderDX11();
	ShaderDX11(const ShaderDX11&);
	~ShaderDX11();

	int getUniformBufferSize() { return uniformsSize; }

	// Temporary for testing
	void bindMatrixUniforms(RenderingContextDX11*, Mat4, Mat4, Mat4);

	void bindUniforms(RenderingContextDX11* dx11, float* uniforms);
	void bindTexture(RenderingContextDX11*, ID3D11ShaderResourceView**, int numTextures) const;
	void bind(RenderingContextDX11*) const;

	bool initialize(RenderingContextDX11*, LPCSTR, int);
	bool cleanUp();

	void addAttribute(LPCSTR name, int index, int size, bool appended);
	void addClipPlane();
private:
	// Warning; This gets deleted, do not use this anywhere except in initialization and before its deletion
	std::vector<D3D11_INPUT_ELEMENT_DESC> attributes;
	int numAttributes;
	int uniformsSize;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	ID3D11SamplerState* samplerState;

	ID3D11Buffer* uniformBuffer;
	// Temporary
	ID3D11Buffer* matrixBuffer;

	static void outputShaderError(LPCSTR, ID3D10Blob*);
};

#endif