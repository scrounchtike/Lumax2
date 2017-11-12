
#include "DX11App10.hpp"

DX11App10::DX11App10() {

}

DX11App10::DX11App10(const DX11App10& other) { }
DX11App10::~DX11App10() { }

void DX11App10::input() {
	camera->input();
}

void DX11App10::update() {
	camera->update();
}

void DX11App10::render() {
	struct UniformBuffer {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
	};
	UniformBuffer shaderBuffer;

	shaderBuffer.projection = camera->getProjection();
	shaderBuffer.view = camera->getView();
	shaderBuffer.transform = Mat4().initIdentity();

	shader->bind(dx11);
	shader->bindUniforms(dx11, (float*)(&shaderBuffer));
	ID3D11ShaderResourceView* textures[2] = { dirt->getTexture(), normalMap->getTexture() };

	shader->bindTexture(dx11, textures, 2);

	terrain->render();
}

bool DX11App10::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Camera initialization
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	// Shader initialization
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	shader->addAttribute("NORMAL", 0, 3, true);
	shader->addAttribute("TANGENT", 0, 3, true);
	shader->addAttribute("BITANGENT", 0, 3, true);
	shader->initialize(dx11, "applications/DX11/app10/shaderTerrain", 48);

	// Terrain initialization
	terrain = new TerrainDX11();
	bool result = terrain->initialize(dx11, "res/terrains/terrain2.txt");
	assert(result);

	// Texture initialization
	test = new TextureDX11();
	test->initialize(dx11, "res/textures_dds/terrainTest.dds");

	dirt = new TextureDX11();
	dirt->initialize(dx11, "res/textures_dds/dirt1.dds");

	normalMap = new TextureDX11();
	normalMap->initialize(dx11, "res/textures_dds/dirt1normal.dds");

	return true;
}

bool DX11App10::cleanUp() {
	return true;
}