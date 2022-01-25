#include "Tile.h"

Tile::Tile() {
    this->tilePosition = new TilePosition(0, 0);
    this->height = 0.0f;
    this->width = 0.0f;
    this->radius = 0.0f;
    this->center = glm::vec3(0.0f);
	this->has_tower = false;
	this->has_roadblock = false;
	this->has_been_visited = false;
	this->parent = nullptr;
	this->distanceFromExit = 0.0f;
}

Tile::Tile(int lineIndex, int columnIndex, float height, float width, float radius) {
    this->tilePosition = new TilePosition(lineIndex, columnIndex);
    this->height = height;
    this->width = width;
    this->radius = radius;
    calculateCenter();
    calculateUpperLeftCornerPosition();
    calculateUpperRightCornerPosition();
    calculateLowerLeftCornerPosition();
    calculateLowerRightCornerPosition();
    this->has_tower = false;
    this->has_roadblock = false;
    this->has_been_visited = false;
    this->parent = nullptr;
    this->distanceFromExit = 0.0f;
}

int Tile::getLineIndex() const
{
    return tilePosition->getLineIndex() ;
}

void Tile::setLineIndex(int lineIndex)
{
    tilePosition->setLineIndex(lineIndex);
}

int Tile::getColumnIndex() const
{
    return tilePosition->getColumnIndex();
}

void Tile::setColumnIndex(int columnIndex)
{
    tilePosition->setColumnIndex(columnIndex);
}

float Tile::getHeight() const
{
    return height;
}

void Tile::setHeight(float height)
{
    this->height = height;
}

float Tile::getWidth() const {
    return width;
}

void Tile::setWidth(float width) {
    this->width = width;
}

float Tile::getRadius() const {
    return radius;
}

void Tile::setRadius(float radius) {
    this->radius = radius;
}

glm::vec3 Tile::getCenter() {
    return center;
}

void Tile::calculateCenter() {
    float centerX = (getColumnIndex() + 1) * width - radius;
    float centerY = 0.0f;   //Miért nem mûködik az aktuális magasságra?
    float centerZ = (getLineIndex() + 1) * width - radius;

    this->center = glm::vec3(centerX, centerY, centerZ);
}

glm::vec3 Tile::getUpperLeftCornerPosition() const
{
    return upperLeftCornerPosition;
}

void Tile::calculateUpperLeftCornerPosition(){
    glm::vec3 upperLeftCornerPosition;
    upperLeftCornerPosition.x = getColumnIndex() * width;
    upperLeftCornerPosition.y = 0;
    upperLeftCornerPosition.z = getLineIndex() * width;
    this->upperLeftCornerPosition = upperLeftCornerPosition ;
}

glm::vec3 Tile::getUpperRightCornerPosition() const
{
    return upperRightCornerPosition;
}

void Tile::calculateUpperRightCornerPosition() {
    glm::vec3 upperRightCornerPosition;
    upperRightCornerPosition.x = (getColumnIndex() + 1) * width;
    upperRightCornerPosition.y = 0;
    upperRightCornerPosition.z = getLineIndex() * width;
    this->upperRightCornerPosition = upperRightCornerPosition;
}

glm::vec3 Tile::getLowerLeftCornerPosition() const{
    return lowerLeftCornerPosition;
}

void Tile::calculateLowerLeftCornerPosition() {
    glm::vec3 lowerLeftCornerPosition;
    lowerLeftCornerPosition.x = getColumnIndex() * width;
    lowerLeftCornerPosition.y = 0;
    lowerLeftCornerPosition.z = (getLineIndex() + 1) * width;
    this->lowerLeftCornerPosition = lowerLeftCornerPosition;
}

glm::vec3 Tile::getLowerRightCornerPosition() const
{
    return lowerRightCornerPosition;
}

void Tile::calculateLowerRightCornerPosition() {
    glm::vec3 lowerRightCornerPosition;
    lowerRightCornerPosition.x = (getColumnIndex() + 1) * width;
    lowerRightCornerPosition.y = 0;
    lowerRightCornerPosition.z = (getLineIndex() + 1) * width;
    this->lowerRightCornerPosition = lowerRightCornerPosition;
}

bool Tile::hasTower() const
{
    return has_tower;
}

void Tile::installTower()
{
    this->has_tower = true;
}

bool Tile::hasRoadblock() const
{
    return has_roadblock;
}

void Tile::installRoadblock()
{
    this->has_roadblock = true;
}

bool Tile::hasBeenVisited() const
{
    return has_been_visited;
}

void Tile::setVisited(bool hasBeenVisited)
{
    this->has_been_visited = hasBeenVisited;
}

Tile* Tile::getParent() const
{
    return parent;
}

void Tile::setParent(Tile* parent)
{
    this->parent = parent;
}

double Tile::getDistanceFromExit() const
{
    return distanceFromExit;
}

void Tile::setDistanceFromExit(double distanceFromExit)
{
    this->distanceFromExit = distanceFromExit;
}

bool Tile::isAHill() {
    return this->height > 0.1f;
}

bool Tile::isANormalHill() {
    return this->height >= 0.5f && this->height <= 1.0f;
}

bool Tile::isABarrierHill() {
    return this->height == 2.0f;
}

bool Tile::isInAnInnerHillRow() {
    return getLineIndex() % 2 != 0;
}

bool Tile::isValidNeighbor() {
    return this->getHeight() < 0.1f && !(this->hasRoadblock());
}

void Tile::toString() {
    std::cout << "Tile[" << getLineIndex() << "][" << getColumnIndex() << "]" << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Radius: " << radius << std::endl;
    std::cout << "Center position: (" << center.x << "," << center.y << "," << center.z << ")" << std::endl;
}