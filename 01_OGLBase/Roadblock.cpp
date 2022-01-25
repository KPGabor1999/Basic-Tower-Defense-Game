#include "Roadblock.h"

Roadblock::Roadblock(Level* level, int lineIndex, int columnIndex, MeshObject* ownMesh): GameObject(level, lineIndex, columnIndex) {
	loadMesh(ownMesh);
}

void Roadblock::loadMesh(MeshObject* ownMesh) {
	this->ownMesh = ownMesh;
}