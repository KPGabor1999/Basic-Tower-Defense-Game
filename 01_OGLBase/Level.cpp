#include "Level.h"
#include <math.h>
#include <imgui/imgui.h>

Level::Level() {
	tileResolution = 10;
	levelSize = 25;
	levelResolution = levelSize * tileResolution;
	tileRadius = (tileResolution / 2) / (float)levelResolution;
	scale = 40;

	// a shadereket t�rol� program l�trehoz�sa az OpenGL-hez hasonl� m�don:
	surfaceProgram.Init(
		{
			{ GL_VERTEX_SHADER, "shaders/Level.vert"},
			{ GL_FRAGMENT_SHADER, "shaders/Level.frag"}
		},
		{
			{ 0, "vs_in_pos" },		// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },	// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 2, "vs_in_tex" },		// VAO 1-es csatorna menjen a vs_in_tex-be
		},
		{
			{0, "fs_out_col"} ,
			{1, "fs_out_data"}
		}
	);

	grassTexture.FromFile("assets/Grass.jpg");
	hillTexture.FromFile("assets/Hill.jpg");

	generate();
}

Level::Level(int size) {
	tileResolution = 10;
	levelSize = size;
	levelResolution = levelSize * tileResolution;
	tileRadius = (tileResolution / 2) / (float)levelResolution;
	scale = 40;

	// a shadereket t�rol� program l�trehoz�sa az OpenGL-hez hasonl� m�don:
	surfaceProgram.Init(
		{
			{ GL_VERTEX_SHADER, "shaders/Level.vert"},
			{ GL_FRAGMENT_SHADER, "shaders/Level.frag"}
		},
		{
			{ 0, "vs_in_pos" },		// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },	// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 2, "vs_in_tex" },		// VAO 1-es csatorna menjen a vs_in_tex-be
		},
		{
			{0, "fs_out_col"} ,
			{1, "fs_out_data"}
		}
		);

	grassTexture.FromFile("assets/Grass.png");
	hillTexture.FromFile("assets/Hill.png");

	generate(size);
}

float Level::getTileRadius() {
	return tileRadius * scale;
}

int Level::getScale() {
	return scale;
}

float Level::getTileHeight(int lineIndex, int columnIndex) {
	return levelTilesData[lineIndex][columnIndex]->getHeight();
}

int Level::getLevelSize() {
	return levelSize;
}

void Level::setLevelSize(int newSize) {
	levelSize = newSize;
}

int Level::getTileResolution() {
	return tileResolution;
}

float Level::getTileWidth() {
	return (tileResolution / (float)levelResolution) * scale;
}

std::vector<Tile*> Level::getLineData(int lineIndex) {
	return levelTilesData[lineIndex];
}

Tile* Level::getTileData(int lineIndex, int columnIndex) {
	return levelTilesData[lineIndex][columnIndex];
}

bool Level::selectedTileIsAHill(int lineIndex, int columnIndex) {
	return this->getTileData(lineIndex, columnIndex)->getHeight() >= 0.5f;
}

bool Level::hasTower(int lineIndex, int columnIndex) {
	return levelTilesData[lineIndex][columnIndex]->hasTower();
}

bool Level::hasRoadblock(int lineIndex, int columnIndex) {
	return levelTilesData[lineIndex][columnIndex]->hasRoadblock();
}

bool Level::isEntranceOrExit(Tile* selectedTile) {
	return
		(selectedTile->getLineIndex() == 0 || selectedTile->getLineIndex() == getLevelSize() - 1) ||
		(getLevelSize() % 2 == 0 && selectedTile->getLineIndex() == getLevelSize() - 2 && selectedTile->getColumnIndex() == getLevelSize() / 2);
}

glm::vec3 Level::getTileCenter(Tile* currentTile) {
	return currentTile->getCenter();
}

void Level::installTower(int lineIndex, int columnIndex) {
	levelTilesData[lineIndex][columnIndex]->installTower();
}

void Level::installRoadblock(int lineIndex, int columnIndex) {
	levelTilesData[lineIndex][columnIndex]->installRoadblock();
	BFS();
}

void Level::setBuildModeOn(bool buildModeOn) {
	this->buildModeOn = buildModeOn;
}

