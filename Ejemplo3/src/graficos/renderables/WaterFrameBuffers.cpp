#include "WaterFrameBuffers.h"

#include "platform/Application.h"

#include <GL/glew.h>

WaterFrameBuffers::WaterFrameBuffers() {
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

WaterFrameBuffers::~WaterFrameBuffers() {
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFrameBuffers::bindReflectionFrameBuffer() {
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer() {
	bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetApplication().GetWindowWidth(), Application::GetApplication().GetWindowHeight());
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer() {
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer() {
	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(int frameBuffer, unsigned int width, unsigned int height) {
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#include <iostream>

unsigned int WaterFrameBuffers::createFrameBuffer() {
	unsigned int frameBuffer = 0; 
	glGenFramebuffers(1, &frameBuffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frameBuffer;
}

unsigned int WaterFrameBuffers::createTextureAttachment(unsigned int width, unsigned int height) {
	unsigned int texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		texture, 0);
	return texture;
}

unsigned int WaterFrameBuffers::createDepthTextureAttachment(unsigned int width, unsigned int height) {
	unsigned int texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		texture, 0);
	return texture;
}

unsigned int WaterFrameBuffers::createDepthBufferAttachment(unsigned int width, unsigned int height) {
	unsigned int depthBuffer = 0;

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
		height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}