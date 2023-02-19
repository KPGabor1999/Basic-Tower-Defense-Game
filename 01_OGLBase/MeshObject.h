#pragma once

//SZ�M�T AZ INCLUDE-OL�SOK SORRENDJE!!!

//GLM: Affin transzform�ci�hoz sz�ks�ges eszk�z�k
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Renderel�shez sz�ks�ges oszt�lyok
#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

//mesh
#include "includes/ObjParser_OGL3.h"

class MeshObject	//�rjahasznos�that� seg�doszt�ly mesh-ek bet�lt�s�hez.
{
private:
	ProgramObject meshProgram;	//A mesh-et renderel� program.
	std::unique_ptr<Mesh> mesh;	//A konkr�t mesh-re mutat� pointer.
	Texture2D meshTexture;		//A mesh- text�r�ja. Egy mesh-nek csak egy text�r�ja lehet, �gyhogy egy lapon legyen rajta minden.

public:
	MeshObject();
	MeshObject(const char* meshFilepath, const std::string& textureFilepath, const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual void render(glm::mat4 viewProj, glm::mat4 currentMeshWorld);
};

