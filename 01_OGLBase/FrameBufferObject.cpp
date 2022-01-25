#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject(int windowWidth, int windowHeight) {
	// postprocess shader - a geometria a kódban van
	programPostprocess.Init({
		{ GL_VERTEX_SHADER,		"shaders/postprocess.vert" },
		{ GL_FRAGMENT_SHADER,	"shaders/postprocess.frag" }
	});

	// FBO - kezdeti
	CreateFrameBuffer(windowWidth, windowHeight);
}

void FrameBufferObject::CreateFrameBuffer(int windowWidth, int windowHeight)
{
	// takarítsunk, ha nem elõször hívják ezt a függvényt
	if (frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &depthBuffer);
		glDeleteTextures(1, &colorBuffer);
		glDeleteFramebuffers(1, &frameBuffer);
	}

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		//std::cout << "Error creating color attachment" << std::endl;
		//exit(1);
	}

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating depth attachment" << std::endl;
		exit(1);
	}

	//Reaktív felületet reprezentáló textúra:
	glGenTextures(1, &dataBuffer);
	glBindTexture(GL_TEXTURE_2D, dataBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, windowWidth, windowHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, dataBuffer, 0);

	// -- Completeness check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Incomplete framebuffer (";
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		std::cout << ")" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	// -- Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	frameBufferCreated = true;
}

void FrameBufferObject::readPixels(int mouseX, int mouseY, int windowHeight, glm::vec3* selectedSpot) {
	//Visszafejtjük, melyik mezõre kattintottunk:
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, (void*)selectedSpot);
}

void FrameBufferObject::load() {
	//FBO kiválasztása
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);
}

void FrameBufferObject::applyPostProcessingEffects() {
	//Visszaállás alapértelmezett FBO-ra:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	programPostprocess.Use();
	programPostprocess.SetTexture("frameTex", 0, colorBuffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FrameBufferObject::clean() {
	if (frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &depthBuffer);
		glDeleteTextures(1, &colorBuffer);
		glDeleteFramebuffers(1, &frameBuffer);
	}
}