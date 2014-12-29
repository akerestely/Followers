#include "Level.h"

#include <fstream>

#include "Engine/Errors.h"
#include <stddef.h>

const int ROW = 200;
const int COL = 200;
const double CELL_SIZE = 1;

Level::Level(const std::string &fileName) : nCols(0), nRows(0), levelData(nullptr)
{
	//initialization
	vboId = iboId = vboIdWireframe = iboIdWireframe = 0;
	showWireframe = false;

	//read binary data from file
	readBinaryData(fileName);

	//build color height
	heightColorMap.insert(std::make_pair(0.0f, Engine::ColorRGBA8()));
	heightColorMap.insert(std::make_pair(1100.0f, Engine::ColorRGBA8(0,150,150)));
	heightColorMap.insert(std::make_pair(1500.0f, Engine::ColorRGBA8(0,180,0)));
	heightColorMap.insert(std::make_pair(1700.0f, Engine::ColorRGBA8(255,255,0)));
	heightColorMap.insert(std::make_pair(1900.0f, Engine::ColorRGBA8(255,120,0)));
	heightColorMap.insert(std::make_pair(2100.0f, Engine::ColorRGBA8(180,31,21)));
	heightColorMap.insert(std::make_pair(2300.0f, Engine::ColorRGBA8(128,60,60)));
	heightColorMap.insert(std::make_pair(2600.0f, Engine::ColorRGBA8(255,255,255)));
	heightColorMap.insert(std::make_pair(3500.0f, Engine::ColorRGBA8(255,255,255,100)));

	//construct vertex data based on level data
	Engine::Vertex *vertexData = new Engine::Vertex[ ROW * COL ];
	for(int z=0; z<ROW; z++)
		for (int x=0; x<COL; x++)
		{
			float y = levelData[z*nRows + x];
			vertexData[z*ROW + x].SetPosition((x)*CELL_SIZE, y/10 - 230, (z)*CELL_SIZE);
			vertexData[z*ROW + x].color = getColorByHeight(y);
		}
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Engine::Vertex)*ROW * COL, vertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set color for wire frame vertex, and upload to GPU
	Engine::ColorRGBA8 wireframeColor(0,0,0,255);
	for(int z=0; z<ROW; z++)
		for (int x=0; x<COL; x++)
		{
			//vertexData[z*ROW + x].position.y += 0.01;
			vertexData[z*ROW + x].color = wireframeColor;
		}
	glGenBuffers(1, &vboIdWireframe);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdWireframe);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Engine::Vertex)*ROW * COL,vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertexData;

	//assign triangle indices. Two triangles at once, that form a rectangle.
	unsigned int *mapElements = new unsigned int[(COL-1)*(ROW-1)*2*3];
	for(int i=0,k=0; i<ROW-1; i++)
		for(int j=0; j<COL-1; j++)
		{
			mapElements[k++] = i*COL + j;
			mapElements[k++] = i*COL + j+1;
			mapElements[k++] = (i+1)*COL + j;

 			mapElements[k++] = i*COL + j+1;
 			mapElements[k++] = (i+1)*COL + j;
 			mapElements[k++] = (i+1)*COL + j+1;
		}
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(COL-1)*(ROW-1)*2*3, mapElements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] mapElements;

	//assign horizontal line indices.
	unsigned int *wireframeElements = new unsigned int[(COL-1)*(ROW-1)*2*3*2];
	for(int i=0,k=0; i<ROW-1; i++)
		for(int j=0; j<COL-1; j++)
		{
			wireframeElements[k++] = i*COL + j;
			wireframeElements[k++] = i*COL + j+1;

			wireframeElements[k++] = i*COL + j+1;
			wireframeElements[k++] = (i+1)*COL + j;

			wireframeElements[k++] = (i+1)*COL + j;
			wireframeElements[k++] = i*COL + j;

			wireframeElements[k++] = i*COL + j+1;
			wireframeElements[k++] = (i+1)*COL + j;

			wireframeElements[k++] = (i+1)*COL + j;
			wireframeElements[k++] = (i+1)*COL + j+1;

			wireframeElements[k++] = (i+1)*COL + j+1;
			wireframeElements[k++] = i*COL + j+1;
		}
	glGenBuffers(1, &iboIdWireframe);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIdWireframe);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(COL-1)*(ROW-1)*2*3*2, wireframeElements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] wireframeElements;

	printf("Uploaded map!\n");
}

