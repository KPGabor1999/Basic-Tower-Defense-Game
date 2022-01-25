#pragma once

#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

//STL
#include "Tile.h"
#include <vector>
#include <tuple>
#include <queue>

class Level
{
private:
	ProgramObject		surfaceProgram;		//Parametrikus felület shader
	VertexArrayObject	surfaceVAO;			//Parametrikus felület VAO-ja
	IndexBuffer			surfaceIndices;		//Parametrikus felület indextömbje
	ArrayBuffer			surfaceVBO;			//Parametrikus felület VBO-ja
	Texture2D			grassTexture;		//Felületre használt textúra
	Texture2D			hillTexture;		//Felületre használt textúra

	//A pálya belsõ adatai:
	std::vector<std::vector<Tile*>> levelTilesData;	//sorindex, oszlopindex, magasság, van-e rajta torony, meglátogattuk-e már
	int levelSize;									//n*n-es level_sizeméret

	//A pálya felületének adatai:
	int tileResolution;		//n*n-es méret
	int levelResolution;	//Parametrikus felület felbontása (négyzetekben).
	float tileRadius;		//Egy mezõ szélességének a fele (a szinusz domb sugara).
	int scale;				//Mennyivel nagyítod a kezdeti uv felületet.

	//Kijelölt mezõ adatai:
	bool buildModeOn = false;
	Tile* selectedTile;
	bool canBuildHere;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureUV;
	};

public:
	Level();
	Level(int size);

	float  getTileRadius();
	int  getScale();
	float getTileHeight(int lineIndex, int columnIndex);
	int  getLevelSize();
	void setLevelSize(int newSize);
	int  getTileResolution();
	float getTileWidth();
	std::vector<Tile*> getLineData(int lineIndex);
	Tile* getTileData(int lineIndex, int columnIndex);
	bool selectedTileIsAHill(int lineIndex, int columnIndex);
	bool hasTower(int lineIndex, int columnIndex);
	bool hasRoadblock(int lineIndex, int columnIndex);
	bool isEntranceOrExit(Tile* selectedTile);
	glm::vec3 getTileCenter(Tile* currentTile);
	void installTower(int lineIndex, int columnIndex);
	void installRoadblock(int lineIndex, int columnIndex);
	void setBuildModeOn(bool buildModeOn);
	void setSelectedTile(int lineIndex, int columnIndex);
	void setCanBuildHere(bool conditionPassed);

	void render(glm::mat4& viewMatrix);

private:
	void generate();
	void generate(int size);
	void initializeGameBoard();
	void BFS();
	bool hasUpperNeighbor(Tile* currentTile);
	Tile* getUpperNeighbor(Tile* currentTile);
	bool hasLowerNeighbor(Tile* currentTile);
	Tile* getLowerNeighbor(Tile* currentTile);
	bool hasLeftNeighbor(Tile* currentTile);
	Tile* getLeftNeighbor(Tile* currentTile);
	bool hasRightNeighbor(Tile* currentTile);
	Tile* getRightNeighbor(Tile* currentTile);
	void visit(Tile* tile);
	void unVisit(Tile* tile);
	void unVisitTiles();
	void initializeLevelTopology();
};