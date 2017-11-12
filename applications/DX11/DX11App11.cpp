
#include "DX11App11.hpp"

#include "../../OBJ_Loader.hpp"

#include "../../terrain/TerrainDX11.hpp"

DX11App11::DX11App11() {

}

DX11App11::DX11App11(const DX11App11& other) { }
DX11App11::~DX11App11() { }

void DX11App11::input() {
	engine.input();
}

void DX11App11::update() {
	engine.update();
}

void DX11App11::render() {
	engine.render();
}

bool DX11App11::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Engine initialization
	engine = RenderingEngineDX11();
	engine.initialize(dx11);

	// Shader initialization
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	//shader->addAttribute("NORMAL", 0, 3, true);
	shader->initialize(dx11, "applications/DX11/default_shaders/shader3Dtexture", 48);

	ShaderDX11* waterShader = new ShaderDX11();
	waterShader->addAttribute("POSITION", 0, 3, false);
	waterShader->addAttribute("TEXCOORD", 0, 2, true);
	waterShader->addAttribute("NORMAL", 0, 3, true);
	waterShader->initialize(dx11, "applications/DX11/app8/reflectiveSurfaceShader", 48);

	// Texture initialization
	test = new TextureDX11();
	test->initialize(dx11, "res/textures_dds/test.dds");

	// Model initialization
	RawModel raw = OBJ_Loader::loadModel("res/models/cube.obj");
	model = new Model3D(raw.vertices, raw.numVertices, raw.texCoords);

	// Material initialization
	material = new MaterialDX11();
	material->initialize(test);

	// Adding the cube model
	Transform* cubeTransform = new Transform();
	cubeTransform->initializeTranslationOnly(Vec3(0, 5, 3));
	RenderingInstance* cubeRI = engine.add3DModel(model, material, shader, cubeTransform);

	// Adding the reflective surface with the cube reflected inside of it
	Transform* planeTransform = new Transform();
	planeTransform->initializeScaleOnly(Vec3(15, 1, 15));
	std::vector<RenderingInstance*> reflectedInstances;
	reflectedInstances.push_back(cubeRI);
	engine.addReflectiveSurface(reflectedInstances, material, waterShader, planeTransform);

	// Terrain tests
	TerrainDX11* terrain1 = new TerrainDX11();
	terrain1->initialize(dx11, "res/terrains/terrain3.txt");
	Transform* terrainTransform = new Transform();
	float size = 30.0f;
	terrainTransform->initialize(Vec3(-size/2.0f, -10, -size/2.0f), Vec3(0,0,0), Vec3((1.0f/128.0f)*size, size/1.5f, (1.0f/128.0f)*size));
	TextureDX11* terrainGrass = new TextureDX11();
	terrainGrass->initialize(dx11, "res/textures_dds/grass.dds");
	MaterialDX11* terrainMaterial = new MaterialDX11();
	terrainMaterial->initialize(terrainGrass);
	ShaderDX11* terrainShader = new ShaderDX11();
	terrainShader->addAttribute("POSITION", 0, 3, false);
	terrainShader->addAttribute("TEXCOORD", 0, 2, true);
	terrainShader->addAttribute("NORMAL", 0, 3, true);
	terrainShader->initialize(dx11, "applications/DX11/app11/terrainShader", 48);
	engine.addTerrain(terrain1, terrainMaterial, terrainShader, terrainTransform);

	return true;
}

bool DX11App11::cleanUp() {
	return true;
}