void Level::setSelectedTile(int lineIndex, int columnIndex) {
	selectedTile = getTileData(lineIndex, columnIndex);
}

void Level::setCanBuildHere(bool conditionPassed) {
	canBuildHere = conditionPassed;
}

void Level::generate() {
	initializeGameBoard();
	initializeLevelTopology();
}

void Level::generate(int size) {
	levelSize = size;
	levelResolution = levelSize * tileResolution;
	tileRadius = (tileResolution / 2) / (float)levelResolution;

	//std::cout << "Tile_radius: " << tileRadius << std::endl;
	levelTilesData.clear();
	initializeGameBoard();
	initializeLevelTopology();
}

void Level::initializeGameBoard() {
	srand(time(0));

	std::cout << "Initializing tile heights... ";
	for (int lineIndex = 0; lineIndex < levelSize; lineIndex++) {
		//Hozz�adjuk az �j sort:
		std::vector<Tile*> currentRow;
		levelTilesData.push_back(currentRow);
		//Az �j sort felt�ltj�k elemekkel:
		for (int columnIndex = 0; columnIndex < levelSize; columnIndex++) {
			float currentHeight = 0.0f;
			//A p�lya sz�l�n l�v� mez�k 2-es magass�g�ak:
			if (lineIndex == 0 || columnIndex == 0 || lineIndex == levelSize - 1 || columnIndex == levelSize - 1) {
				currentHeight = 2.0f;
			}
			//Minden m�sodik sor belseje s�k:
			else if ((lineIndex + 1) % 2 == 0) {
				currentHeight = 0.0f;
			}
			//K�l�nben a mez�n legyen v�letlen magass�g� domb:
			else {
				int randomNumber = rand() % 10 + 1;		//1-10
				if (randomNumber <= 7) {				//70% es�ly
					randomNumber = rand() % 6 + 5;		//5-10
					currentHeight = randomNumber * 0.1f;
				}
				else {
					currentHeight = 0.0f;
				}
			}

			Tile* currentTile = new Tile(lineIndex, columnIndex, currentHeight, getTileWidth(), getTileRadius());

			//Legy�rtott mez�adatok felv�tele a m�trixba:
			levelTilesData[lineIndex].push_back(currentTile);
		}
	}
	std::cout << "Done." << std::endl;

	//A p�lya kij�rat�t �s bej�rat�t ut�lag sz�rjuk be:		//�gy gazdas�gosabb.
	std::cout << "Installing track ends... ";
	levelTilesData[0][levelSize / 2]->setHeight(0.0f);
	levelTilesData[1][levelSize / 2]->setHeight(0.0f);
	levelTilesData[levelSize - 1][levelSize / 2]->setHeight(0.0f);
	levelTilesData[levelSize - 2][levelSize / 2]->setHeight(0.0f);
	std::cout << "Done." << std::endl;

	//Biztos�tsuk, hogy minden sorban van legal�bb 1 �res mez�:
	std::cout << "Unblocking path... ";
	for (int lineIndex = 2; lineIndex < levelSize; lineIndex += 2) {			//El�g csak minden p�ratlanadik sort bej�rni a 3-adikt�l kezdve:
		//Megsz�moljuk a soron bel�li �res mez�ket:
		int  numberOfEmptyTiles = 0;
		for (int columnIndex = 1; columnIndex < levelSize - 1; columnIndex++) {	//Azokon bel�l is csak a bels� (nem sz�ls� mez�ket):
			if (levelTilesData[lineIndex][columnIndex]->getHeight() < 0.5f) {		//Akkor csak 0 lehet a magass�g.
				numberOfEmptyTiles++;
			}
		}
		//�s ha egy sincs, v�letlenszer�en csin�lunk egyet:
		if (numberOfEmptyTiles == 0) {
			int randomTileIndex = rand() % (levelSize - 1) + 1;
			levelTilesData[lineIndex][randomTileIndex]->setHeight(0.0f);
		}
	}
	std::cout << "Done." << std::endl;

	//BFS algoritmus lefuttat�sa a kij�ratb�l indulva:
	BFS();
}

