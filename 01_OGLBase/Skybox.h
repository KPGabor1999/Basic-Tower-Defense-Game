#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

class Skybox
{
private:
	ProgramObject		skyboxProgram;
	VertexArrayObject	skyboxVao;
	IndexBuffer			skyboxIndices;
	ArrayBuffer			skyboxVBO;
	TextureCubeMap		skyboxTexture;
public:
	Skybox(
		const std::string& xposFilepath,
		const std::string& xnegFilepath,
		const std::string& yposFilepath,
		const std::string& ynegFilepath,
		const std::string& zposFilepath,
		const std::string& znegFilepath);	//Paraméterként add meg a képfájlok neveit.
	void render(glm::mat4 viewProj, glm::vec3 eyePosition);
};

