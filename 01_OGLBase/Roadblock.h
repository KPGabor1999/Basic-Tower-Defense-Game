#pragma once

#include "GameObject.h"

class Roadblock : public GameObject
{
public:
	static const int buildingCost = 15;

public:
	Roadblock(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh);

private:
	int getBuildingCost();
	virtual void loadMesh(MeshObject* ownMesh);
};

