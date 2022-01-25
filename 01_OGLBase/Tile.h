#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "TilePosition.h"
#include <iostream>

class Tile
{
private:
	//int lineIndex;
	//int columnIndex;
    TilePosition* tilePosition;
    float height;
    float width;    //nxn-es mezõ
    float radius;
    glm::vec3 center;
	glm::vec3 upperLeftCornerPosition;
	glm::vec3 upperRightCornerPosition;
	glm::vec3 lowerLeftCornerPosition;
	glm::vec3 lowerRightCornerPosition;
	bool has_tower;
	bool has_roadblock;
	bool has_been_visited;
	Tile* parent;
	double distanceFromExit;
public:
	Tile();
    Tile(int lineIndex, int columnIndex, float height, float width, float radius);

    int getLineIndex() const;
    void setLineIndex(int lineIndex);

    int getColumnIndex() const;
    void setColumnIndex(int columnIndex);

    float getHeight() const;
    void setHeight(float height);

    float getWidth() const;
    void setWidth(float width);

    float getRadius() const;
    void setRadius(float radius);

    glm::vec3 getCenter();
    void calculateCenter();

    glm::vec3 getUpperLeftCornerPosition() const;
    void calculateUpperLeftCornerPosition();

    glm::vec3 getUpperRightCornerPosition() const;
    void calculateUpperRightCornerPosition();

    glm::vec3 getLowerLeftCornerPosition() const;
    void calculateLowerLeftCornerPosition();

    glm::vec3 getLowerRightCornerPosition() const;
    void calculateLowerRightCornerPosition();

    bool hasTower() const;
    void installTower();

    bool hasRoadblock() const;
    void installRoadblock();

    bool hasBeenVisited() const;
    void setVisited(bool hasBeenVisited);

    Tile* getParent() const;
    void setParent(Tile* parent);

    double getDistanceFromExit() const;
    void setDistanceFromExit(double distanceFromExit);

    bool isAHill();
    bool isANormalHill();
    bool isABarrierHill();
    bool isInAnInnerHillRow();
    bool isValidNeighbor();

    void toString();
};

