//Framebuffer object:
//A kamera el� egy 3D-s lapot tesz�nk, amire r�tessz�k a jelenet renderelt k�p�t text�rak�nt.
//Ebb�l a text�r�b�l azt�n kiolvashatjuk a pixelekben t�rolt adatokat (nem csak sz�nt) vagy tov�bbi post-processing effekteket hajthatunk v�gre a text�r�n.

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
	// FBO-hoz sz�ks�ges dolgok
	bool   frameBufferCreated{ false };
	GLuint depthBuffer;
	GLuint colorBuffer;
	GLuint frameBuffer;
	GLuint dataBuffer;

public:
	//Init f�zis:
	FrameBufferObject(int windowWidth, int windowHeight);
	//FrameBufferObject �jra inicializ�l�sa az ablak �tm�retez�sekor:
	void CreateFrameBuffer(int windowWidth, int windowHeight);
	//Eg�r esem�nykezel�se:
	void readPixels(int mouseX, int mouseY, int windowHeight, glm::vec3* selectedSpot);
	//Render f�zis:
	void load();
	void applyPostProcessingEffects();
	//Cleanup f�zis:
	void clean();
};

