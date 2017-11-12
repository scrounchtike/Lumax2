
#include "DX11App8.hpp"

DX11App8::DX11App8() {

}

DX11App8::DX11App8(const DX11App8& other) { }
DX11App8::~DX11App8() { }

void DX11App8::input() {
	camera->input();
}

void DX11App8::update() {
	camera->update();
}

void DX11App8::render() {
	struct Shader3DUniform {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
		Vec4 clipPlane;
	};
	Shader3DUniform shaderBuffer;

	struct ReflectShaderUniform {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
	};
	ReflectShaderUniform reflectBuffer;

	ID3D11ShaderResourceView* waterTexture[1] = { texture->getTexture() };

	//2. Render the cube
	clipShader->bind(dx11);

	shaderBuffer.projection = camera->getProjection();
	shaderBuffer.view = camera->getView();
	shaderBuffer.transform = Mat4().initTranslation(0, 4, 6);
	shaderBuffer.clipPlane = Vec4(0, 0, 0, 0);

	clipShader->bindUniforms(dx11, (float*)(&shaderBuffer));
	clipShader->bindTexture(dx11, waterTexture, 1);

	cube->render();

	// Enable Stencil Test
	//dx11->enableStencilState();

	//1. Render the water patch
	reflectShader->bind(dx11);

	reflectBuffer.projection = camera->getProjection();
	reflectBuffer.view = camera->getView();
	reflectBuffer.transform = Mat4().initScale(15, 1, 15);

	reflectShader->bindUniforms(dx11, (float*)(&reflectBuffer));
	reflectShader->bindTexture(dx11, waterTexture, 1);

	waterPatch->render();

	//3. Render the inversed cube
	clipShader->bind(dx11);

	Mat4 inversedCubeTransform = Mat4().initTranslation(0, -4, 6).mul(Mat4().initScale(1, 1, 1));
	shaderBuffer.transform = inversedCubeTransform;

	clipShader->bindUniforms(dx11, (float*)(&shaderBuffer));
	clipShader->bindTexture(dx11, waterTexture, 1);

	cube->render();

	// Disable stencil test
	//dx11->disableStencilState();

	//3. Render the water patch
	reflectShader->bind(dx11);

	reflectBuffer.projection = camera->getProjection();
	reflectBuffer.view = camera->getView();
	reflectBuffer.transform = Mat4().initScale(15, 1, 15);

	reflectShader->bindUniforms(dx11, (float*)(&reflectBuffer));
	reflectShader->bindTexture(dx11, waterTexture, 1);

	waterPatch->render();

	// TODO: Is there a way to not have to rerender the water plane?
}

void DX11App8::render2() {
	// Data for the scene
	Vec3 cubePosition = Vec3(1, 6, 6);
	Vec4 clipPlaneNULL = Vec4(0, 0, 0, 0);
	float waterLevel = 0;
	Vec4 clipPlaneWater = Vec4(0, -1, 0, waterLevel);

	struct ClipUniformBuffer {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
		Vec4 clipPlane;
	};
	ClipUniformBuffer clipBuffer;

	struct WaterUniformBuffer {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
	};
	WaterUniformBuffer waterBuffer;

	struct Uniform2DBuffer {
		Mat4 transform;
	};
	Uniform2DBuffer buffer2D;

	clipBuffer.view = camera->getView();

	// 1. Rendering the cube to an offscreen buffer
	clipShader->bind(dx11);

	// Using the offscreen buffer
	//renderTextureBuffer->setRenderTarget(dx11, dx11->getDepthStencilView());
	//renderTextureBuffer->clearRenderTarget(dx11, dx11->getDepthStencilView(), 0.0f, 0.0f, 0.7f, 1.0f);

	// Setting uniforms
	clipBuffer.projection = Mat4().initProjectionDX11(70.0f, 600.0 / 800.0, 0.1f, 1000.f);

	// Move the camera to under the water
	float distance = camera->getPosition().y - waterLevel;
	camera->getPosition().y = camera->getPosition().y - (2 * distance);
	// Invert the pitch
	Vec3 right = camera->getRight();
	Vec3 forward = camera->getForward();
	Vec3 up = camera->getUp();
	camera->getForward().y = -forward.y;
	camera->getUp() = cross(camera->getForward(), right);
	camera->getUp().normalize();
	camera->update();
	
	clipBuffer.view = camera->getView();

	camera->getPosition().y = camera->getPosition().y + (2 * distance);
	// Invert the pitch back to the previous one
	camera->getForward() = forward;
	camera->getUp() = up;
	camera->update();

	clipBuffer.transform = Mat4().initTranslation(cubePosition);
	clipBuffer.clipPlane = clipPlaneWater;
	clipShader->bindUniforms(dx11, (float*)(&clipBuffer));
	ID3D11ShaderResourceView* textures[1] = { texture->getTexture() };
	clipShader->bindTexture(dx11, textures, 1);

	// Render the cube
	cube->render();

	// Go back to using the window frambuffer
	//dx11->setRenderTarget();
	//dx11->setViewport();
	//float color[4] = { 0.5f,0.5f,0.5f,1.0f };
	//dx11->clearBuffer(color);

	//2. Render the cube to the window
	clipBuffer.projection = camera->getProjection();
	clipBuffer.view = camera->getView();
	clipBuffer.clipPlane = clipPlaneNULL;
	clipShader->bindUniforms(dx11, (float*)(&clipBuffer));

	cube->render();

	//3. Render the water patch
	waterShader->bind(dx11);

	waterBuffer.projection = camera->getProjection();
	waterBuffer.view = camera->getView();
	waterBuffer.transform = Mat4().initScale(15, 1, 15);
	waterShader->bindUniforms(dx11, (float*)(&waterBuffer));
	ID3D11ShaderResourceView* waterTexture[1] = { renderTextureBuffer->getShaderResourceView() };
	waterShader->bindTexture(dx11, waterTexture, 1);

	// Render the water patch
	waterPatch->render();

	shader2D->bind(dx11);

	// 2D shader uniforms
	//buffer2D.transform = Mat4().initTranslation(0, 0, 0);
	//shader2D->bindUniforms(dx11, (float*)(&buffer2D));
	//shader2D->bindTexture(dx11, waterTexture, 1);
	
	//plane->render();

}

