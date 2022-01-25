#include "Enemy.h"

constexpr auto ENEMY_STARTING_HP = 10;
constexpr auto ENEMY_STARTING_REWARD = 5;

int Enemy::startingHp = ENEMY_STARTING_HP;
int Enemy::startingReward = ENEMY_STARTING_REWARD;

Enemy::Enemy(Level* level, MeshObject* ownMesh): GameObject(level, 0, level->getLevelSize()/2) {
	loadMesh(ownMesh);
	this->hp = startingHp;

	Tile* currentTile = level->getTileData(tilePosition->getLineIndex(), tilePosition->getColumnIndex());
	Tile* nextTile = currentTile->getParent();
	setFaceDirection(currentTile, nextTile);
	setMovementDirection(currentTile, nextTile);

	this->meshPosition = calculatePosition(level, currentTile->getLineIndex(), currentTile->getColumnIndex());
	this->distanceFromExit = currentTile->getDistanceFromExit();
	this->faceDirection = NORTH;
	this->movementDirection = glm::vec3(0.0f);

	this->alive = true;
	this->passed = false;

	this->reward = startingReward;

	/*
	//Véletlenszerû ellenséglerakás debuggolási célból:
	srand(time(NULL));
	int random_x_position = rand() % level->getLevelSize();
	int random_z_position = rand() % level->getLevelSize();
	this->position = calculatePosition(level, random_z_position, random_x_position);
	*/
}


double Enemy::getDistanceFromExit() {
	return distanceFromExit;
}

void Enemy::setDistanceFromExit(float distanceFromExit) {
	this->distanceFromExit = distanceFromExit;
}

void Enemy::getHurt(int amount) {
	if (hp - amount < 0) {
		hp = 0;
	}
	else {
		hp -= amount;
	}
}

int Enemy::getHp() {
	return hp;
}

bool Enemy::isAlive() {
	return alive;
}

bool Enemy::passedThrough() {
	return passed;
}

void Enemy::stepForward() {
	if (hp == 0) {
		this->alive = false;
		return;
	}

	//Megnézzük, hol állunk most:
	Tile* currentTile = level->getTileData(tilePosition->getLineIndex(), tilePosition->getColumnIndex());
	//Lekérjük a jelenlegi mezõ szülõjét (a soronkövetkezõ mezõt):
	Tile* nextTile = currentTile->getParent();

	if (nextTile->getDistanceFromExit() != 0.0f) {		//Vagy currentTile->parent != nullptr
		//Felé fordulunk:
		setFaceDirection(currentTile, nextTile);
		normalizePositionTo(nextTile);			//Hogy ne csússzon el a mozgás sávja.
		setMovementDirection(currentTile, nextTile);

		//Majd rálépünk:
		setLineIndex(nextTile->getLineIndex());
		setColumnIndex(nextTile->getColumnIndex());
		setDistanceFromExit(nextTile->getDistanceFromExit());
	}
	else {
		this->passed = true;
	}
}

void Enemy::setFaceDirection(Tile* currentTile, Tile* nextTile) {
	if (currentTile->getColumnIndex() == nextTile->getColumnIndex() &&
		currentTile->getLineIndex() == nextTile->getLineIndex() - 1) {
		this->faceDirection = Orientation::NORTH;
		this->meshOrientation.y = 0.0f;
	}
	else if (
		currentTile->getColumnIndex() == nextTile->getColumnIndex() &&
		currentTile->getLineIndex() == nextTile->getLineIndex() + 1) {
		this->faceDirection = Orientation::SOUTH;
		this->meshOrientation.y = M_PI;
	}
	else if (
		currentTile->getColumnIndex() == nextTile->getColumnIndex() - 1 &&
		currentTile->getLineIndex() == nextTile->getLineIndex()) {
		this->faceDirection = Orientation::WEST;
		this->meshOrientation.y = M_PI / 2.0f;
	}
	else if (
		currentTile->getColumnIndex() == nextTile->getColumnIndex() + 1 &&
		currentTile->getLineIndex() == nextTile->getLineIndex()) {
		this->faceDirection = Orientation::EAST;
		this->meshOrientation.y = -(M_PI / 2.0f);
	}
}

void Enemy::normalizePositionTo(Tile* nextTile) {
	if (this->faceDirection == NORTH || this->faceDirection == SOUTH) {
		meshPosition.x = nextTile->getCenter().x;
	}
	else {
		meshPosition.z = nextTile->getCenter().z;
	}
}

void Enemy::setMovementDirection(Tile* currentTile, Tile* nextTile) {
	this->movementDirection = level->getTileCenter(nextTile) - level->getTileCenter(currentTile);
}

glm::vec3 Enemy::getMovementDirection() {
	return movementDirection;
}

int Enemy::getReward() {
	return reward;
}

void Enemy::move(float stepInterval, float gameSpeed) {
	//Majd folyamatosan csúszunk felé:
	float movementSpeed = gameSpeed / (stepInterval * 60.0f);
	this->meshPosition += movementSpeed * movementDirection;
}

void Enemy::upgrade() {
	startingHp *= 1.4f;
	startingReward *= 1.5f;
}

void Enemy::reset() {
	Enemy::startingHp = ENEMY_STARTING_HP;
	Enemy::startingReward = ENEMY_STARTING_REWARD;
}

void Enemy::loadMesh(MeshObject* ownMesh) {
	this->ownMesh = ownMesh;
}