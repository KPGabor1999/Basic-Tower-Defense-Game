#pragma once

#include "Tower.h"

class StrongTower : public Tower
{
public:
	static int towerLevel;
	static float reloadTime;	//seconds
	static int damage;
	const static int range = 8;		//tiles
	const static int buildingCost = 20;
	static int upgradeCost;

public:
	StrongTower(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh);
	virtual void fire(float gameSpeed, float deltaTime, std::vector<Enemy*>& enemies, std::vector<CannonBall*>& cannonBalls, MeshObject* cannonBallMesh);
	static bool isUpgradable();
	static bool upgrade();
	static void reset();

private:
	virtual void loadMesh(MeshObject* ownMesh);
	void initStats();
};