
#include <stdlib.h>

#include "bake_light.hpp"

const int BakeLight::LIGHTMAP_SIZE = 32;

BakeLight::BakeLight(){
	
}

//TODO: Add way to indicate light position and color / + multiple lights at once?
//Create a light information struct
GLuint BakeLight::bake(Vec3 A, Vec3 B, Vec3 C, Vec3 normal){
	const int SIZE = 16;
	unsigned char data[SIZE * SIZE * 3];
	float step = 1.0f / (float)SIZE;

	Vec3 light_pos = Vec3(0,0,-1);
	Vec3 light_color = Vec3(1,1,1);
	Vec3 pos;
	float s_dist, t_dist;
	Vec3 s_vec = C - A;
	//Vec3 s_vec = Vec3(1,0,0) - Vec3(0,0,0);
	s_dist = sqrt(dot(s_vec, s_vec));
	Vec3 t_vec = B - A;
	//Vec3 t_vec = Vec3(0,1,0) - Vec3(0,0,0);
	t_dist = sqrt(dot(t_vec, t_vec));
	float s, t;
	s = t = 0.0f;
	for(int i = 0; i < SIZE; ++i){
		for(int j = 0; j < SIZE; ++j){
			pos.x = s_dist * s;
			pos.y = t_dist * t;
			pos.z = 0;

			Triangle triangle(A, B, C);
			Vec2 texel_pos = Vec2(s, t);
			Vec3 pixel_pos = texCoordToPos(triangle, Vec2(0,0.5f), Vec2(1,1), Vec2(1,0), texel_pos);
			
			//Vec3 brightness = Vec3(0,0,0);

			//Transform light pos to texCoord coordinates
			Vec3 light_p = light_pos;
			
			//Perform transformations on texel
			//none

			float exponent = 0.5f;
			float linear = 0.05f;
			float constant = 0.01f;
			
			float intensity = 0.8f;
			Vec3 light = pixel_pos - light_p;
			float distance = light.getLength();
			light = light.normalize();
			float dot1 = dot(-light, normal);
			Vec3 brightness = Vec3(0.0f,0.0f,0.0f);
			if(dot1 > 0){
				Vec3 factor = light_color * dot1 * intensity;
				//attenuation
				float attenuation = (exponent*distance*distance)+(linear*distance)+constant;
				brightness += factor / attenuation;
			}
			
			brightness += Vec3(0.3f,0.3f,0.3f);
			
			if(brightness.x > 1.0f)
				brightness.x = 1.0f;
			if(brightness.y > 1.0f)
				brightness.y = 1.0f;
			if(brightness.z > 1.0f)
				brightness.z = 1.0f;
			
			//std::cout << "pos = " << pos.x << " " << pos.y << std::endl;
			//d = dot(pos,pos) * 0.8f;
			//if(d < 1.0f)
			//d = 1.0f;
			//tmp = 1.0f / d;
			
			data[(i * SIZE + j) * 3 + 0] = (unsigned char)(255.0f * brightness.x);
			data[(i * SIZE + j) * 3 + 1] = (unsigned char)(255.0f * brightness.y);
			data[(i * SIZE + j) * 3 + 2] = (unsigned char)(255.0f * brightness.z);

			s += step;
		}
		s = 0.0f;
		t += step;
	}
	
	
	//Create the baked light texture
	GLuint baked_triangle;
	glGenTextures(1, &baked_triangle);
	glBindTexture(GL_TEXTURE_2D, baked_triangle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return baked_triangle;
}

GLuint BakeLight::bake_model(RawModel model, std::vector<float>& bakedTexCoords){
	int numTriangles = model.numVertices/3;
	std::cout << "Num triangles = " << numTriangles << std::endl;

	//A 512x512 texture should be fine
	GLuint finalTexture;
	glGenTextures(1, &finalTexture);
	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//Temporary 16x16 texture storage
	unsigned char data[LIGHTMAP_SIZE * LIGHTMAP_SIZE * 3];

	int numTextures = 16;
	int subsetSize = 32;
	for(int i = 0; i < numTriangles; ++i){
		Vec3 A = Vec3(model.vertices[i*9+0], model.vertices[i*9+1], model.vertices[i*9+2]);
		Vec3 B = Vec3(model.vertices[i*9+3], model.vertices[i*9+4], model.vertices[i*9+5]);
		Vec3 C = Vec3(model.vertices[i*9+6], model.vertices[i*9+7], model.vertices[i*9+8]);
		Vec2 T1 = Vec2(model.texCoords[i*6+0], model.texCoords[i*6+1]);
		Vec2 T2 = Vec2(model.texCoords[i*6+2], model.texCoords[i*6+3]);
		Vec2 T3 = Vec2(model.texCoords[i*6+4], model.texCoords[i*6+5]);
		Vec3 normal = Vec3(model.normals[i*9+0], model.normals[i*9+1], model.normals[i*9+2]);
		bake_triangle(A, B, C, T1, T2, T3, normal, data);
		
		int xOffset = (i % numTextures) * subsetSize;
		int yOffset = (i / numTextures) * subsetSize;
		
		float xTex = (float)xOffset/512.0;
		float yTex = (float)yOffset/512.0;
		bakedTexCoords.push_back(xTex + T1.x/16.0);
		bakedTexCoords.push_back(yTex + T1.y/16.0);
		bakedTexCoords.push_back(xTex + T2.x/16.0);
		bakedTexCoords.push_back(yTex + T2.y/16.0);
		bakedTexCoords.push_back(xTex + T3.x/16.0);
		bakedTexCoords.push_back(yTex + T3.y/16.0);

		/*
		unsigned char R_ = (unsigned char)((float)rand() / (float)RAND_MAX * 255.0f);
		unsigned char G_ = (unsigned char)((float)rand() / (float)RAND_MAX * 255.0f);
		unsigned char B_ = (unsigned char)((float)rand() / (float)RAND_MAX * 255.0f);
		for(int y = 0; y < LIGHTMAP_SIZE; ++y){
			for(int x = 0; x < LIGHTMAP_SIZE; ++x){
				//Get a random color:
				data[(y * LIGHTMAP_SIZE + x) * 3 + 0] = R_;
				data[(y * LIGHTMAP_SIZE + x) * 3 + 1] = G_;
				data[(y * LIGHTMAP_SIZE + x) * 3 + 2] = B_;
				//std::cout << ((float)rand() / (float)RAND_MAX * 255.0f) << std::endl;
			}
		}
		*/
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, subsetSize, subsetSize, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	
	return finalTexture;
}

void BakeLight::bake_triangle(Vec3 A, Vec3 B, Vec3 C, Vec2 T1, Vec2 T2, Vec2 T3, Vec3 normal, unsigned char* data){
	float step = 1.0f / (float)LIGHTMAP_SIZE;

	//TODO: Create a light description struct for multiple lights
	Vec3 light_pos = Vec3(0,1.5f,-1.5f);
	Vec3 light_color = Vec3(1,1,1);
	
	float s_dist, t_dist;
	Vec3 s_vec = C - A;
	s_dist = sqrt(dot(s_vec, s_vec));
	Vec3 t_vec = B - A;
	t_dist = sqrt(dot(t_vec, t_vec));
	float s, t;
	s = t = 0.0f;
	for(int i = 0; i < LIGHTMAP_SIZE; ++i){
		for(int j = 0; j < LIGHTMAP_SIZE; ++j){
			Triangle triangle(A, B, C);
			Vec3 pixel_pos = texCoordToPos(triangle, T1, T2, T3, Vec2(s+step/2.0,t+step/2.0));

			//Perform transformations
			//NONE

			//Light description struct
			float exponent = 0.5f;
			float linear = 0.05f;
			float constant = 0.01f;
			float intensity = 0.8f;
			
			Vec3 light = pixel_pos - light_pos;
			float distance = light.getLength();
			light = light.normalize();
			float dot1 = dot(-light, normal);
			Vec3 brightness = Vec3(0.0f,0.0f,0.0f);
			if(dot1 > 0){
				Vec3 factor = light_color * dot1 * intensity;
				//attenuation
				float attenuation = (exponent*distance*distance)+(linear*distance)+constant;
				brightness += factor / attenuation;
			}
			
			brightness += Vec3(0.3f,0.3f,0.3f);

			//Clamping
			if(brightness.x > 1.0f)
				brightness.x = 1.0f;
			if(brightness.y > 1.0f)
				brightness.y = 1.0f;
			if(brightness.z > 1.0f)
				brightness.z = 1.0f;
			
			data[(i * LIGHTMAP_SIZE + j) * 3 + 0] = (unsigned char)(255.0f * brightness.x);
			data[(i * LIGHTMAP_SIZE + j) * 3 + 1] = (unsigned char)(255.0f * brightness.y);
			data[(i * LIGHTMAP_SIZE + j) * 3 + 2] = (unsigned char)(255.0f * brightness.z);

			s += step;
		}
		s = 0.0f;
		t += step;
	}
}

Vec3 BakeLight::texCoordToPos(Triangle triangle, Vec2 T1, Vec2 T2, Vec2 T3, Vec2 P){
	float i;
	float s;
	float t;
	Vec3 r;
	Vec3 V1 = triangle.A;
	Vec3 V2 = triangle.B;
	Vec3 V3 = triangle.C;
	i = 1.0f / ((T2.x - T1.x) * (T3.y - T1.y) - (T2.y - T1.y) * (T3.x - T1.x));
	s = i * ( (T3.y - T1.y) * (P.x - T1.x) - (T3.x - T1.x) * (P.y - T1.y));
	t = i * (-(T2.y - T1.y) * (P.x - T1.x) + (T2.x - T1.x) * (P.y - T1.y));
	r.x = V1.x + s * (V2.x - V1.x) + t * (V3.x - V1.x);
	r.y = V1.y + s * (V2.y - V1.y) + t * (V3.y - V1.y);
	r.z = V1.z + s * (V2.z - V1.z) + t * (V3.z - V1.z);
	return r;
}
