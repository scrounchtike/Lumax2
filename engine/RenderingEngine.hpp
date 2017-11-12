
#ifndef RENDERING_ENGINE_HPP
#define RENDERING_ENGINE_HPP

#include <string>
#include <vector>

#include "../math.hpp"

#include "../rendering/DebugRenderer.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Mesh.hpp"

class RenderingEngine{
public:
	RenderingEngine(Camera* camera);

	void addPoint(Vec3 point, Vec3 color);
	void addPoint(Point& point);
	
	void addLine(Vec3 A, Vec3 B, Vec3 color);
	void addLine(Line& line);

	void addQuad(Vec3 A, Vec3 B, Vec3 color);
	void addQuadScale(Vec3 A, Vec3 scale, Vec3 color);
	void addQuad(Quad& quad);

	void input();
	void update();
	void render() const;

	void render3D() const;
	void render2D() const;

	void renderDebug() const;
	void renderMeshes() const;
private:
	//Camera
	Camera* camera;

	//TODO: Add a resource manager (for shaders mostly)

	std::vector<Mesh*> meshes;
	
};

#endif
