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
	//Azt a mez�t l�j�k, ami fel� �ppen siklik:
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
	//�jrasz�moljuk az ellens�g ir�ny�t:
	//std::cout << "Moving towards target: " << std::endl;

	//Majd fel� ir�ny�tjuk az �gy�goly�t:
	meshPosition += (gameSpeed / 30.0f) * targetDirection;		//Az FPS sz�m �gy j� lesz?
	//std::cout << "CannonBall position: (" << meshPosition.x << "," << meshPosition.y << "," << meshPosition.z << ")" << std::endl;

	//Ha m�r megtett�k a torony �s az ellens�g t�vols�g�t, megsebezz�k az ellens�get �s az �gy�goly� elt�nik:
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