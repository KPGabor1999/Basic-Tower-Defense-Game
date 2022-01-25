#pragma once

// C++ includes
#include <memory>

// GLEW
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

// mesh
#include "includes/ObjParser_OGL3.h"

//FBO:
#include "FrameBufferObject.h"

//Pálya, bejárata, kijárata:
#include "Level.h"

//Skybox
#include "Skybox.h"

//Tornyok:
#include "Tower.h"
#include "CheapTower.h"
#include "FastReloadTower.h"
#include "StrongTower.h"
#include "Roadblock.h"

//Ellenségek:
#include "Enemy.h"

//Ágyúgolyók:
#include "CannonBall.h"

constexpr auto STARTING_FUNDS = 25;
constexpr auto STARTING_ENEMY_SPAWN_RATE = 5;

class CMyApp
{
protected:
	//FBO
	FrameBufferObject* frameBufferObject;

	//Ablak felbontása
	int windowWidth = 640;
	int windowHeight = 480;

	//Egérpozíció és a felületi pont, amire mutatsz:
	int mouseX;
	int mouseY;
	glm::vec3* selectedSpot = new glm::vec3(0, 0, 0);

	//-----------------------------------------------

	//Sky box
	Skybox* skybox;

	//Kamera
	gCamera	camera;

	//A pálya:
	Level* level;
	int levelSizeSetting = 25;

	//A programban használt mesh-ek listája (Flyweight tervminta):
	enum MeshId {ENEMY, CHEAPTOWER, FASTRELOADTOWER, STRONGTOWER, ROADBLOCK, CANNONBALL};
	std::map<MeshId, MeshObject*> meshes;

	//Megy-e éppen a játék:
	bool gameOn;

	//Tornyok lerakása és frissítése:
	bool buildModeOn = false;
	std::vector<std::string> buildingTypeNames = { "Olcso torony", "Sorozatlovo torony", "Eros torony", "Utakadaly" };		//A listázandó elemek nevei.
	std::vector<std::string> listItemNames;
	int selectedBuildingType = 0; 								//Az éppen kiválasztott elem indexe.
	std::string buildButtonText;
	std::string upgradeButtonText;
	std::vector<Tower*> installedTowers;
	std::vector<Roadblock*> installedRoadblocks;

	//Ellenségek lerakása és mozgatása:
	std::vector<Enemy*> enemies;
	int totalEnemiesSpawned = 0;
	int totalEnemiesKilled = 0;
	int numberOfEnemiesPassedTrough = 0;
	int enemiesPassedTroughLimit = 15;

	//Agyúgolyók létrehozása és mozgatása:
	std::vector<CannonBall*> cannonBalls;

	//Játék időzítése:
	float gameSpeed = 1;									//Az idő múlási sebessége.
	float enemyMovementWaitingTime = 0;						//Várunk a következő léptetési ciklusra.
	float enemyMovementRate = 3;							//Az ellenségek ennyi másodpercenként lépnek egyet.
	float enemySpawnWaitingTime = 0;						//Várunk a következő spawnolási ciklusra.
	float enemySpawnRate = STARTING_ENEMY_SPAWN_RATE;		//Úr ellenség lerakása ennyi másodpercenként.

	//Játékos pénze:
	int funds = STARTING_FUNDS;

public:
	//Játék betöltése:
	CMyApp();
	~CMyApp();
	bool Init();
	void InitMeshes();
	void generateListItemNames();
	void startGame();


	//Az updatecycle műveletei:
	void Update();
	void endGame();
	void moveEnemies(float deltaTime);
	void earn(int amount);
	void spawnEnemies(float deltaTime);
	void updateTowers(float deltaTime);
	void updateCannonBalls();

	//Renderelési fázis:
	void Render();
	void renderGame(glm::mat4& viewProj);
	void displayImGUI();
	void restartGame();
	void clearGameBoard();
	void resetGameObjects();
	void resetTowers();
	void refreshListItemNames();
	void resetEnemies();
	void generateBuildButtonText();
	bool selectedBuildingTypeAtMaxLevel();
	void generateUpgradeButtonText();

	//Billentyűzetbevitelek kezelése:
	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);

	//Egérbevitelek kezelése:
	void MouseMove(SDL_MouseMotionEvent&);
	void notifyLevel();
	void MouseDown(SDL_MouseButtonEvent&);
	void installBuilding(int lineIndex, int columnIndex);
	//bool canBuildTowerHere(int lineIndex, int columnIndex);
	bool canBuildCheapTowerHere(int lineIndex, int columnIndex);
	bool canBuildFastReloadTowerHere(int lineIndex, int columnIndex);
	bool canBuildStrongTowerHere(int lineIndex, int columnIndex);
	bool canBuildRoadblockHere(int lineIndex, int columnIndex);
	bool canAfford(int price);
	void installCheapTower(int lineIndex, int columnIndex);
	void installFastReloadTower(int lineIndex, int columnIndex);
	void installStrongTower(int lineIndex, int columnIndex);
	void installRoadblock(int lineIndex, int columnIndex);
	void pay(int amount);
	void build();
	void cancelBuildMode();
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);

	void Resize(int, int);	//Ablak átméretezése
	void Clean();			//Program bezárása
};

