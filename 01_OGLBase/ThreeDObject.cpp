#include "ThreeDObject.h"

ThreeDObject::ThreeDObject() {
	this->ownMesh = nullptr;
	this->meshScale = glm::vec3(0.0f);
	this->meshPosition = glm::vec3(0.0f);
	this->meshOrientation = glm::vec3(0.0f);
}

glm::vec3 ThreeDObject::getMeshPosition() {
	return meshPosition;
}

glm::vec3 ThreeDObject::getMeshOrientation() {
	return meshOrientation;
}

void ThreeDObject::render(glm::mat4 viewProj) {
	//Elforgatjuk, méretezzük, és végül elhelyezzük a mesh-et:
	glm::mat4 ThreeDObjectWorld = glm::translate<float>(meshPosition) * glm::scale<float>(meshScale) * glm::rotate<float>(meshOrientation.x, glm::vec3(1,0,0)) * glm::rotate<float>(meshOrientation.y, glm::vec3(0, 1, 0)) * glm::rotate<float>(meshOrientation.z, glm::vec3(0, 0, 1));
	ownMesh->render(viewProj, ThreeDObjectWorld);
}