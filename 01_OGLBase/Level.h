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
	ProgramObject		surfaceProgram;		//Parametrikus fel�let shader
	VertexArrayObject	surfaceVAO;			//Parametrikus fel�let VAO-ja
	IndexBuffer			surfaceIndices;		//Parametrikus fel�let indext�mbje
	ArrayBuffer			surfaceVBO;			//Parametrikus fel�let VBO-ja
	Texture2D			grassTexture;		//Fel�letre haszn�lt text�ra
	Texture2D			hillTexture;		//Fel�letre haszn�lt text�ra

	//A p�lya bels� adatai:
	std::vector<std::vector<Tile*>> levelTilesData;	//sorindex, oszlopindex, magass�g, van-e rajta torony, megl�togattuk-e m�r
	int levelSize;									//n*n-es level_sizem�ret

	//A p�lya fel�let�nek adatai:
	int tileResolution;		//n*n-es m�ret
	int levelResolution;	//Parametrikus fel�let felbont�sa (n�gyzetekben).
	float tileRadius;		//Egy mez� sz�less�g�nek a fele (a szinusz domb sugara).
	int scale;				//Mennyivel nagy�tod a kezdeti uv fel�letet.

	//Kijel�lt mez� adatai:
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