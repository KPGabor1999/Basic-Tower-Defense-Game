#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <string>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"
#include <time.h>

//Játékobjektumok beimportálása:
#include "Level.h"

CMyApp::CMyApp(void)
{
}

CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	frameBufferObject = new FrameBufferObject(windowWidth, windowHeight);
	skybox = new Skybox(
		"assets/xpos.jpg",
		"assets/xneg.jpg",
		"assets/ypos.jpg",
		"assets/yneg.jpg",
		"assets/zpos.jpg",
		"assets/zneg.jpg"
		);
	level = new Level();
	InitMeshes();
	generateListItemNames();
	startGame();
	//enemies.push_back(new Enemy(level));

	
	/*std::cout << "Size of a tile: " << level->getTileWidth() << std::endl;
	std::cout << "Radius of a tile: " << level->getTileRadius() << std::endl;
	std::cout << "Scale of the tiles: " << level->getScale() << std::endl;
	std::cout << "Initialized Tiles: " << std::endl;
	for (auto currentLine : level->levelTilesData) {
		for (auto currentTile : currentLine) {
			currentTile->toString();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
	
	//Kamera:

	//A kamera mindig a bejáratra nézzen:
	float eyeX = (level->getLevelSize() / 2 * level->getTileWidth() - level->getTileRadius()) + 10;
	float eyeZ = level->getTileWidth() + 8;

	//Az eye és az at legyenek közel egymáshoz:
	camera.SetView(glm::vec3(eyeX, 3, eyeZ), glm::vec3(eyeX - 1, 3, eyeZ - 1), glm::vec3(0, 1, 0));
	camera.SetProj(60.0f, (float) windowWidth / (float) windowHeight , 0.01f, 1000.0f);

	return true;
}

void CMyApp::startGame() {
	gameOn = true;
}

void CMyApp::InitMeshes() {
	meshes[ENEMY]           = new MeshObject("assets/Enemy.obj", "assets/Enemy.jpg", "shaders/Enemy.vert", "shaders/Enemy.frag");
	meshes[CHEAPTOWER]      = new MeshObject("assets/CheapTower.obj", "assets/CheapTower.jpg", "shaders/Tower.vert", "shaders/Tower.frag");
	meshes[FASTRELOADTOWER] = new MeshObject("assets/FastReloadTower.obj", "assets/FastReloadTower.jpg", "shaders/Tower.vert", "shaders/Tower.frag");
	meshes[STRONGTOWER]     = new MeshObject("assets/StrongTower.obj", "assets/StrongTower.jpg", "shaders/Tower.vert", "shaders/Tower.frag");
	meshes[ROADBLOCK]       = new MeshObject("assets/Roadblock.obj", "assets/Roadblock.jpg", "shaders/Roadblock.vert", "shaders/Roadblock.frag");
	meshes[CANNONBALL]      = new MeshObject("assets/CannonBall.obj", "assets/CannonBall.jpg", "shaders/CannonBall.vert", "shaders/CannonBall.frag");
}

void CMyApp::generateListItemNames() {
	for (int currentIndex = 0; currentIndex < buildingTypeNames.size() - 1; currentIndex++) {
		std::string listItemName = buildingTypeNames[currentIndex].c_str();
		listItemName += " (";

		int buildingLevel;
		switch (currentIndex) {
		case 0: buildingLevel = CheapTower::towerLevel; break;
		case 1: buildingLevel = FastReloadTower::towerLevel; break;
		case 2: buildingLevel = StrongTower::towerLevel; break;
		}

		if(buildingLevel < 4) listItemName += std::to_string(buildingLevel);

		switch (buildingLevel) {
		case 1: listItemName += "-es szint)"; break;
		case 2: listItemName += "-es szint)"; break;
		case 3: listItemName += "-as szint)"; break;
		case 4: listItemName += "MAX)"; break;
		}

		listItemNames.push_back(listItemName);
	}

	listItemNames.push_back(buildingTypeNames[buildingTypeNames.size() - 1]);
}

void CMyApp::Update()
{
	static Uint32 lastTime = SDL_GetTicks();
	float deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;

	if (gameOn) {
		moveEnemies(deltaTime);
		spawnEnemies(deltaTime);
		updateTowers(deltaTime);
		updateCannonBalls();

		if (numberOfEnemiesPassedTrough > enemiesPassedTroughLimit) endGame();
	}

	camera.Update(deltaTime, level->getScale());

	lastTime = SDL_GetTicks();
}

void CMyApp::endGame() {
	gameOn = false;
}

void CMyApp::moveEnemies(float deltaTime) {
	enemyMovementWaitingTime += gameSpeed * deltaTime;

	//Csak a lépésközönként lépnek előre egy mezőt:
	if (enemyMovementWaitingTime >= enemyMovementRate) {
		for (std::vector<Enemy*>::iterator currentEnemy = enemies.begin(); currentEnemy != enemies.end(); ) {
			(*currentEnemy)->stepForward();
			if (!(*currentEnemy)->isAlive()) {
				//std::cout << "Kinyirtunk egy ellenseget." << std::endl;
				totalEnemiesKilled++;
				earn((*currentEnemy)->getReward());
				currentEnemy = enemies.erase(currentEnemy);
			}
			else if ((*currentEnemy)->passedThrough()) {
				//std::cout << "Egy ellenseg atjutott." << std::endl;
				numberOfEnemiesPassedTrough++;
				currentEnemy = enemies.erase(currentEnemy);
			}
			else {
				//Ha még nem, békén hagyjuk:
				++currentEnemy;
			}
		}
		enemyMovementWaitingTime -= enemyMovementRate;
	}

	//Amúgy a soronkövetkező mező felé csúsznak:
	for (std::vector<Enemy*>::iterator currentEnemy = enemies.begin(); currentEnemy != enemies.end(); ++currentEnemy) {
		(*currentEnemy)->move(enemyMovementRate, gameSpeed);
	}
}

void CMyApp::earn(int amount) {
	funds += amount;
}

void CMyApp::spawnEnemies(float deltaTime) {
	enemySpawnWaitingTime += gameSpeed * deltaTime;

	//Ellenségek lerakása azonos időközönként:
	if (enemySpawnWaitingTime >= enemySpawnRate) {
		enemies.push_back(new Enemy(level, meshes[MeshId::ENEMY]));
		totalEnemiesSpawned++;
		if (totalEnemiesSpawned % 10 == 0) {	//Minden 10. lerakott ellenség után megerősödnek.
			Enemy::upgrade();
			enemySpawnRate *= 0.9f;
			if (enemySpawnRate < 2.0f) enemySpawnRate = 2.0f;

			std::cout << "Enemies got stronger:" << std::endl;
			std::cout << "Spawning rate: every " << enemySpawnRate << " seconds" << std::endl;
			std::cout << "Hp: " << Enemy::startingHp<< std::endl;
			std::cout << "Reward: " << Enemy::startingReward << std::endl << std::endl;
		}
		enemySpawnWaitingTime -= enemySpawnRate;
	}
}

void CMyApp::updateTowers(float deltaTime) {
	//Ágyúk elsütése/újratöltése:
	for (auto currentTower : installedTowers) {
		currentTower->fire(gameSpeed, deltaTime, enemies, cannonBalls, meshes[MeshId::CANNONBALL]);
	}
}

void CMyApp::updateCannonBalls() {
	//Ágyúgolyók mozgatása és eltűntetése:
	for (std::vector<CannonBall*>::iterator currentCannonBall = cannonBalls.begin(); currentCannonBall != cannonBalls.end(); ) {
		if ((*currentCannonBall)->move(gameSpeed)) {
			++currentCannonBall;
		}
		else {
			currentCannonBall = cannonBalls.erase(currentCannonBall);
		}
	}
}

void CMyApp::Render()
{
	//FBO bekapcsolása:
	frameBufferObject->load();

	//Töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT):
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = camera.GetViewProj();

	renderGame(viewProj);

	//Post processing effektek aktiválása, majd FBO kikapcsolása
	frameBufferObject->applyPostProcessingEffects();

	//ImGui
	displayImGUI();
}

void CMyApp::renderGame(glm::mat4& viewProj) {
	//Sky box kirajzolása:
	skybox->render(viewProj, camera.GetEye());

	//Pálya kirajzolása:
	level->render(viewProj);

	//Tornyok kirajzolása:
	for (auto currentTower : installedTowers) currentTower->render(viewProj);

	//Útakadályok kirajzolása:
	for (auto currentRoadblock : installedRoadblocks) currentRoadblock->render(viewProj);

	//Ellenségek kirajzolása:
	for (auto currentEnemy : enemies) currentEnemy->render(viewProj);

	//Ágyúgolyók kirajzolása:
	for (auto currentCannonBall : cannonBalls) currentCannonBall->render(viewProj);
}

void CMyApp::displayImGUI() {
	//ImGui::ShowTestWindow();
	if (ImGui::Begin("Iranyito panel: ")) {
		if (!gameOn) {
			//Game Over ablak, játék újrakezése:
			ImGui::Text("Jatek vege: Tul sokan jutottak at...");
			std::string finalScoreText = "Vegso pontszam: " + std::to_string(totalEnemiesKilled);
			ImGui::Text(finalScoreText.c_str());
			ImGui::Text("");
		}

		//N*N-es pálya generálása:
		ImGui::Text("Palya merete (nxn): "); ImGui::SameLine(); ImGui::DragInt("", &levelSizeSetting, 0.2f, 25, 50);
		if (ImGui::Button("Uj jatek")) restartGame();

		if (gameOn) {
			//HUD:
			std::string passedThrough = std::to_string(numberOfEnemiesPassedTrough);
			std::string passedThroughLimit = std::to_string(enemiesPassedTroughLimit);
			std::string cash = std::to_string(funds);

			ImGui::Text("Atjutottak: "); ImGui::SameLine(); ImGui::Text(passedThrough.c_str());
			ImGui::Text("Limit: "); ImGui::SameLine(); ImGui::Text(passedThroughLimit.c_str());
			ImGui::Text("Penz: "); ImGui::SameLine(); ImGui::Text(cash.c_str());

			//A választható épületek listája:
			ImGui::Text("Epulettipusok: "); ImGui::SameLine(); ImGui::ListBox("", &selectedBuildingType, listItemNames);

			//Építés gomb:
			generateBuildButtonText();
			if (ImGui::Button(buildButtonText.c_str())) buildModeOn = true;

			//Fejlesztés gomb:
			if (selectedBuildingType < 3 && !selectedBuildingTypeAtMaxLevel()) {
				ImGui::SameLine();
				generateUpgradeButtonText();
				if (ImGui::Button(upgradeButtonText.c_str())) {
					switch (selectedBuildingType) {
					case 0:
						if (canAfford(CheapTower::upgradeCost) && CheapTower::isUpgradable()) {
							pay(CheapTower::upgradeCost);
							CheapTower::upgrade();
						} break;
					case 1:
						if (canAfford(FastReloadTower::upgradeCost) && FastReloadTower::isUpgradable()) {
							pay(FastReloadTower::upgradeCost);
							FastReloadTower::upgrade();
						} break;
					case 2:
						if (canAfford(StrongTower::upgradeCost) && StrongTower::isUpgradable()) {
							pay(StrongTower::upgradeCost);
							StrongTower::upgrade();
						} break;
					}
				}
				refreshListItemNames();
			}

			//Játéksebesség beállítása:
			ImGui::Text("Jateksebesseg: "); ImGui::SameLine();
			if (ImGui::Button("Szunet")) gameSpeed = 0.0f; ImGui::SameLine();
			if (ImGui::Button("0.5x")) gameSpeed = 0.5f; ImGui::SameLine();
			if (ImGui::Button("1x")) gameSpeed = 1.0f; ImGui::SameLine();
			if (ImGui::Button("2x")) gameSpeed = 2.0f; ImGui::SameLine();
			if (ImGui::Button("5x")) gameSpeed = 5.0f; ImGui::SameLine();
			if (ImGui::Button("10x")) gameSpeed = 10.0f;
		}
	}
	ImGui::End();
}

void CMyApp::restartGame() {
	std::cout << "Starting a new game..." << std::endl;
	clearGameBoard();
	resetGameObjects();
	delete level;
	level = new Level(levelSizeSetting);
	totalEnemiesSpawned = 0;
	totalEnemiesKilled = 0;
	numberOfEnemiesPassedTrough = 0;
	enemySpawnRate = STARTING_ENEMY_SPAWN_RATE;
	funds = STARTING_FUNDS;
	startGame();
}

void CMyApp::clearGameBoard() {

	for (auto currentTower : installedTowers) delete currentTower;
	installedTowers.clear();

	for (auto currentCannonBall : cannonBalls) delete currentCannonBall;
	cannonBalls.clear();

	for (auto currentRoadblock : installedRoadblocks) delete currentRoadblock;
	installedRoadblocks.clear();

	for (auto currentEnemy : enemies) delete currentEnemy;
	enemies.clear();
}

void CMyApp::resetGameObjects() {
	resetTowers();
	resetEnemies();
}

void CMyApp::resetTowers() {
	CheapTower::reset();
	FastReloadTower::reset();
	StrongTower::reset();
	refreshListItemNames();
}

void CMyApp::refreshListItemNames() {
	listItemNames.clear();
	generateListItemNames();
}

void CMyApp::resetEnemies() {
	Enemy::reset();
	enemySpawnRate = STARTING_ENEMY_SPAWN_RATE;
}

void CMyApp::generateBuildButtonText() {
	buildButtonText = "Epit(";

	switch (selectedBuildingType) {
	case 0: buildButtonText += std::to_string(CheapTower::buildingCost); break;
	case 1: buildButtonText += std::to_string(FastReloadTower::buildingCost); break;
	case 2: buildButtonText += std::to_string(StrongTower::buildingCost); break;
	case 3: buildButtonText += std::to_string(Roadblock::buildingCost); break;
	}
	buildButtonText += ")";
}

bool CMyApp::selectedBuildingTypeAtMaxLevel() {
	switch (selectedBuildingType) {
	case 0: return CheapTower::towerLevel == 4; break;
	case 1: return FastReloadTower::towerLevel == 4; break;
	case 2: return StrongTower::towerLevel == 4; break;
	}
}

void CMyApp::generateUpgradeButtonText() {
	upgradeButtonText = "Fejleszt(";

	switch (selectedBuildingType) {
	case 0: upgradeButtonText += std::to_string(CheapTower::upgradeCost); break;
	case 1: upgradeButtonText += std::to_string(FastReloadTower::upgradeCost); break;
	case 2: upgradeButtonText += std::to_string(StrongTower::upgradeCost); break;
	}

	upgradeButtonText += ")";
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	camera.MouseMove(mouse);

	//Minden egérmozdulatkor lekérjük az egér pozícióját.
	mouseX = mouse.x;
	mouseY = mouse.y;

	if (buildModeOn) notifyLevel();
}

void CMyApp::notifyLevel() {
	//Építőmódban minden egérmozdulatkor kiolvassuk azt a mezőt, amin az egér áll és továbbküldjük az adatait a pályának:
	frameBufferObject->readPixels(mouseX, mouseY, windowHeight, selectedSpot);

	//A pályának 3 dolgot kell tudnia:

	//Építkezünk-e?
	level->setBuildModeOn(true);

	// Melyik mezőre?
	int selectedLineIndex = (int)(selectedSpot->z / (level->getTileWidth()));
	int selectedColumnIndex = (int)(selectedSpot->x / (level->getTileWidth()));
	level->setSelectedTile(selectedLineIndex, selectedColumnIndex);

	//Oda szabad-e?
	switch (selectedBuildingType) {
	case 0: level->setCanBuildHere(canBuildCheapTowerHere(selectedLineIndex, selectedColumnIndex)); break;
	case 1: level->setCanBuildHere(canBuildFastReloadTowerHere(selectedLineIndex, selectedColumnIndex)); break;
	case 2: level->setCanBuildHere(canBuildStrongTowerHere(selectedLineIndex, selectedColumnIndex)); break;
	case 3: level->setCanBuildHere(canBuildRoadblockHere(selectedLineIndex, selectedColumnIndex)); break;
	}
}

bool CMyApp::canBuildCheapTowerHere(int lineIndex, int columnIndex) {
	Tile* selectedTile = level->getTileData(lineIndex, columnIndex);
	return canAfford(CheapTower::buildingCost) && selectedTile->isANormalHill() && !(selectedTile->hasTower());
}

bool CMyApp::canBuildFastReloadTowerHere(int lineIndex, int columnIndex) {
	Tile* selectedTile = level->getTileData(lineIndex, columnIndex);
	return canAfford(FastReloadTower::buildingCost) && selectedTile->isANormalHill() && !(selectedTile->hasTower());
}

bool CMyApp::canBuildStrongTowerHere(int lineIndex, int columnIndex) {
	Tile* selectedTile = level->getTileData(lineIndex, columnIndex);
	return canAfford(StrongTower::buildingCost) && selectedTile->isANormalHill() && !(selectedTile->hasTower());
}

bool CMyApp::canBuildRoadblockHere(int lineIndex, int columnIndex) {
	Tile* selectedTile = level->getTileData(lineIndex, columnIndex);

	if (!canAfford(Roadblock::buildingCost)) return false;
	if (selectedTile->isAHill()) return false;
	if (selectedTile->hasRoadblock()) return false;
	if (level->isEntranceOrExit(selectedTile)) return false;
	if (selectedTile->isInAnInnerHillRow()) return false;

	int numberOfFreeTiles = 0;
	for (auto currentTile : level->getLineData(lineIndex)) {
		if (currentTile->getHeight() < 0.1f && !(currentTile->hasRoadblock())) {
			numberOfFreeTiles++;
		}
	}
	//std::cout << "Sor szabad emzőinek száma: " << numberOfFreeTiles << std::endl;
	return numberOfFreeTiles >= 2;
}

bool CMyApp::canAfford(int price) {
	return funds >= price;
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT) {
		build();
	}
	else if (mouse.button == SDL_BUTTON_RIGHT) {
		cancelBuildMode();
	}
}

void CMyApp::build() {
	if (!buildModeOn) return;

	//Visszafejted a kattintott mező helyét (a mátrixban):
	frameBufferObject->readPixels(mouseX, mouseY, windowHeight, selectedSpot);
	int lineIndex = (int)(selectedSpot->z / (level->getTileWidth()));
	int columnIndex = (int)(selectedSpot->x / (level->getTileWidth()));

	/*
	//Debug:
	std::cout << "Eger pozicioja (ablakhoz kepest): (" << mouseX << "," << mouseY << ")" << std::endl;
	std::cout << "A kattintott feluletpont pozicioja: ("
		<< selectedSpot->x << "," << selectedSpot->y << "," << selectedSpot->z << ")" << std::endl;
	std::cout << "A kattintott mezo indexei: [" << lineIndex << "][" << columnIndex << "]" << std::endl;
	*/

	//És ha lehet rá építkezni, lerakjuk rá a megfelelő típusú tornyot:
	installBuilding(lineIndex, columnIndex);

	buildModeOn = false;
	level->setBuildModeOn(false);
}

