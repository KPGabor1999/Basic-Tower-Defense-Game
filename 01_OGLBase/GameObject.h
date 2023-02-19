#pragma once

#include "ThreeDObject.h"
#include "Level.h"
#include "TilePosition.h"

class GameObject : public ThreeDObject
{
protected:
	//J�t�kspecifikus ismeretek:
	Level* level;
	TilePosition* tilePosition;	//A mez�, amin �ppen �llunk.

public:
	GameObject(Level* level, int lineIndex, int columnIndex);
	TilePosition* getTilePosition();
	int getLineIndex();
	void setLineIndex(int lineIndex);
	int getColumnIndex();
	void setColumnIndex(int columnIndex);

protected:
	virtual glm::vec3 calculateScale(int actualLevelSize);
	virtual glm::vec3 calculatePosition(Level* level, int lineIndex, int columnIndex);
};

