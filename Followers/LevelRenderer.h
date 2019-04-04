#pragma once

#include "Level.h"
#include "Engine/CameraSpectator.h"
#include "Engine/GLSLProgram.h"
#include "Engine/Sun.h"

class LevelRenderer
{
public:
	LevelRenderer(const Level *level);
	~LevelRenderer(void);

	void Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun);
private:
	void buildModel(const Level *level);
	void initShader(const Level *level);

	unsigned int vboId, iboId;
	unsigned int iboSize;

	Engine::GLSLProgram *program;
};