void CMyApp::installBuilding(int lineIndex, int columnIndex) {
	Tile* selectedTile = level->getTileData(lineIndex, columnIndex);

	if (buildingTypeNames[selectedBuildingType].compare("Olcso torony") == 0) {
		if (canBuildCheapTowerHere(lineIndex, columnIndex)) installCheapTower(lineIndex, columnIndex);
	}
	else if (buildingTypeNames[selectedBuildingType].compare("Sorozatlovo torony") == 0) {
		if (canBuildFastReloadTowerHere(lineIndex, columnIndex)) installFastReloadTower(lineIndex, columnIndex);
	}
	else if (buildingTypeNames[selectedBuildingType].compare("Eros torony") == 0) {
		if (canBuildStrongTowerHere(lineIndex, columnIndex)) installStrongTower(lineIndex, columnIndex);
	}
	else if (buildingTypeNames[selectedBuildingType].compare("Utakadaly") == 0) {
		if (canBuildRoadblockHere(lineIndex, columnIndex)) installRoadblock(lineIndex, columnIndex);
	}
}

void CMyApp::installCheapTower(int lineIndex, int columnIndex) {
	pay(CheapTower::buildingCost);
	installedTowers.push_back(new CheapTower(level, lineIndex, columnIndex, meshes[MeshId::CHEAPTOWER]));
	level->installTower(lineIndex, columnIndex);
}

