#include "Skybox.h"

Skybox::Skybox(
	const std::string& xposFilepath, 
	const std::string& xnegFilepath,
	const std::string& yposFilepath,
	const std::string& ynegFilepath,
	const std::string& zposFilepath,
	const std::string& znegFilepath) {
	skyboxVBO.BufferData(
		std::vector<glm::vec3>{
			// hátsó lap
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
			// elülsõ lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),
		}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	skyboxIndices.BufferData(
		std::vector<int>{
		// hátsó lap
		0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	skyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, skyboxVBO },
		}, skyboxIndices
		);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	skyboxTexture.AttachFromFile(xposFilepath, true, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	skyboxTexture.AttachFromFile(xnegFilepath, true, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	skyboxTexture.AttachFromFile(yposFilepath, true, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	skyboxTexture.AttachFromFile(ynegFilepath, true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	skyboxTexture.AttachFromFile(zposFilepath, true, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	skyboxTexture.AttachFromFile(znegFilepath, true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// a GL_TEXTURE_MAG_FILTER-t és a GL_TEXTURE_MIN_FILTER-t beállítja az AttachFromFile
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	skyboxProgram.Init(
		{
			{ GL_VERTEX_SHADER, "shaders/Skybox.vert" },	//Ezek lehetnek konstansok.
			{ GL_FRAGMENT_SHADER, "shaders/Skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },		// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

void Skybox::render(glm::mat4 viewProj, glm::vec3 eyePosition) {
	// skybox
	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	skyboxVao.Bind();
	skyboxProgram.Use();
	skyboxProgram.SetUniform("MVP", viewProj * glm::translate<float>(eyePosition));

	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	skyboxProgram.SetCubeTexture("skyboxTexture", 0, skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	skyboxProgram.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);

	//Akirajzolás végén kapcsoljuk ki a skybox VAO-ját:
	glBindVertexArray(0);
}