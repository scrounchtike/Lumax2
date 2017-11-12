
#ifndef RENDERING_ENGINE_DX11_HPP
#define RENDERING_ENGINE_DX11_HPP

#include <vector>

#include "../RAL/RenderingContextDX11.hpp"

#include "../rendering/Camera.hpp"
#include "../RL/Model2DDX11.hpp"

#include "../RL/Model3D.hpp"

#include "../rendering/ShaderDX11.hpp"
#include "../rendering/TextureDX11.hpp"
#include "../rendering/MaterialDX11.hpp"
#include "../terrain/TerrainDX11.hpp"

struct RenderingInstance {
	RenderingInstance() {}
	RenderingInstance(Model3D* model, MaterialDX11* material, ShaderDX11* shader) : model(model), material(material), shader(shader), transform(new Transform()) {
	}
	RenderingInstance(Model3D* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform) : model(model), material(material), shader(shader), transform(transform) {
	}
	Model3D* model;
	MaterialDX11* material;
	ShaderDX11* shader;
	Transform* transform;

	ShaderDX11* underwaterShader = 0;
};

struct TerrainInstance {
	TerrainInstance() {}
	TerrainInstance(TerrainDX11* terrain, MaterialDX11* material, ShaderDX11* shader) : terrain(terrain), material(material), shader(shader), transform(new Transform()) {
	}
	TerrainInstance(TerrainDX11* terrain, MaterialDX11* material, ShaderDX11* shader, Transform* transform) : terrain(terrain), material(material), shader(shader), transform(transform) {
	}
	TerrainDX11* terrain;
	MaterialDX11* material;
	ShaderDX11* shader;
	Transform* transform;

	ShaderDX11* clipShader;

	ShaderDX11* underwaterShader = 0;
};

struct ReflectiveSurface {
	ReflectiveSurface() {}
	ReflectiveSurface(RenderingInstance instance, std::vector<RenderingInstance*> reflectedInstances) : renderingInstance(instance), reflectedInstances(reflectedInstances) {
	}
	RenderingInstance renderingInstance;

	std::vector<RenderingInstance*> reflectedInstances;
	// TODO: Add reflected terrains
};

class RenderingEngineDX11 {
public:
	RenderingEngineDX11();
	RenderingEngineDX11(const RenderingEngineDX11&);
	~RenderingEngineDX11();

	void input();
	void update();
	void render();

	void renderModels3D();
	void renderReflectiveSurfaces();
	void renderTerrains();

	// Basic render calls
	void renderRenderingInstance(RenderingInstance& instance, Transform* transform);
	void renderTerrainInstance(TerrainInstance& instance, Transform* transform, Vec4 clippingPlane = Vec4(0, 0, 0, 0));
	void renderReflectiveSurface(ReflectiveSurface& surface);

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();

	// Engine interface
	TerrainInstance* addTerrain(TerrainDX11* terrain, MaterialDX11* material, ShaderDX11* shader, Transform* transform = 0);
	RenderingInstance* add3DModel(Model3D* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform = 0);
	void add2DModel();

	// Reflections
	// Can only be a plane for now
	ReflectiveSurface* addReflectiveSurface(std::vector<RenderingInstance*> reflectedInstances, MaterialDX11* material, ShaderDX11* shader, Transform* transform = 0);

	Camera* camera;
private:
	RenderingContextDX11* dx11;

	// Game objects
	std::vector<ReflectiveSurface> reflectiveSurfaces;

	std::vector<TerrainInstance> terrains;
	std::vector<RenderingInstance> models3D;
	std::vector<Model2DDX11*> models2D;

	// Back-end tests and procedures
	bool checkModelWithShader(Model3D* model, ShaderDX11* shader);
	bool checkMaterialWithShader(MaterialDX11* material, ShaderDX11* shader);

	// Test shaders
	ShaderDX11* clipShader;
};

#endif