Level::~Level(void)
{
	delete[] levelData;
}

void Level::SwitchWireframeVisibility()
{
	showWireframe = !showWireframe;
}

void Level::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,uv));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

	if(showWireframe)
	{
		glPolygonOffset(1, 1);
		glEnable(GL_POLYGON_OFFSET_FILL);

		glBindBuffer(GL_ARRAY_BUFFER, vboIdWireframe);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,uv));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIdWireframe);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_LINES, size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	}

	//glDrawArrays(GL_TRIANGLES, 0, /*(ROW-1) **/ (COL-2));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

double Level::GetHeight(glm::vec2 point2d)
{
	int posx1 = (int)(point2d.x / CELL_SIZE);
	int posx2 = posx1 + 1;
	int posy1 = (int)(point2d.y / CELL_SIZE);
	int posy2 = posy1 + 1;

	double f00 = levelData[posy1*nRows + posx1];
	double f10 = levelData[posy1*nRows + posx2];
	double f11 = levelData[posy2*nRows + posx2];
	double f01 = levelData[posy2*nRows + posx1];

	double px = point2d.x / CELL_SIZE - posx1;
	double py = point2d.y / CELL_SIZE - posy2;

	double fFinal = f00 * (1.0 - px) * (1.0 - py) +
		f10 * px * (1.0 - py) + f01 * (1.0 - px) * py + f11 * px * py;

	return fFinal;
}

void Level::readAscFile(const std::string &fileName)
{
	std::ifstream file(fileName);
	if(file.fail())
		Engine::fatalError("File could not be opened!");

	std::string temp; ///temporary string

	file>>temp>>nCols; ///ignore word and read nCols

	file>>temp>>nRows; ///ignore word and read nRows

	for (int i=0; i < 5; i++)
		std::getline(file,temp); ///ignore next 4 rows

	//allocate memory
	levelData = new float[nRows*nCols];
	//read actual matrix data
	for (int y=0; y<nRows; ++y)
	{
		const int FINISHED_ELEMENTS = y*nCols;
		for (int x=0; x<nCols; ++x)
			file>>levelData[FINISHED_ELEMENTS + x];
	}
}

void Level::writeToBinary(const std::string &fileName, unsigned int nCols, unsigned int nRows)
{
	std::ofstream file(fileName,std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");
	if(!nCols) nCols = this->nCols;
	if(!nRows) nRows = this->nRows;

	file.write((char*)&nCols,sizeof(nCols));
	file.write((char*)&nRows,sizeof(nRows));

	for(int i=0; i<nRows; i++)
		file.write((char*)&levelData[i*this->nCols],sizeof(float)*nCols);
}

Engine::ColorRGBA8 Level::getColorByHeight(float height)
{
	//Get the first element that is greater than or equal with height. This will be the upper bound. Seriously.
	auto itLow = heightColorMap.lower_bound(height);
	//Now make the upper bound equal to lower bound(witch is still the upper bound), then decrement the upper bound.
	auto itUp = itLow--;

	float height1=itLow->first, height2=itUp->first;
	Engine::ColorRGBA8 c1=itLow->second, c2=itUp->second, cRes;
	float t = (height - height1) / (height2 - height1);
	cRes.r = (c2.r - c1.r) * t + c1.r;
	cRes.g = (c2.g - c1.g) * t + c1.g;
	cRes.b = (c2.b - c1.b) * t + c1.b;
	cRes.a = (c2.a - c1.a) * t + c1.a;
	return cRes;
}

void Level::readBinaryData(const std::string &fileName)
{
	//data from file
	std::ifstream file(fileName,std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");
	file.read((char*)&nCols,sizeof(nCols));
	file.read((char*)&nRows,sizeof(nRows));

	levelData = new float[nRows*nCols];
	file.read((char*)levelData,sizeof(float)*nRows*nCols);
}
