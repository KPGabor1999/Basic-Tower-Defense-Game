#pragma once

#include "ThreeDObject.h"
#include "Enemy.h"

class CannonBall : public ThreeDObject
{
private:
	int damage;
	glm::vec3 startingPosition;	//A torony helye, ahonnan kilõtték.
	Enemy* target;				//A célpontra mutató pointer (sebzéshez).
	glm::vec3 targetPosition;	//Az ellenség pozíciója.
	glm::vec3 targetDirection;	//Az ellenség felé mutató irányvektor.
	float targetDistance;		//Milyen távol van az ellenség.

public:
	CannonBall(int damage, glm::vec3 startingPosition, Enemy* target, MeshObject* ownMesh);
	glm::vec3 calculateTargetPosition();
	glm::vec3 calculateTargetDirection();
	float calculateDistance(glm::vec3 startingPosition, glm::vec3 targetPosition);
	virtual void loadMesh(MeshObject* mesh);
	bool move(float gameSpeed);
	void hurt(Enemy* target);
};

