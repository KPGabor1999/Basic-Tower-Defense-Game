#include "StrongTower.h"

constexpr auto STRONGTOWER_STARTING_LEVEL = 1;
constexpr auto STRONGTOWER_STARTING_RELOAD_TIME = 3.0f;
constexpr auto STRONGTOWER_STARTING_DAMAGE = 10;
constexpr auto STRONGTOWER_STARTING_UPGRADECOST = 400;

int StrongTower::towerLevel = STRONGTOWER_STARTING_LEVEL;
float StrongTower::reloadTime = STRONGTOWER_STARTING_RELOAD_TIME;
int StrongTower::damage = STRONGTOWER_STARTING_DAMAGE;
int StrongTower::upgradeCost = STRONGTOWER_STARTING_UPGRADECOST;

StrongTower::StrongTower(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh) : Tower(level, lineIndex, columnIndex) {
	loadMesh(ownMesh);
	initStats();
}

void StrongTower::fire(float gameSpeed, float deltaTime, std::vector<Enemy*>& enemies, std::vector<CannonBall*>& cannonBalls, MeshObject* cannonBallMesh) {
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

bool StrongTower::isUpgradable() {
	return towerLevel < 4;
}

bool StrongTower::upgrade() {
	towerLevel++;
	if (towerLevel % 2 == 0) {
		damage *= 2;
	} else reloadTime /= 2;

	upgradeCost *= towerLevel;

	std::cout << "StrongTowers upgraded to level " << StrongTower::towerLevel << "!" << std::endl;
	std::cout << "Damage: " << StrongTower::damage << std::endl;
	std::cout << "Reload time: " << StrongTower::reloadTime << std::endl;
	if (towerLevel < 4) std::cout << "Upgrade cost: " << StrongTower::upgradeCost << std::endl;
	std::cout << std::endl;

	return true;
}

void StrongTower::reset() {
	StrongTower::towerLevel = STRONGTOWER_STARTING_LEVEL;
	StrongTower::reloadTime = STRONGTOWER_STARTING_RELOAD_TIME;
	StrongTower::damage = STRONGTOWER_STARTING_DAMAGE;
	StrongTower::upgradeCost = STRONGTOWER_STARTING_UPGRADECOST;
}

void StrongTower::loadMesh(MeshObject* ownMesh) {
	this->ownMesh = ownMesh;
}

void StrongTower::initStats() {
	this->zRangeStart = getLineIndex() - range;
	this->zRangeEnd = getLineIndex() + range;
	this->xRangeStart = getColumnIndex() - range;
	this->xRangeEnd = getColumnIndex() + range;
}