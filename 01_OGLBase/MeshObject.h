#pragma once

//SZÁMÍT AZ INCLUDE-OLÁSOK SORRENDJE!!!

//GLM: Affin transzformációhoz szükséges eszközök
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Rendereléshez szükséges osztályok
#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

//mesh
#include "includes/ObjParser_OGL3.h"

class MeshObject	//Úrjahasznosítható segédosztály mesh-ek betöltéséhez.
{
private:
	ProgramObject meshProgram;	//A mesh-et renderelõ program.
	std::unique_ptr<Mesh> mesh;	//A konkrét mesh-re mutató pointer.
	Texture2D meshTexture;		//A mesh- textúrája. Egy mesh-nek csak egy textúrája lehet, úgyhogy egy lapon legyen rajta minden.

public:
	MeshObject();
	MeshObject(const char* meshFilepath, const std::string& textureFilepath, const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual void render(glm::mat4 viewProj, glm::mat4 currentMeshWorld);
};