bool DX11App8::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Initializing the camera
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	shader2D = new ShaderDX11();
	shader2D->addAttribute("POSITION", 0, 2, false);
	shader2D->addAttribute("TEXCOORD", 0, 2, true);
	shader2D->initialize(dx11, "applications/DX11/app8/shader2D", 16);

	// Initializing 3D clip shader
	clipShader = new ShaderDX11();
	clipShader->addAttribute("POSITION", 0, 3, false);
	clipShader->addAttribute("TEXCOORD", 0, 2, true);
	clipShader->addAttribute("NORMAL", 0, 3, true);
	clipShader->initialize(dx11, "applications/DX11/app8/clipShader", 52);

	// Initializing water shader
	waterShader = new ShaderDX11();
	waterShader->addAttribute("POSITION", 0, 3, false);
	waterShader->addAttribute("TEXCOORD", 0, 2, true);
	waterShader->initialize(dx11, "applications/DX11/app8/waterShader", 48);

	// Initializing the 3D shader
	shader3D = new ShaderDX11();
	shader3D->addAttribute("POSITION", 0, 3, false);
	shader3D->addAttribute("TEXCOORD", 0, 2, true);
	shader3D->addAttribute("NORMAL", 0, 3, true);
	shader3D->initialize(dx11, "applications/DX11/app3/shader3D", 48);

	reflectShader = new ShaderDX11();
	reflectShader->addAttribute("POSITION", 0, 3, false);
	reflectShader->addAttribute("TEXCOORD", 0, 2, true);
	reflectShader->addAttribute("NORMAL", 0, 3, true);
	reflectShader->initialize(dx11, "applications/DX11/app8/reflectiveSurfaceShader", 48);

	// Creating cube model
	cube = new Model3DDX11(dx11);
	cube->initialize("res/models/cube.obj");

	// Creating water patch model
	float vertices[12] = { -1,0,-1,-1,0,1,1,0,1,1,0,-1 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	float normals[12] = { 0,1,0,0,1,0,0,1,0,0,1,0 };
	int indices[6] = { 0,1,2,2,3,0 };
	waterPatch = new Model3DDX11(dx11);
	waterPatch->initialize(vertices, 4, indices, 6, texCoords, normals);

	float verticesPlane[8] = { -1,0,-1,1,0,1,0,0 };
	float texCoordsPlane[8] = { 0,0,0,1,1,1,1,0 };
	int indicesPlane[6] = { 0,1,2,2,3,0 };
	plane = new Model2DDX11(dx11);
	plane->initialize(verticesPlane, 4, indicesPlane, 6, texCoordsPlane);

	// Creating the cube texture
	texture = new TextureDX11();
	texture->initialize(dx11, "res/textures_dds/test.dds");

	// Creating the render target texture
	renderTextureBuffer = new RenderTextureBuffer();
	renderTextureBuffer->initialize(dx11, 800, 600);

	return true;
}

bool DX11App8::cleanUp() {
	return true;
}