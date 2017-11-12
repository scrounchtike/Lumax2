
#include "ShaderDX11.hpp"
#include "../RAL/Window.hpp"

#include <string>
#include <cassert>
#include <fstream>

ShaderDX11::ShaderDX11() {
	numAttributes = 0;
}

ShaderDX11::ShaderDX11(const ShaderDX11& other) { }
ShaderDX11::~ShaderDX11() { }

void ShaderDX11::bindMatrixUniforms(RenderingContextDX11* context, Mat4 transform, Mat4 view, Mat4 projection) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->getDeviceContext()->VSSetConstantBuffers(0, 1, &matrixBuffer);
	result = context->getDeviceContext()->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)) {
		OutputDebugString("Error: Failed to create mapped resource for constant buffers binding\n");
		assert(false);
	}

	struct MatrixBuffer {
		Mat4 transform;
		Mat4 view;
		Mat4 projection;
	};

	MatrixBuffer* dataPtr = (MatrixBuffer*)mappedResource.pData;
	dataPtr->transform = transform;
	dataPtr->view = view;
	dataPtr->projection = projection;

	context->getDeviceContext()->Unmap(matrixBuffer, 0);

	// Setting the constant buffer in the shader
	unsigned int bufferNumber = 0;
	context->getDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
}

void ShaderDX11::bindUniforms(RenderingContextDX11* dx11, float* uniforms) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Need to transpose matrices?
	// Apparently not..

	dx11->getDeviceContext()->VSSetConstantBuffers(0, 1, &uniformBuffer);
	result = dx11->getDeviceContext()->Map(uniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create mapped resource for constant buffers binding\n");
		assert(false);
	}

	void* dataPtr = mappedResource.pData;
	memcpy(dataPtr, uniforms, uniformsSize * sizeof(float));

	dx11->getDeviceContext()->Unmap(uniformBuffer, 0);

	// Setting the constant buffer in the shader
	unsigned int bufferNumber = 0;
	dx11->getDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &uniformBuffer);
}

void ShaderDX11::bindTexture(RenderingContextDX11* context, ID3D11ShaderResourceView** texture, int numTextures) const {
	context->getDeviceContext()->PSSetShaderResources(0, numTextures, texture);
}

void ShaderDX11::bind(RenderingContextDX11* context) const {
	context->getDeviceContext()->IASetInputLayout(layout);
	context->getDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	context->getDeviceContext()->PSSetShader(pixelShader, NULL, 0);

	// This is temporary: only needed for texturing
	context->getDeviceContext()->PSSetSamplers(0, 1, &samplerState);
}

