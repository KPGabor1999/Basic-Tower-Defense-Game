#pragma once

#include "GameObject.h"
#include "Tile.h"

class Enemy : public GameObject
{
public:
	static int startingHp;
	static int startingReward;
private:
	float hp;
	double distanceFromExit;
	enum Orientation {NORTH, EAST, SOUTH, WEST};
	Orientation faceDirection;
	glm::vec3 movementDirection;
	bool alive;
	bool passed;
	int reward;

public:
	Enemy(Level* level, MeshObject* ownMesh);

	double getDistanceFromExit();
	void setDistanceFromExit(float distanceFromExit);
	void getHurt(int amount);
	int getHp();
	bool isAlive();
	bool passedThrough();

	void stepForward();
	void setFaceDirection(Tile* currentTile, Tile* nextTile);
	void normalizePositionTo(Tile* nextTile);
	void setMovementDirection(Tile* currentTile, Tile* nextTile);
	glm::vec3 getMovementDirection();
	int getReward();

	void move(float stepInterval, float gameSpeed);
	static void upgrade();
	static void reset();

private:
	virtual void loadMesh(MeshObject* ownMesh);
};

