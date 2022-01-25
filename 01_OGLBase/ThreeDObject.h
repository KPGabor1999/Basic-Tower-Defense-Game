#pragma once

//Ismernie kell a pályát
#include "MeshObject.h"

class ThreeDObject		//Újrahasznosítható segédosztály betöltött mesh-ek transzformálásához és rendereléséhez.
{
protected:
	MeshObject* ownMesh;			//A mesh-et betöltõ objektum.
	glm::vec3 meshScale;			//A mesh méretezése az xyz tengelyeken.
	glm::vec3 meshPosition;			//A mesh pozíciója a 3D-s térben.
	glm::vec3 meshOrientation;		//A mesh elforgatásai az xyz tengelyeken.

public:
	ThreeDObject();
	virtual void loadMesh(MeshObject* mesh) = 0;
	glm::vec3 getMeshPosition();
	glm::vec3 getMeshOrientation();
	//Jöhetne még egy getMeshScale is?
	virtual void render(glm::mat4 viewProj);
};

