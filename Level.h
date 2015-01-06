#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Engine/Vertex.h"

class Level
{
public:
	Level(const std::string &fileName);

	void SwitchWireframeVisibility();

	void Render();

	double GetHeight(glm::vec2 point2d);

	~Level(void);
private:
	void readBinaryData(const std::string &fileName);
	void readAscFile(const std::string &fileName);
	void writeToBinary(const std::string &fileName,unsigned int nCols=0U, unsigned int nRows=0U);

	Engine::ColorRGBA8 getColorByHeight(float height);
	Engine::Position normal(Engine::Position p[3]);

	unsigned int nCols, nRows;
	float *levelData;

	unsigned int vboId,iboId;
	unsigned int vboIdWireframe,iboIdWireframe;
	bool showWireframe;

	std::map<float,Engine::ColorRGBA8> heightColorMap;
};

