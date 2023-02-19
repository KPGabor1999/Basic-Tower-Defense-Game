#pragma once

#include "ThreeDObject.h"
#include "Enemy.h"

class CannonBall : public ThreeDObject
{
private:
	int damage;
	glm::vec3 startingPosition;	//A torony helye, ahonnan kil�tt�k.
	Enemy* target;				//A c�lpontra mutat� pointer (sebz�shez).
	glm::vec3 targetPosition;	//Az ellens�g poz�ci�ja.
	glm::vec3 targetDirection;	//Az ellens�g fel� mutat� ir�nyvektor.
	float targetDistance;		//Milyen t�vol van az ellens�g.

public:
	CannonBall(int damage, glm::vec3 startingPosition, Enemy* target, MeshObject* ownMesh);
	glm::vec3 calculateTargetPosition();
	glm::vec3 calculateTargetDirection();
	float calculateDistance(glm::vec3 startingPosition, glm::vec3 targetPosition);
	virtual void loadMesh(MeshObject* mesh);
	bool move(float gameSpeed);
	void hurt(Enemy* target);
};

