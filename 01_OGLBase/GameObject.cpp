#include "GameObject.h"

GameObject::GameObject(Level* level, int lineIndex, int columnIndex): ThreeDObject() {
	this->level = level;
	this->tilePosition = new TilePosition(lineIndex, columnIndex);

	this->meshScale = calculateScale(level->getLevelSize());
	this->meshPosition = calculatePosition(level, lineIndex, columnIndex);
}

TilePosition* GameObject::getTilePosition() {
	return tilePosition;
}

int GameObject::getLineIndex() {
	return tilePosition->getLineIndex();
}

void GameObject::setLineIndex(int lineIndex) {
	tilePosition->setLineIndex(lineIndex);
}

int GameObject::getColumnIndex() {
	return tilePosition->getColumnIndex();
}

void GameObject::setColumnIndex(int columnIndex) {
	tilePosition->setColumnIndex(columnIndex);
}

glm::vec3 GameObject::calculateScale(int actualLevelSize) {
	float towerScale = 25.0f / actualLevelSize;	//A pálya normális méretéhez képest növeljük vagy csökkentjük.
	//std::cout << "Object scale: " << towerScale << std::endl;
	return glm::vec3(towerScale);
}

glm::vec3 GameObject::calculatePosition(Level* level, int lineIndex, int columnIndex) {
	//Kiszámoljuk a bal felsõ sarok pozícióját, majd eltoljuk x-en és z-a rádiusz-szal, mindezt a méretaránnyal felszorozva.
	float translateX = (columnIndex * level->getTileWidth() + level->getTileRadius());
	float translateY = level->getTileHeight(lineIndex, columnIndex);
	float translateZ = (lineIndex * level->getTileWidth() + level->getTileRadius());

	//Debug:
	//std::cout << "Level scale:  " << level->getScale() << std::endl;
	//std::cout << "Tile size: " << level->getTileWidth() << std::endl;
	//std::cout << "Tile radius:  " << level->getTileRadius() << std::endl;
	//std::cout << "Object position : (" << translateX << "," << translateY << "," << translateZ << ")" << std::endl;

	return glm::vec3(translateX, translateY, translateZ);
}