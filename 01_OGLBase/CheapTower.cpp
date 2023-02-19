#include "CheapTower.h"

constexpr auto CHEAPTOWER_STARTING_LEVEL = 1;
constexpr auto CHEAPTOWER_STARTING_RELOAD_TIME = 2.0f;
constexpr auto CHEAPTOWER_STARTING_DAMAGE = 3;
constexpr auto CHEAPTOWER_STARTING_UPGRADECOST = 100;

int CheapTower::towerLevel = CHEAPTOWER_STARTING_LEVEL;
float CheapTower::reloadTime = CHEAPTOWER_STARTING_RELOAD_TIME;
int CheapTower::damage = CHEAPTOWER_STARTING_DAMAGE;
int CheapTower::upgradeCost = CHEAPTOWER_STARTING_UPGRADECOST;

CheapTower::CheapTower(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh): Tower(level, lineIndex, columnIndex) {
	loadMesh(ownMesh);
	initStats();
}

void CheapTower::fire(float gameSpeed, float deltaTime, std::vector<Enemy*>& enemies, std::vector<CannonBall*>& cannonBalls, MeshObject* cannonBallMesh) {
	if (timeSinceLastShot >= reloadTime) {		//Ha m�r bet�lt�ttek az �gy�k:

		Enemy* target = nullptr;
		int targetIndex;

		//A c�lpont az a ellens�g, aki a torony hat�k�r�n bel�l a legk�zelebb �ll a c�lhoz (ha van ilyen):
		for (int currentIndex = 0; currentIndex < enemies.size(); ++currentIndex) {
			Enemy* currentEnemy = enemies[currentIndex];
			//std::cout << "Enemy in range: " << inRange(currentEnemy) << std::endl << std::endl;
			if (inRange(currentEnemy)) {
				if (target == nullptr) {
					target = currentEnemy;
					targetIndex = currentIndex;
				}
				else if (currentEnemy->getDistanceFromExit() < target->getDistanceFromExit()) {
					target = currentEnemy;
					targetIndex = currentIndex;
				}
			}
		}

		//Ha tal�ltunk c�lpontot, kil�v�nk r� egy �gy�goly�t:
		if (target != nullptr) {
			cannonBalls.push_back(new CannonBall(damage, meshPosition, target, cannonBallMesh));
			//Vissza�ll�tjuk az eltelt id�t:
			timeSinceLastShot -= reloadTime;
		}
	}
	else {
		//Ha m�g nem t�lt�tt�nk �jra, v�runk:
		timeSinceLastShot += gameSpeed * deltaTime;
	}
}

bool CheapTower::isUpgradable() {
	return towerLevel < 4;
}

bool CheapTower::upgrade() {
	towerLevel++;
	if (towerLevel % 2 == 0) {
		damage *= 2;
	} else reloadTime /= 2;

	upgradeCost *= towerLevel;

	std::cout << "CheapTowers upgraded to level " <<  CheapTower::towerLevel << "!" << std::endl;
	std::cout << "Damage: " << CheapTower::damage << std::endl;
	std::cout << "Reload time: " << CheapTower::reloadTime << std::endl;
	if (towerLevel < 4) std::cout << "Upgrade cost: " << CheapTower::upgradeCost << std::endl;
	std::cout << std::endl;

	return true;
}

void CheapTower::reset() {
	CheapTower::towerLevel = CHEAPTOWER_STARTING_LEVEL;
	CheapTower::reloadTime = CHEAPTOWER_STARTING_RELOAD_TIME;
	CheapTower::damage = CHEAPTOWER_STARTING_DAMAGE;
	CheapTower::upgradeCost = CHEAPTOWER_STARTING_UPGRADECOST;
}

void CheapTower::loadMesh(MeshObject* ownMesh) {
	this->ownMesh = ownMesh;
}

void CheapTower::initStats() {
	this->zRangeStart = getLineIndex() - range;
	this->zRangeEnd = getLineIndex() + range;
	this->xRangeStart = getColumnIndex() - range;
	this->xRangeEnd = getColumnIndex() + range;
}