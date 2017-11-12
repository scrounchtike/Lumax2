
#ifndef RENDERING_ENGINE_DX11_HPP
#define RENDERING_ENGINE_DX11_HPP

#include <vector>

#include "../RAL/RenderingContextDX11.hpp"

#include "../rendering/Camera.hpp"
#include "../rendering/Model2D_DX11.hpp"
#include "../rendering/Model3D_DX11.hpp"
#include "../rendering/ShaderDX11.hpp"
#include "../rendering/TextureDX11.hpp"
#include "../rendering/MaterialDX11.hpp"

struct RenderingInstance {
	RenderingInstance() {}
	RenderingInstance(Model3DDX11* model, MaterialDX11* material, ShaderDX11* shader) : model(model), material(material), shader(shader), transform(new Transform()) {
	}
	RenderingInstance(Model3DDX11* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform) : model(model), material(material), shader(shader), transform(transform) {
	}
	Model3DDX11* model;
	MaterialDX11* material;
	ShaderDX11* shader;
	Transform* transform;

	ShaderDX11* underwaterShader = 0;
};

struct ReflectiveSurface {
	ReflectiveSurface() {}
	ReflectiveSurface(RenderingInstance instance, std::vector<RenderingInstance*> reflectedInstances) : renderingInstance(instance), reflectedInstances(reflectedInstances) {
	}
	RenderingInstance renderingInstance;
	std::vector<RenderingInstance*> reflectedInstances;
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

	// Basic render calls
	void renderRenderingInstance(RenderingInstance& instance, Transform* transform);
	void renderReflectiveSurface(ReflectiveSurface& surface);

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();

	// Engine interface
	RenderingInstance* add3DModel(Model3DDX11* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform = 0);
	void add2DModel();

	// Reflections
	// Can only be a plane for now
	ReflectiveSurface* addReflectiveSurface(std::vector<RenderingInstance*> reflectedInstances, MaterialDX11* material, ShaderDX11* shader, Transform* transform = 0);

	Camera* camera;
private:
	RenderingContextDX11* dx11;

	// Game objects
	std::vector<ReflectiveSurface> reflectiveSurfaces;

	std::vector<RenderingInstance> models3D;
	std::vector<Model2DDX11*> models2D;

	// Back-end tests and procedures
	bool checkModelWithShader(Model3DDX11* model, ShaderDX11* shader);
	bool checkMaterialWithShader(MaterialDX11* material, ShaderDX11* shader);
};

#endif