
#include "TiledWorld.hpp"

TiledWorld::TiledWorld(){
	//Resource stuff
	manager = new ResourceManager("res");
	Shader* terrainShader = manager->addShader("terrainShader", "shaders/terrainShader");
	terrainShader->addUniform("projection");
	terrainShader->addUniform("view");
	terrainShader->addUniform("transform");
	terrainShader->addUniform("LOD");
	
	thread1 = std::thread(&TiledWorld::loadChunk, this);
	
	CHUNK_WIDTH = 16;
	
	for(int i = 0; i < 9; ++i){
		for(int k = 0; k < 9; ++k){
			Vec3 chunkPosition = Vec3(-(9 * CHUNK_WIDTH) / 2.0f + (i * CHUNK_WIDTH), 0, -(9 * CHUNK_WIDTH) / 2.0f + (k * CHUNK_WIDTH));

			//Will have to change according to the render distance
			int boundMin = -72;
			int boundMax = 56;
			
			int LOD = 1;
			if(chunkPosition.x >= (boundMin + 48) && chunkPosition.x <= (boundMax - 48) && chunkPosition.z >= (boundMin + 48) && chunkPosition.z <= (boundMax - 48))
				LOD = 3;
			else if(chunkPosition.x >= (boundMin + 32) && chunkPosition.x <= (boundMax - 32) && chunkPosition.z >= (boundMin + 32) && chunkPosition.z <= (boundMax - 32))
				LOD = 2;
			
			Terrain* terrain = new Terrain(chunkPosition, LOD);
			terrain->state = Terrain::SEEN;
			terrains.push_back(terrain);
		}
	}
}

void TiledWorld::input(){
	
}

void TiledWorld::update(Vec3 playerPosition){
	int chunkX = fastFloor((playerPosition.x + CHUNK_WIDTH/2.0)/CHUNK_WIDTH);
	int chunkZ = fastFloor((playerPosition.z + CHUNK_WIDTH/2.0)/CHUNK_WIDTH);

	Vec2 chunkPos = Vec2(chunkX * CHUNK_WIDTH - (CHUNK_WIDTH / 2.0f), chunkZ * CHUNK_WIDTH - (CHUNK_WIDTH / 2.0f));
	
	int relX = chunkPos.x + 8;
	int relZ = chunkPos.y + 8;
	int xBoundMin = -72 + relX;
	int xBoundMax = 56 + relX;
	int zBoundMin = -72 + relZ;
	int zBoundMax = 56 + relZ;

	for(int i = 0; i < terrains.size(); ++i){
		Terrain* terrain = terrains[i];
		Vec2 pos = terrain->getPosition().xz();
		int previousLOD = terrain->getLOD();

		if(pos.x >= (xBoundMin + 48) && pos.x <= (xBoundMax - 48) && pos.y >= (zBoundMin + 48) && pos.y <= (zBoundMax - 48)){
			terrain->setLOD(3);
		}else if(pos.x >= (xBoundMin + 32) && pos.x <= (xBoundMax - 32) && pos.y >= (zBoundMin + 32) && pos.y <= (zBoundMax - 32)){
			terrain->setLOD(2);
		}else{
			terrain->setLOD(1);
		}

		if(pos.x < xBoundMin || pos.x > xBoundMax || pos.y < zBoundMin || pos.y > zBoundMax)
			terrain->state = Terrain::UNSEEN;
	}

	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			int newX = -(9 * CHUNK_WIDTH) / 2.0f + (i * CHUNK_WIDTH) + (chunkPos.x + CHUNK_WIDTH/2.0f);
			int newZ = -(9 * CHUNK_WIDTH) / 2.0f + (j * CHUNK_WIDTH) + (chunkPos.y + CHUNK_WIDTH/2.0f);
			bool loaded = false;
			for(Terrain* terrain : terrains){
				if(terrain->state == Terrain::SEEN || terrain->state == Terrain::LOADING || terrain->state == Terrain::ADD){
					if(terrain->getPosition().equals(Vec3(newX, 0, newZ))){
						loaded = true;
						break;
					}
				}
			}
			if(!loaded){
				Terrain* newTerrain = nullptr;
				for(Terrain* terrain : terrains){
					if(terrain->state == Terrain::UNSEEN){
						newTerrain = terrain;
						break;
					}
				}
				//Assert that we have enough chunks:
				if(newTerrain == nullptr){
					std::cerr << "No more terrains available, sorry." << std::endl;
					std::exit(1);
				}
				newTerrain->setPosition(Vec3(newX, 0, newZ));
				newTerrain->state = Terrain::LOADING;
			}
		}
	}
	
	//Needed for synchronization with threads:
	for(Terrain* terrain : terrains){
		if(terrain->state == Terrain::ADD){
			mutex.lock();
      
			//Reinit OpenGL!
			terrain->initGlRenderData();
      
			terrain->state = Terrain::SEEN;
			mutex.unlock();
		}
	}
}

void TiledWorld::loadChunk(){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		mutex.lock();
    
		for(Terrain* terrain : terrains){
			if(terrain->state != Terrain::LOADING)
				continue;
			
			mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			mutex.lock();
			
			clock_t clock1 = clock();
			terrain->regenerate();
			clock_t clock2 = clock();
      
			mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			mutex.lock();
      
			clock_t clock3 = clock();
			terrain->createMesh();
			clock_t clock4 = clock();
			clock_t time = (clock4 - clock3) + (clock2 - clock1);
			
			terrain->state = Terrain::ADD;
		}
    
		mutex.unlock();
	}
}

void TiledWorld::render(Camera* camera){
	glDisable(GL_CULL_FACE);
	
	Shader* terrainShader = manager->getShader("terrainShader");
	terrainShader->bind();
	terrainShader->setUniformMatrix("projection", camera->getProjection());
	terrainShader->setUniformMatrix("view", camera->getView());
	
	for(int i = 0; i < terrains.size(); ++i){
		Terrain* terrain = terrains[i];
		if(terrain->state != Terrain::SEEN)
			continue;

		terrainShader->setUniformMatrix("transform", Mat4().initTranslation(terrain->getPosition()));
		terrainShader->setUniform1f("LOD", terrain->getLOD());
		terrain->render();
	}
}
