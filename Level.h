#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Engine/Vertex.h"
#include "Engine/GLSLProgram.h"

class Level
{
public:
	Level(const std::string &fileName, Engine::GLSLProgram *shaderProgram);

	void SwitchWireframeVisibility();

	void Render();

	double GetHeight(glm::vec2 point2d);

	~Level(void);
private:
	void readBinaryData(const std::string &fileName);
	void readAscFile(const std::string &fileName);
	void writeToBinary(const std::string &fileName,unsigned int nCols=0U, unsigned int nRows=0U);

	void getMaxMinHeight(float &maxHeight, float &minHeight);
	unsigned short* makeHeightMap(float &multiplier);

	Engine::ColorRGBA8 getColorByHeight(float height);
	Engine::Position Level::normal(Engine::Position p1, Engine::Position p2, Engine::Position p3);

	Engine::GLSLProgram *shaderProgram;
	GLuint heightMapId;
	unsigned int nCols, nRows;
	float xllcorner, yllcorner, cellsize, noData_value;
	float *levelData;

	unsigned int vboId,iboId;
	unsigned int vboIdWireframe,iboIdWireframe;
	unsigned int vboNormals, iboNormals;
	bool showWireframe;

	std::map<float,Engine::ColorRGBA8> heightColorMap;
};

