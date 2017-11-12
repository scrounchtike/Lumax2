
#include <iostream>

#include "RenderingEngine.hpp"

RenderingEngine::RenderingEngine(Camera* camera) : camera(camera){
	DebugRenderer::init();
}

void RenderingEngine::addPoint(Vec3 point, Vec3 color = Vec3(1,0,0)){
	DebugRenderer::addPoint(point, color);
}

void RenderingEngine::addPoint(Point &point){
	DebugRenderer::addPoint(point);
}

void RenderingEngine::addLine(Vec3 A, Vec3 B, Vec3 color = Vec3(1,0,0)){
  DebugRenderer::addLine(A, B, color);
}

void RenderingEngine::addLine(Line &line){
  DebugRenderer::addLine(line);
}

void RenderingEngine::addQuad(Vec3 A, Vec3 B, Vec3 color = Vec3(1,0,0)){
	Vec3 scale = B - A;
	DebugRenderer::addQuad(A, scale, color);
}

void RenderingEngine::addQuadScale(Vec3 A, Vec3 scale, Vec3 color = Vec3(1,0,0)){
  DebugRenderer::addQuad(A, scale, color);
}

void RenderingEngine::addQuad(Quad &quad){
	DebugRenderer::addQuad(quad);
}

void RenderingEngine::input(){
	
}

void RenderingEngine::update(){
  DebugRenderer::update();
}

void RenderingEngine::render() const{
	render3D();
	renderDebug();
	render2D();
}

void RenderingEngine::render3D() const{
	
}

void RenderingEngine::render2D() const{
	
}

void RenderingEngine::renderDebug() const{
	DebugRenderer::render(camera);
}

//Non-optimized function that renders every mesh individually without taking
//advantage of possible duplicate information (such as same model or same shader).
void RenderingEngine::renderMeshes() const{
	for(int i = 0; i < meshes.size(); ++i){
		meshes[i]->render();
	}
}
