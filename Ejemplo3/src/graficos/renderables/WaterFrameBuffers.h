#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"

class WaterFrameBuffers {
private:
	const unsigned int REFLECTION_WIDTH = 320;
	const unsigned int REFLECTION_HEIGHT = 180;

	const unsigned int REFRACTION_WIDTH = 1280;
	const unsigned int REFRACTION_HEIGHT = 720;

	unsigned int reflectionFrameBuffer;
	unsigned int reflectionTexture;
	unsigned int reflectionDepthBuffer;

	unsigned int refractionFrameBuffer;
	unsigned int refractionTexture;
	unsigned int refractionDepthTexture;
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();

	void unbindCurrentFrameBuffer();

	inline unsigned int getReflectionTexture() { return reflectionTexture; }
	inline unsigned int getRefractionTexture() { return refractionTexture; }
	inline unsigned int getRefractionDepthTexture() { return refractionDepthTexture; }

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int frameBuffer, unsigned int width, unsigned int height);
	unsigned int createFrameBuffer();
	unsigned int createTextureAttachment(unsigned int width, unsigned int height);
	unsigned int createDepthTextureAttachment(unsigned int width, unsigned int height);
	unsigned int createDepthBufferAttachment(unsigned int width, unsigned int height);
};