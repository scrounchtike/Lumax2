
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
	renderModels3D();
	renderReflectiveSurfaces();
}

void RenderingEngineDX11::renderModels3D() {
	for (RenderingInstance instance : models3D)
		renderRenderingInstance(instance, instance.transform);
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

	for (RenderingInstance* instance : surface.reflectedInstances) {
		// Reflect the object's transform onto the reflective plane
		// Assuming for now it is y=0
		Vec3 translation = instance->transform->getTranslation();
		Vec3 newTranslation = translation;
		newTranslation.y *= -1;
		instance->transform->setTranslation(newTranslation);

		renderRenderingInstance(*instance, instance->transform);

		instance->transform->setTranslation(translation);
	}

	// Unbind the stencil buffer mode
	dx11->enableState("main");

	surface.renderingInstance.shader->bind(dx11);
	surface.renderingInstance.material->bind(dx11, surface.renderingInstance.shader);
	surface.renderingInstance.shader->bindUniforms(dx11, uniformBuffer);

	surface.renderingInstance.model->render();
}

RenderingInstance* RenderingEngineDX11::add3DModel(Model3DDX11* model, MaterialDX11* material, ShaderDX11* shader, Transform* transform) {
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
	Model3DDX11* planeModel = RenderingPlane::getModelWithNormals();

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

bool RenderingEngineDX11::checkModelWithShader(Model3DDX11* model, ShaderDX11* shader) {
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
	dx11->registerDepthStencilView(depthStencilDesc, "reflectiveSurfaces");

	// Static initialization of static shapes and models
	RenderingPlane::initialize(dx11);

	return true;
}

bool RenderingEngineDX11::cleanUp() {
	return true;
}