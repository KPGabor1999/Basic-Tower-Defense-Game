#pragma once

#include "GameObject.h"
#include "Enemy.h"
#include "CannonBall.h"

class Tower : public GameObject
{
protected:
	int zRangeStart;
	int zRangeEnd;
	int xRangeStart;
	int xRangeEnd;

	float timeSinceLastShot = 0;		//Mennyi idõ telt el az elõzõ tüzelés óta.

public:
	Tower(Level* level, int lineIndex, int columnIndex);
	virtual void fire(float gameSpeed, float deltaTime, std::vector<Enemy*>& enemies, std::vector<CannonBall*>& cannonBalls, MeshObject* cannonBallMesh) = 0;

protected:
	virtual void initStats() = 0;
	bool inRange(Enemy* enemy) const;
	bool enemyInSight(Enemy* enemy) const;
};