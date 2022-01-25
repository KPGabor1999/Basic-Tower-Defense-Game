#include "CannonBall.h"

CannonBall::CannonBall(int damage, glm::vec3 startingPosition, Enemy* target, MeshObject* ownMesh) {
	this->damage = damage;
	this->startingPosition = startingPosition;
	this->meshScale = glm::vec3(0.5f);
	this->meshPosition = this->startingPosition;
	this->target = target;

	//this->targetPosition = this->target->getMeshPosition();
	this->targetPosition = calculateTargetPosition();

	//std::cout << "Target position: (" << targetPosition.x << "," << targetPosition.y << "," << targetPosition.z << ")" << std::endl;
	this->targetDirection = calculateTargetDirection();
	//std::cout << "Target direction: (" << targetDirection.x << "," << targetDirection.y << "," << targetDirection.z << ")" << std::endl;
	this->targetDistance = calculateDistance(this->startingPosition, this->targetPosition);
	//std::cout << "Target distance: " << targetDistance << std::endl;
	loadMesh(ownMesh);
}

glm::vec3 CannonBall::calculateTargetPosition() {
	//Azt a mezõt lõjük, ami felé éppen siklik:
	return target->getMeshPosition() + target->getMovementDirection();
}

glm::vec3 CannonBall::calculateTargetDirection() {
	this->targetPosition = target->getMeshPosition();
	return targetPosition - meshPosition;
}

float CannonBall::calculateDistance(glm::vec3 startingPosition, glm::vec3 targetPosition) {
	return sqrt(pow(targetPosition.x - startingPosition.x, 2) + pow(targetPosition.y - startingPosition.y, 2) + pow(targetPosition.z - startingPosition.z, 2));
}

void CannonBall::loadMesh(MeshObject* mesh) {	//Flyweight tervminta!
	this->ownMesh = mesh;
}

bool CannonBall::move(float gameSpeed) {
	//Újraszámoljuk az ellenség irányát:
	//std::cout << "Moving towards target: " << std::endl;

	//Majd felé irányítjuk az ágyúgolyót:
	meshPosition += (gameSpeed / 30.0f) * targetDirection;		//Az FPS szám így jó lesz?
	//std::cout << "CannonBall position: (" << meshPosition.x << "," << meshPosition.y << "," << meshPosition.z << ")" << std::endl;

	//Ha már megtettük a torony és az ellenség távolságát, megsebezzük az ellenséget és az ágyúgolyó eltûnik:
	if (calculateDistance(meshPosition, startingPosition) >= targetDistance) {
		hurt(target);
		return false;
	}
	else return true;
}

void CannonBall::hurt(Enemy* target) {
	//std::cout << "CannonBall damage: " << damage << std::endl;
	target->getHurt(damage);
	//std::cout << "Megsebeztunk egy ellenseget, maradt " << target->getHp() << " hp-ja." << std::endl << std::endl;
}