//Framebuffer object:
//A kamera elé egy 3D-s lapot teszünk, amire rátesszük a jelenet renderelt képét textúraként.
//Ebbõl a textúrából aztán kiolvashatjuk a pixelekben tárolt adatokat (nem csak színt) vagy további post-processing effekteket hajthatunk végre a textúrán.

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

class FrameBufferObject
{
private:
	// Postprocess shaderek programja
	ProgramObject	programPostprocess;
	// FBO-hoz szükséges dolgok
	bool   frameBufferCreated{ false };
	GLuint depthBuffer;
	GLuint colorBuffer;
	GLuint frameBuffer;
	GLuint dataBuffer;

public:
	//Init fázis:
	FrameBufferObject(int windowWidth, int windowHeight);
	//FrameBufferObject újra inicializálása az ablak átméretezésekor:
	void CreateFrameBuffer(int windowWidth, int windowHeight);
	//Egér eseménykezelése:
	void readPixels(int mouseX, int mouseY, int windowHeight, glm::vec3* selectedSpot);
	//Render fázis:
	void load();
	void applyPostProcessingEffects();
	//Cleanup fázis:
	void clean();
};