/*
BFS (G, s)                   //Where G is the graph and s is the source node
	  let Q be queue.
	  Q.enqueue( s ) //Inserting s in queue until all its neighbour vertices are marked.

	  mark s as visited.
	  while ( Q is not empty)
		   //Removing that Tile from queue,whose neighbour will be visited now
		   v  =  Q.dequeue( )

		  //processing all the neighbours of v
		  for all neighbours w of v in Graph G
			   if w is not visited
						Q.enqueue( w )             //Stores w in Q to further visit its neighbour
						mark w as visited.
*/

void Level::BFS() {
	std::queue<Tile*> Q;

	//Az algoritmust a p�lya kij�rat�t�l ind�tjuk:
	Tile* exit = levelTilesData[levelSize - 1][levelSize / 2];
	exit->setParent(nullptr);
	Q.push(exit);

	visit(exit);

	while (!Q.empty()) {
		Tile* currentTile = Q.front(); Q.pop();

		//�sszegy�jtj�k szomsz�dokat egy list�ba:
		std::vector<Tile*> neighbors;
		if (hasUpperNeighbor(currentTile)) {
			Tile* upperNeighbor = getUpperNeighbor(currentTile);
			if (upperNeighbor->isValidNeighbor()) neighbors.push_back(upperNeighbor);
		}
		if (hasLowerNeighbor(currentTile)) {
			Tile* lowerNeighbor = getLowerNeighbor(currentTile);
			if (lowerNeighbor->isValidNeighbor()) neighbors.push_back(lowerNeighbor);
		}
		if (hasLeftNeighbor(currentTile)) {
			Tile* leftNeighbor = getLeftNeighbor(currentTile);
			if (leftNeighbor->isValidNeighbor()) neighbors.push_back(leftNeighbor);
		}
		if (hasRightNeighbor(currentTile)) {
			Tile* rightNeighbor = getRightNeighbor(currentTile);
			if (rightNeighbor->isValidNeighbor()) neighbors.push_back(rightNeighbor);
		}

		//Bej�rjuk a vizsg�lt cs�cs szomsz�dait:
		for (Tile* currentNeighbor : neighbors) {
			//�s betessz�k a sorba a m�g meg nem l�togatottakat:
			if (!currentNeighbor->hasBeenVisited()) {
				visit(currentNeighbor);
				currentNeighbor->setParent(currentTile);		//Minden bev�logatott cs�csnak legyen sz�l�je. Az ellens�gek a sz�l�l�ncon l�pkednek v�gig.
				currentNeighbor->setDistanceFromExit( sqrt(pow((exit->getLineIndex() - currentNeighbor->getLineIndex()),2) + pow((exit->getColumnIndex() - currentNeighbor->getColumnIndex()),2)) );
				Q.push(currentNeighbor);
			}
		}
	}
	/*
	//L�ssuk a gener�lt gr�fot:
	std::cout << "The generated BFS graph: " << std::endl << std::endl;
	for (int lineIndex = 0; lineIndex < levelSize; lineIndex++) {
		for (int columnIndex = 0; columnIndex < levelSize; columnIndex++) {
			//std::cout << (levelTilesData[lineIndex][columnIndex]->has_been_visited) << "   ";
			std::cout << (levelTilesData[lineIndex][columnIndex]->distanceFromExit) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/

	//V�g�l vissza�ll�tjuk a mez�k megl�togatotts�g�t (a k�vetkez� h�v�sokra).
	unVisitTiles();
}

bool Level::hasUpperNeighbor(Tile* currentTile) {
	return currentTile->getLineIndex() > 0;
}

Tile* Level::getUpperNeighbor(Tile* currentTile) {
	return getTileData(currentTile->getLineIndex() - 1, currentTile->getColumnIndex());
}

bool Level::hasLowerNeighbor(Tile* currentTile) {
	return currentTile->getLineIndex() < levelSize - 1;
}

Tile* Level::getLowerNeighbor(Tile* currentTile) {
	return getTileData(currentTile->getLineIndex() + 1, currentTile->getColumnIndex());
}

bool Level::hasLeftNeighbor(Tile* currentTile) {
	return currentTile->getColumnIndex() > 0;
}

Tile* Level::getLeftNeighbor(Tile* currentTile) {
	return getTileData(currentTile->getLineIndex(), currentTile->getColumnIndex() - 1);
}

bool Level::hasRightNeighbor(Tile* currentTile) {
	return currentTile->getColumnIndex() < levelSize - 1;
}

Tile* Level::getRightNeighbor(Tile* currentTile) {
	return getTileData(currentTile->getLineIndex(), currentTile->getColumnIndex() + 1);
}

void Level::visit(Tile* tile) {
	tile->setVisited(true);
}

void Level::unVisit(Tile* tile) {
	tile->setVisited(false);
}

void Level::unVisitTiles() {
	for (auto currentLine : levelTilesData) {
		for (auto currentTile : currentLine) {
			unVisit(currentTile);
		}
	}
}

glm::vec3 get_surface_position(float u, float v, float height) {	//Fel�let Tilepoz�ci�i
	float x = u;
	float z = v;
	float y = height;

	return glm::vec3(x, y, z);
}

glm::vec3 get_surface_normal(float u, float v, float height)
{
	// Numerikusan (nem kell ismerni a k�pletet, el�g a poz�ci��t)
	glm::vec3 du = get_surface_position(u + 0.01, v, height) - get_surface_position(u - 0.01, v, height);
	glm::vec3 dv = get_surface_position(u, v + 0.01, height) - get_surface_position(u, v - 0.01, height);

	return glm::normalize(glm::cross(du, dv));
}

void Level::initializeLevelTopology()
{
	std::cout << "Initializing surface...";

	std::vector<Vertex> vertexes((levelResolution + 1) * (levelResolution + 1));
	//Bej�rjuk az egyes mez�k bal-fels� sarkait:
	for (int tileGridZ = 0; tileGridZ < levelSize; ++tileGridZ) {		//0-24
		for (int tileGridX = 0; tileGridX < levelSize; ++tileGridX) {	//0-24
			float currentTileCenterU = (tileGridX * tileResolution + (tileResolution / 2)) / (float)levelResolution;
			float currentTileCenterV = (tileGridZ * tileResolution + (tileResolution / 2)) / (float)levelResolution;
			float hillRadius = tileRadius;
			//Majd bej�rjuk a mez�ket k�pz� 10*10-es fel�letdarabokat:
			for (int currentTileZ = tileGridZ * tileResolution; currentTileZ <= tileGridZ * tileResolution + 10; currentTileZ++) {		//0-250, 10-es etappokban
				for (int currentTileX = tileGridX * tileResolution; currentTileX <= tileGridX * tileResolution + 10; currentTileX++) {	//0-250, 10-es etappokban
					//�s a mez� ter�let�re legener�ljuk a dombot:
					//uv koordin�t�k teljes laphoz k�pest -> Tile poz�ci� k�sz norm�lvektoraik
					float tileGridU = currentTileX / (float)levelResolution;
					float tileGridV = currentTileZ / (float)levelResolution;
					//uv koordin�t�k adott mez�h�z k�pest -> text�rakoordin�t�k
					float currentTileU = (currentTileX - tileGridX * tileResolution) / (float)tileResolution;
					float currentTileV = (currentTileZ - tileGridZ * tileResolution) / (float)tileResolution;
					//Soron k�vetkez� Tile be�ll�t�sa:
					float distanceFromTileCenter = sqrt(pow(currentTileCenterU - tileGridU, 2) + pow(currentTileCenterV - tileGridV, 2));
					float currentTileHeight = 0;
					if (distanceFromTileCenter >= hillRadius) {
						currentTileHeight = 0;
					}
					else {
						float hillHeight = levelTilesData[tileGridZ][tileGridX]->getHeight();
						float basicHill = abs(sin((1 - (distanceFromTileCenter / hillRadius)) * (M_PI / 2)));
						currentTileHeight = hillHeight * basicHill;
					}
					vertexes[currentTileZ + currentTileX * (levelResolution + 1)].position = get_surface_position(tileGridU, tileGridV, currentTileHeight);
					vertexes[currentTileZ + currentTileX * (levelResolution + 1)].normal = get_surface_normal(tileGridU, tileGridV, currentTileHeight);
					if (currentTileZ % tileResolution == 0 || currentTileX % tileResolution == 0) {
						vertexes[currentTileZ + currentTileX * (levelResolution + 1)].textureUV = glm::vec2(1.0f, 0.0f);
					} else {
						vertexes[currentTileZ + currentTileX * (levelResolution + 1)].textureUV = glm::vec2(currentTileU, currentTileV);
					}
				}
			}
		}
	}

	std::vector<int> indices(3 * 2 * levelResolution * levelResolution);
	for (int i = 0; i < levelResolution; ++i)
		for (int j = 0; j < levelResolution; ++j)
		{
			indices[6 * i + j * 3 * 2 * levelResolution + 0] = (i)+(j) * (levelResolution + 1);
			indices[6 * i + j * 3 * 2 * levelResolution + 1] = (i + 1) + (j) * (levelResolution + 1);
			indices[6 * i + j * 3 * 2 * levelResolution + 2] = (i)+(j + 1) * (levelResolution + 1);
			indices[6 * i + j * 3 * 2 * levelResolution + 3] = (i + 1) + (j) * (levelResolution + 1);
			indices[6 * i + j * 3 * 2 * levelResolution + 4] = (i + 1) + (j + 1) * (levelResolution + 1);
			indices[6 * i + j * 3 * 2 * levelResolution + 5] = (i)+(j + 1) * (levelResolution + 1);
		}

	surfaceVBO.BufferData(vertexes);
	surfaceIndices.BufferData(indices);
	surfaceVAO.Init(
		{
			//Vertex poz�ci�k kiolvas�sa:
			{CreateAttribute<0,				//Ezt a VAO csatorn�t haszn�lod.
							glm::vec3,		//Ilyen t�pus� adatot akarsz kiolvasni.
							0,				//Itt kezd�dik a mem�ri�ban.
							sizeof(Vertex)>,//Ennyi t�vols�gra tal�lhat� a k�vetkez�.
							surfaceVBO},	//Ebb�l a VBO-b�l olvasol.
			{CreateAttribute<1, glm::vec3, sizeof(glm::vec3)    , sizeof(Vertex)>, surfaceVBO},	//Vertex norm�lvektorok kiolvas�sa
			{CreateAttribute<2, glm::vec2, 2 * sizeof(glm::vec3), sizeof(Vertex)>, surfaceVBO}	//Vertex text�rakoordin�t�j�nak kiolvas�sa
		}, surfaceIndices
	);

	std::cout << "Done." << std::endl << std::endl;
}

void Level::render(glm::mat4& viewMatrix) {
	//Parametrikus fel�let:
	glm::mat4 surfaceWorld = glm::scale<float>(glm::vec3(scale, 1, scale));
	surfaceVAO.Bind();
	surfaceProgram.Use();
	surfaceProgram.SetTexture("grassTexture", 1, grassTexture);
	surfaceProgram.SetTexture("hillTexture", 2, hillTexture);
	if (buildModeOn) {
		//Mez� kiemel�s�hez sz�ks�ges adatok:
		surfaceProgram.SetUniform("buildModeOn", 1);
		surfaceProgram.SetUniform("selectedTileUpperLeftCorner", selectedTile->getUpperLeftCornerPosition());
		surfaceProgram.SetUniform("selectedTileUpperRightCorner", selectedTile->getUpperRightCornerPosition());
		surfaceProgram.SetUniform("selectedTileLowerLeftCorner", selectedTile->getLowerLeftCornerPosition());
		surfaceProgram.SetUniform("selectedTileLowerRightCorner", selectedTile->getLowerRightCornerPosition());
		surfaceProgram.SetUniform("canBuildHere", (canBuildHere ? 1 : 0));
	}
	else {
		surfaceProgram.SetUniform("buildModeOn", 0);
	}
	surfaceProgram.SetUniform("MVP", viewMatrix * surfaceWorld);
	surfaceProgram.SetUniform("world", surfaceWorld);
	surfaceProgram.SetUniform("worldIT", glm::inverse(glm::transpose(surfaceWorld)));
	glDrawElements(GL_TRIANGLES, levelResolution * levelResolution * 2 * 3, GL_UNSIGNED_INT, nullptr); //N�gyzetek sz�ma * 6 Tile-et renderel�nk
	surfaceProgram.Unuse();
}