
#pragma once

#include "RenderingEngineDX11.hpp"

#include "../RL/RenderingPlane.hpp"

RenderingEngineDX11::RenderingEngineDX11() {

}

RenderingEngineDX11::RenderingEngineDX11(const RenderingEngineDX11& other){ }
RenderingEngineDX11::~RenderingEngineDX11() { }

void RenderingEngineDX11::input() {
	camera->input();
}

void RenderingEngineDX11::update() {
	camera->update();
}

void RenderingEngineDX11::render() {
	//renderModels3D();
	//renderTerrains();
	renderReflectiveSurfaces();
	renderModels3D();
}

void RenderingEngineDX11::renderModels3D() {
	for (RenderingInstance instance : models3D)
		renderRenderingInstance(instance, instance.transform);
}

void RenderingEngineDX11::renderTerrains() {
	for (TerrainInstance instance : terrains)
		renderTerrainInstance(instance, instance.transform);
}

void RenderingEngineDX11::renderReflectiveSurfaces() {
	for (ReflectiveSurface surface : reflectiveSurfaces)
		renderReflectiveSurface(surface);
}

void RenderingEngineDX11::renderRenderingInstance(RenderingInstance& instance, Transform* transform) {
	float* uniformBuffer = new float[instance.shader->getUniformBufferSize()];
	Mat4* matrixBuffer = (Mat4*)uniformBuffer;

	// Setting the matrix uniforms
	*matrixBuffer++ = camera->getProjection();
	*matrixBuffer++ = camera->getView();
	// TODO: Add a transform to each rendering instance
	*matrixBuffer++ = transform->getTransformation();

	instance.shader->bind(dx11);
	instance.material->bind(dx11, instance.shader);
	instance.shader->bindUniforms(dx11, uniformBuffer);

	instance.model->render();
}

void RenderingEngineDX11::renderTerrainInstance(TerrainInstance& instance, Transform* transform, Vec4 clipPlane) {
	float* uniformBuffer = new float[instance.clipShader->getUniformBufferSize()];
	Mat4* matrixBuffer = (Mat4*)uniformBuffer;

	*matrixBuffer++ = camera->getProjection();
	*matrixBuffer++ = camera->getView();
	*matrixBuffer++ = transform->getTransformation();
	Vec4* clippingPlane = (Vec4*)matrixBuffer;
	*clippingPlane = clipPlane;

	instance.clipShader->bind(dx11);
	instance.material->bind(dx11, instance.clipShader);
	instance.clipShader->bindUniforms(dx11, uniformBuffer);

	//instance.shader->bind(dx11);
	//instance.material->bind(dx11, instance.shader);
	//instance.shader->bindUniforms(dx11, uniformBuffer);

	instance.terrain->render();
}

void RenderingEngineDX11::renderReflectiveSurface(ReflectiveSurface& surface) {
	// Preparing all the uniform data for the water plane rendering
	float* uniformBuffer = new float[surface.renderingInstance.shader->getUniformBufferSize()];
	Mat4* matrixBuffer = (Mat4*)uniformBuffer;

	*matrixBuffer++ = camera->getProjection();
	*matrixBuffer++ = camera->getView();
	*matrixBuffer++ = surface.renderingInstance.transform->getTransformation();

	// Bind the stencil buffer for rendering reflective surfaces
	dx11->enableState("reflectiveSurfaces");

	surface.renderingInstance.shader->bind(dx11);
	surface.renderingInstance.material->bind(dx11, surface.renderingInstance.shader);
	surface.renderingInstance.shader->bindUniforms(dx11, uniformBuffer);

	surface.renderingInstance.model->render();

	// Render underwater terrain
	//renderTerrainInstance(terrains[0], terrains[0].transform, Vec4(0, -1, 0, 0));

	for (RenderingInstance* instance : surface.reflectedInstances) {
		// Reflect the object's transform onto the reflective plane
		// Assuming for now it is y=0
		Vec3 translation = instance->transform->getTranslation();
		Vec3 newTranslation = translation;
		newTranslation.y *= -1;
		instance->transform->setTranslation(newTranslation);
		instance->transform->setScale(Vec3(1, -1, 1));
		instance->transform->calculateTransformation();

		renderRenderingInstance(*instance, instance->transform);

		instance->transform->setScale(Vec3(1, 1, 1));
		instance->transform->setTranslation(translation);
		instance->transform->calculateTransformation();
	}

	// Render upper water terrain inverted
	//Vec3 translation = terrains[0].transform->getTranslation();
	//Vec3 newTranslation = translation;
	//newTranslation.y *= -1;
	//Vec3 scale = terrains[0].transform->getScale();

	//float size = 30.0f;
	//terrains[0].transform->initialize(Vec3(-size / 2.0f, -10, -size / 2.0f), Vec3(0, 0, 0), Vec3((1.0f / 128.0f)*size, -size / 1.5f, (1.0f / 128.0f)*size));
	//terrains[0].transform->setTranslation(newTranslation);
	//terrains[0].transform->setScale(scale.mul(Vec3(1,-1,1)));
	//terrains[0].transform->calculateTransformation();

	//renderTerrainInstance(terrains[0], terrains[0].transform);

	//terrains[0].transform->setScale(scale);
	//terrains[0].transform->setTranslation(translation);
	//terrains[0].transform->calculateTransformation();
	//terrains[0].transform->initialize(Vec3(-size / 2.0f, -10, -size / 2.0f), Vec3(0, 0, 0), Vec3((1.0f / 128.0f)*size, size / 1.5f, (1.0f / 128.0f)*size));

	// Unbind the stencil buffer mode
	dx11->enableState("main");

	surface.renderingInstance.shader->bind(dx11);
	surface.renderingInstance.material->bind(dx11, surface.renderingInstance.shader);
	surface.renderingInstance.shader->bindUniforms(dx11, uniformBuffer);

	surface.renderingInstance.model->render();

	// Render upper water terrain
	renderTerrainInstance(terrains[0], terrains[0].transform, Vec4(0, 1, 0, 0));
}

