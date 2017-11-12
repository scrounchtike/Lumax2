
#include "DX11App9.hpp"

DX11App9::DX11App9() {

}

DX11App9::DX11App9(const DX11App9& other) { }
DX11App9::~DX11App9() { }

void DX11App9::input() {
	camera->input();
}

void DX11App9::update() {
	camera->update();
}

void DX11App9::render() {
	// Texture manipulation
	renderMipmap();

	// Actual rendering
	struct UniformBuffer {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
	};
	UniformBuffer shaderBuffer;

	ID3D11ShaderResourceView* textureResource[1] = { texture->getTexture() };

	shaderBuffer.projection = camera->getProjection();
	shaderBuffer.view = camera->getView();
	shaderBuffer.transform = Mat4().initScale(30, 1, 30);

	shader3D->bind(dx11);
	shader3D->bindUniforms(dx11, (float*)(&shaderBuffer));
	shader3D->bindTexture(dx11, textureResource, 1);

	planeModel->render();
}

void DX11App9::renderMipmap() {
	
}

void DX11App9::renderAnisotropic() {

}

bool DX11App9::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Camera initialization
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	// 3D shader initialization
	shader3D = new ShaderDX11();
	shader3D->addAttribute("POSITION", 0, 3, false);
	shader3D->addAttribute("TEXCOORD", 0, 2, true);
	shader3D->initialize(dx11, "applications/DX11/app9/shaderTexture", 48);

	// Texture initialization
	texture = new TextureDX11();
	texture->initialize(dx11, "res/textures_dds/test.dds");

	// Plane model initialization
	float vertices[12] = { -1,0,-1,-1,0,1,1,0,1,1,0,-1 };
	float texCoords[8] = {0, 0, 0, 1, 1, 1, 1, 0};
	int indices[6] = { 0,1,2,2,3,0 };
	planeModel = new Model3DDX11(dx11);
	planeModel->initialize(vertices, 4, indices, 6, texCoords);

	return true;
}

bool DX11App9::cleanUp() {
	return true;
}