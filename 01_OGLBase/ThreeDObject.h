#pragma once

//Ismernie kell a p�ly�t
#include "MeshObject.h"

class ThreeDObject		//�jrahasznos�that� seg�doszt�ly bet�lt�tt mesh-ek transzform�l�s�hoz �s renderel�s�hez.
{
protected:
	MeshObject* ownMesh;			//A mesh-et bet�lt� objektum.
	glm::vec3 meshScale;			//A mesh m�retez�se az xyz tengelyeken.
	glm::vec3 meshPosition;			//A mesh poz�ci�ja a 3D-s t�rben.
	glm::vec3 meshOrientation;		//A mesh elforgat�sai az xyz tengelyeken.

public:
	ThreeDObject();
	virtual void loadMesh(MeshObject* mesh) = 0;
	glm::vec3 getMeshPosition();
	glm::vec3 getMeshOrientation();
	//J�hetne m�g egy getMeshScale is?
	virtual void render(glm::mat4 viewProj);
};