TerrainInstance* RenderingEngineDX11::addTerrain(TerrainDX11* terrain, MaterialDX11* material, ShaderDX11* shader, Transform* transform) {
	// Do a check to see if terrain and shader match attributes
	bool match = true;
	// Check to see if material and shader match
	match = true;
	if (!match) return nullptr;

	TerrainInstance instance;
	if (transform)
		instance = TerrainInstance(terrain, material, shader, transform);
	else
		instance = TerrainInstance(terrain, material, shader);
	instance.clipShader = clipShader;
	terrains.push_back(instance);

	return &terrains[terrains.size() - 1];
}

RenderingInstance* RenderingEngineDX11::add3DModel(Model3D* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform) {
	// Do a check to see if the models attributes match the shader attributes
	bool match = checkModelWithShader(model, shader);
	if (!match) return nullptr;
	// Check if the material uniforms match the material uniforms described in shader
	match = checkMaterialWithShader(material, shader);
	if (!match) return nullptr;

	RenderingInstance instance;
	if (transform)
		instance = RenderingInstance(model, material, shader, transform);
	else
		instance = RenderingInstance(model, material, shader);

	models3D.push_back(instance);

	return &models3D[models3D.size() - 1];
}

ReflectiveSurface* RenderingEngineDX11::addReflectiveSurface(std::vector<RenderingInstance*> reflectedInstances, MaterialDX11* material, ShaderDX11* shader, Transform* transform) {
	// TODO: Figure out what rendering plane the user wants based on the shader attributes
	// For now, assume a basic texture mapped plane
	Model3D* planeModel = RenderingPlane::getModelWithNormals();

	RenderingInstance instance;
	if(transform)
		instance = RenderingInstance(planeModel, material, shader, transform);
	else
		instance = RenderingInstance(planeModel, material, shader);

	ReflectiveSurface reflect = ReflectiveSurface();

	reflectiveSurfaces.push_back(ReflectiveSurface(instance, reflectedInstances));

	return &reflectiveSurfaces[reflectiveSurfaces.size() - 1];
}

void RenderingEngineDX11::add2DModel() {
	// TODO: Implement support for 2D models
}

bool RenderingEngineDX11::checkModelWithShader(Model3D* model, ShaderDX11* shader) {
	return true;
}

bool RenderingEngineDX11::checkMaterialWithShader(MaterialDX11* material, ShaderDX11* shader) {
	return true;
}

bool RenderingEngineDX11::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Camera initialization
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	// Create the stencil buffer state for reflection rendering
	DepthStencilDescription depthStencilDesc;
	depthStencilDesc.stencilBuffer.stencilEnable = true;
	depthStencilDesc.stencilBuffer.stencilFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.stencilBuffer.stencilFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.depthBuffer.depthEnable = true;
	depthStencilDesc.depthBuffer.depthFunc = D3D11_COMPARISON_ALWAYS;
	dx11->registerDepthStencilView(depthStencilDesc, "reflectiveSurfaces");

	// Static initialization of static shapes and models
	RenderingPlane::initialize(dx11);

	// Clip Shader
	clipShader = new ShaderDX11();
	clipShader->addAttribute("POSITION", 0, 3, false);
	clipShader->addAttribute("TEXCOORD", 0, 2, true);
	clipShader->addAttribute("NORMAL", 0, 3, true);
	clipShader->initialize(dx11, "applications/DX11/app8/clipShader", 52);

	return true;
}

bool RenderingEngineDX11::cleanUp() {
	return true;
}