void CMyApp::installFastReloadTower(int lineIndex, int columnIndex) {
	pay(FastReloadTower::buildingCost);
	installedTowers.push_back(new FastReloadTower(level, lineIndex, columnIndex, meshes[MeshId::FASTRELOADTOWER]));
	level->installTower(lineIndex, columnIndex);
}

void CMyApp::installStrongTower(int lineIndex, int columnIndex) {
	pay(StrongTower::buildingCost);
	installedTowers.push_back(new StrongTower(level, lineIndex, columnIndex, meshes[MeshId::STRONGTOWER]));
	level->installTower(lineIndex, columnIndex);
}

void CMyApp::installRoadblock(int lineIndex, int columnIndex) {
	pay(Roadblock::buildingCost);
	installedRoadblocks.push_back(new Roadblock(level, lineIndex, columnIndex, meshes[MeshId::ROADBLOCK]));
	level->installRoadblock(lineIndex, columnIndex);
}

void CMyApp::pay(int amount) {
	funds -= amount;
}

void CMyApp::cancelBuildMode() {
	//Visszavonod az építkezést:
	buildModeOn = false;
	level->setBuildModeOn(false);
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	windowWidth = _w;
	windowHeight = _h;

	glViewport(0, 0, _w, _h );

	camera.Resize(_w, _h);

	frameBufferObject->CreateFrameBuffer(_w, _h);
}

void CMyApp::Clean()
{
	//Törlünk minden pointert, hogy elkerüljük a memory leak-eket.
	std::cout << "Cleaning up memory...";
	frameBufferObject->clean();
	delete selectedSpot;
	delete skybox;
	delete level;
	clearGameBoard();
	std::cout << "Done." << std::endl;
}