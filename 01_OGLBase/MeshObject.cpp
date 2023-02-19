#include "MeshObject.h"

MeshObject::MeshObject() {
	this->mesh = nullptr;
	this->meshTexture = nullptr;
}

MeshObject::MeshObject(const char* meshFilepath, const std::string& textureFilepath, const char* vertexShaderFilename, const char* fragmentShaderFilename) {
	//mesh bet�llt�se
	mesh = std::unique_ptr<Mesh>(ObjParser::parse(meshFilepath));	//Itt a mappastrukt�r�t �t lehetne �rni -> k�l�m assets/meshes �s asstes/textures mappa.
	mesh->initBuffers();	//Itt �ll�tja be a VAO-t �s a VBO-t. Ezt b�v�tsd ki, hogy a vertexek sz�neit is kiolvassa.

	//text�ra bet�lt�se
	meshTexture.FromFile(textureFilepath);

	//shaderek bet�lt�se
	meshProgram.Init(
		{
			{ GL_VERTEX_SHADER, std::string::basic_string(vertexShaderFilename) },
			{ GL_FRAGMENT_SHADER, std::string::basic_string(fragmentShaderFilename) }
		},
		{
			{ 0, "vs_in_pos" },		// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },	// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" },		// VAO 2-es csatorna menjen a vs_in_tex-be
			// VAO 3-as csatorna menjen a vs_in_col-ba
		}
	);
}

void MeshObject::render(glm::mat4 viewProj, glm::mat4 mesh_world) {	//A transzform�ci�s m�trixokat k�v�lr�l kapja
	meshProgram.Use();
	meshProgram.SetTexture("meshTexture", 0, meshTexture);
	//*Majd k�s�bb �ltal�nos�tsd a heightMap-ek bet�lt�s�t is.*
	meshProgram.SetUniform("MVP", viewProj * mesh_world);
	meshProgram.SetUniform("world", mesh_world);
	meshProgram.SetUniform("worldIT", glm::inverse(glm::transpose(mesh_world)));
	mesh->draw();
	meshProgram.Unuse();
}