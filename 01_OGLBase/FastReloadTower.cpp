#include "FastReloadTower.h"

constexpr auto FASTRELOADTOWER_STARTING_LEVEL = 1;
constexpr auto FASTRELOADTOWER_STARTING_RELOAD_TIME = 1.0f;
constexpr auto FASTRELOADTOWER_STARTING_DAMAGE = 2;
constexpr auto FASTRELOADTOWER_STARTING_UPGRADECOST = 300;

int FastReloadTower::towerLevel = FASTRELOADTOWER_STARTING_LEVEL;
float FastReloadTower::reloadTime = FASTRELOADTOWER_STARTING_RELOAD_TIME;
int FastReloadTower::damage = FASTRELOADTOWER_STARTING_DAMAGE;
int FastReloadTower::upgradeCost = FASTRELOADTOWER_STARTING_UPGRADECOST;

FastReloadTower::FastReloadTower(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh) : Tower(level, lineIndex, columnIndex) {
	loadMesh(ownMesh);
	initStats();
}

void FastReloadTower::fire(float gameSpeed, float deltaTime, std::vector<Enemy*>& enemies, std::vector<CannonBall*>& cannonBalls, MeshObject* cannonBallMesh) {
	if (timeSinceLastShot >= reloadTime) {		//Ha már betöltöttek az ágyúk:

		Enemy* target = nullptr;
		int targetIndex;

		//A célpont az a ellenség, aki a torony hatókörén belül a legközelebb áll a célhoz (ha van ilyen):
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

		//Ha találtunk célpontot, kilövünk rá egy ágyúgolyót:
		if (target != nullptr) {
			cannonBalls.push_back(new CannonBall(damage, meshPosition, target, cannonBallMesh));
		}

		//Visszaállítjuk az eltelt idõt:
		timeSinceLastShot -= reloadTime;
	}
	else {
		//Ha még nem töltöttünk újra, várunk:
		timeSinceLastShot += gameSpeed * deltaTime;
	}
}

bool FastReloadTower::isUpgradable() {
	return towerLevel < 4;
}

bool FastReloadTower::upgrade() {
	towerLevel++;
	if (towerLevel % 2 == 0) {
		damage *= 2;
	} else reloadTime /= 2;

	upgradeCost *= towerLevel;

	std::cout << "FastReloadTowers upgraded to level " << FastReloadTower::towerLevel << "!" << std::endl;
	std::cout << "Damage: " << FastReloadTower::damage << std::endl;
	std::cout << "Reload time: " << FastReloadTower::reloadTime << std::endl;
	if (towerLevel < 4) std::cout << "Upgrade cost: " << FastReloadTower::upgradeCost << std::endl;
	std::cout << std::endl;

	return true;
}

void FastReloadTower::reset() {
	FastReloadTower::towerLevel = FASTRELOADTOWER_STARTING_LEVEL;
	FastReloadTower::reloadTime = FASTRELOADTOWER_STARTING_RELOAD_TIME;
	FastReloadTower::damage = FASTRELOADTOWER_STARTING_DAMAGE;
	FastReloadTower::upgradeCost = FASTRELOADTOWER_STARTING_UPGRADECOST;
}

void FastReloadTower::loadMesh(MeshObject* ownMesh) {
	this->ownMesh = ownMesh;
}

void FastReloadTower::initStats() {
	this->zRangeStart = getLineIndex() - range;
	this->zRangeEnd = getLineIndex() + range;
	this->xRangeStart = getColumnIndex() - range;
	this->xRangeEnd = getColumnIndex() + range;
}