bool ShaderDX11::initialize(RenderingContextDX11* context, LPCSTR fileName, int sizeUniforms) {
	this->uniformsSize = sizeUniforms;

	HRESULT result;

	std::string pixelFullPath = std::string(fileName) + ".ps";
	LPCSTR pixelShaderFile = pixelFullPath.c_str();
	std::string vertexFullPath = std::string(fileName) + ".vs";
	LPCSTR vertexShaderFile = vertexFullPath.c_str();

	ID3D10Blob* compiledVertex;
	ID3D10Blob* compiledPixel;

	int flags = 0;
#ifdef _DX11_DEBUG
	flags = D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob* compileErrors;
	result = D3DX11CompileFromFile(vertexShaderFile, NULL, NULL, "main", "vs_5_0", flags | D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &compiledVertex, &compileErrors, NULL);
	if (FAILED(result)) {
		if (compileErrors) {
			// Log an error statement to inform user to go check at error log
			OutputDebugString("Error: Failed to compile vertex shader. See shader-error.txt for more detail\n");
			outputShaderError(vertexShaderFile, compileErrors);
		}else
			OutputDebugString("Error: Could not find vertex shader\n");
		assert(false);
		return false;
	}

	result = D3DX11CompileFromFile(pixelShaderFile, NULL, NULL, "main", "ps_5_0", flags | D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &compiledPixel, &compileErrors, NULL);
	if (FAILED(result)) {
		if (compileErrors) {
			// Log an error statement to inform user to go check at error log
			OutputDebugString("Error: Failed to compile pixel shader. See shader-error.txt for more detail\n");
			outputShaderError(pixelShaderFile, compileErrors);
		}else
			OutputDebugString("Error: Could not find pixel shader\n");
		assert(false);
		return false;
	}

	// How to pass the device??
	result = context->getDevice()->CreateVertexShader(compiledVertex->GetBufferPointer(), compiledVertex->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(result)) {
		// Log error
		OutputDebugString("Error: Vertex Shader could not be created\n");
		assert(false);
		return false;
	}
	result = context->getDevice()->CreatePixelShader(compiledPixel->GetBufferPointer(), compiledPixel->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(result)) {
		// Log error
		OutputDebugString("Error: Pixel Shader could not be created\n");
		assert(false);
		return false;
	}

	/*
	* This will highly change. So far, the attributes are hardcoded to be POSITION and COLOR.
	* Need to implement initialization of different attributes depending on the shader.
	* Both attributes are interlaced, but this should not be a requirement either.
	*/
	// Create the attribute layout

	/*
	D3D11_INPUT_ELEMENT_DESC inputDesc[3];
	ZeroMemory(&inputDesc, sizeof(inputDesc));

	inputDesc[0].SemanticName = "POSITION";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].AlignedByteOffset = 0;
	inputDesc[0].InputSlot = 0;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0;

	inputDesc[1].SemanticName = "TEXCOORD";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlot = 0;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0;

	inputDesc[2].SemanticName = "NORMAL";
	inputDesc[2].SemanticIndex = 0;
	inputDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[2].InputSlot = 0;
	inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[2].InstanceDataStepRate = 0;
	*/

	// Assumes that we previously told the sahder about our attributes
	//attributes[0].SemanticName = "POSITION";
	//attributes[1].SemanticName = "TEXCOORD";
	//attributes[2].SemanticName = "NORMAL";
	result = context->getDevice()->CreateInputLayout(&attributes[0], numAttributes, compiledVertex->GetBufferPointer(),
		compiledVertex->GetBufferSize(), &layout);
	if (FAILED(result)) {
		// Log error
		OutputDebugString((std::string("Error: Linking of shader failed. Could not create input layout of shader ") + std::string(fileName) + std::string("\n")).c_str());
		assert(false);
		return false;
	}

	// Texture initialization
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = context->getDevice()->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result)) {
		OutputDebugString("Error: Could not create sampler state in shader initialization\n");
		assert(false);
		return false;
	}

	/*
	// Matrix uniforms initialization
	struct MatrixBuffer {
		Mat4 transform;
		Mat4 view;
		Mat4 projection;
	};

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	MatrixBuffer* buffer = new MatrixBuffer();
	buffer->transform = Mat4().initTranslation(Vec3(0,0.5f,0));
	buffer->view = Mat4().initIdentity();
	buffer->projection = Mat4().initIdentity();

	D3D11_SUBRESOURCE_DATA matrices;
	ZeroMemory(&matrices, sizeof(matrices));
	matrices.pSysMem = buffer;

	result = context->getDevice()->CreateBuffer(&matrixBufferDesc, &matrices, &matrixBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Could not create constant buffer for matrices\n");
		assert(false);
		return false;
	}
	*/

	// Uniforms initialization
	D3D11_BUFFER_DESC uniformDesc;
	ZeroMemory(&uniformDesc, sizeof(uniformDesc));
	uniformDesc.Usage = D3D11_USAGE_DYNAMIC;
	uniformDesc.ByteWidth = sizeof(float) * sizeUniforms;
	uniformDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uniformDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// TODO: Get rid of this
	float uniforms[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

	D3D11_SUBRESOURCE_DATA uniformData;
	uniformData.pSysMem = uniforms;
	uniformData.SysMemPitch = 0;
	uniformData.SysMemSlicePitch = 0;

	result = context->getDevice()->CreateBuffer(&uniformDesc, &uniformData, &uniformBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Could not create constant buffer for uniforms\n");
		assert(false);
		return false;
	}

	compiledVertex->Release();
	compiledVertex = 0;

	compiledPixel->Release();
	compiledPixel = 0;

	if (compileErrors) {
		compileErrors->Release();
		compileErrors = 0;
	}

	return true;
}

void ShaderDX11::addAttribute(LPCSTR name, int index, int size, bool appended) {
	D3D11_INPUT_ELEMENT_DESC* inputDesc = new D3D11_INPUT_ELEMENT_DESC();

	inputDesc->SemanticName = name;
	inputDesc->SemanticIndex = index;
	if (size == 1)
		inputDesc->Format = DXGI_FORMAT_R32_FLOAT;
	else if (size == 2)
		inputDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
	else if (size == 3)
		inputDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
	else
		inputDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (appended)
		inputDesc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	else
		inputDesc->AlignedByteOffset = 0;
	inputDesc->InputSlot = 0;
	inputDesc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc->InstanceDataStepRate = 0;

	attributes.push_back(*inputDesc);
	++numAttributes;
}

void ShaderDX11::addClipPlane() {
	D3D11_BUFFER_DESC clipPlaneBufferDesc;
}

bool ShaderDX11::cleanUp() {
	if (matrixBuffer) {
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout) {
		layout->Release();
		layout = 0;
	}
	if (pixelShader) {
		pixelShader->Release();
		pixelShader = 0;
	}
	if (vertexShader) {
		vertexShader->Release();
		vertexShader = 0;
	}
	return true;
}

void ShaderDX11::outputShaderError(LPCSTR shaderName, ID3D10Blob* compileErrors) {
	char* errorOutput = (char*)(compileErrors->GetBufferPointer());
	unsigned long bufferSize = compileErrors->GetBufferSize();

	std::ofstream out;
	out.open("shader-error.txt");
	for (int i = 0; i < bufferSize; ++i) {
		out << errorOutput[i];
	}
	out.close();

	compileErrors->Release();
	compileErrors = 0;
}