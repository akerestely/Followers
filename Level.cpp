#include "Level.h"

#include <fstream>
#include "Engine/Errors.h"

#define PI (float)3.14159265358979323846264338327950288
#define kMERIDIAN_LENGTH_METERS   20003930.0
#define kMERIDIAN_LENGTH_WGS_UNITS   ( 180.0 * 100000 )
#define kWGS_UNIT_TO_METER   ( kMERIDIAN_LENGTH_METERS / kMERIDIAN_LENGTH_WGS_UNITS )
#define kMETER_TO_WGS_UNIT   ( 1.0 / kWGS_UNIT_TO_METER )

Level::Level(const std::string &fileName) : nCols(0), nRows(0), levelData(nullptr)
{
	//read binary data from file
	readBinaryData(fileName);

	//convert meters to WGS units
	for(int z=0; z<nRows; z++)
		for (int x=0; x<nCols; x++)
			levelData[z*nCols + x] *= kMETER_TO_WGS_UNIT;

	printf("Uploaded %s map!\n", fileName.c_str());

	//calculate cos meridian
	cosMeridian = cos(yllcorner*PI/180);

	computeMaxMinHeight();
}

Level::~Level(void)
{
	delete[] levelData;
}

const float Level::GetHeight(glm::vec2 point2d) const
{
	point2d.x = point2d.x / CELL_SIZE / cosMeridian;
	point2d.y = point2d.y / CELL_SIZE;

	int posx1 = (int)point2d.x;
	int posx2 = posx1 + 1;
	int posy1 = (int)point2d.y;
	int posy2 = posy1 + 1;

	//build points
	glm::vec3 p1((float)posx1, levelData[posy1*nCols + posx1], (float)posy1);
	glm::vec3 p2((float)posx1, levelData[posy2*nCols + posx1], (float)posy2);
	glm::vec3 p3((float)posx2, levelData[posy2*nCols + posx2], (float)posy2);
	glm::vec3 p4((float)posx2, levelData[posy1*nCols + posx2], (float)posy1);
	
	//verify in what triangle is the point, the upper or the lower
	if(glm::distance(glm::vec2(p2.x, p2.z), point2d) > glm::distance(glm::vec2(p4.x, p4.z), point2d))
		p2 = p4;

	//calculate height based on Barycentric coordinates
	//see:http://en.wikipedia.org/wiki/Barycentric_coordinate_system 
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

	float l1 = ((p2.z - p3.z) * (point2d.x - p3.x) + (p3.x - p2.x) * (point2d.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (point2d.x - p3.x) + (p1.x - p3.x) * (point2d.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

void Level::computeMaxMinHeight()
{
	maxHeight = minHeight = levelData[0];
	for (int i=0; i<ROW; i++)
		for(int j=0; j<COL; j++)
		{
			float x = levelData[i*nCols + j];
			if(x<minHeight)
				minHeight = x;
			if(x>maxHeight)
				maxHeight = x;
		}
}

void Level::readAscFile(const std::string &fileName)
{
	std::ifstream file(fileName);
	if(file.fail())
		Engine::fatalError("File could not be opened!");

	std::string temp; ///temporary string

	file>>temp>>nCols; ///ignore word and read nCols
	file>>temp>>nRows; ///ignore word and read nRows
	file>>temp>>xllcorner; ///ignore word and read xllcorner
	file>>temp>>yllcorner; ///ignore word and read yllcorner
	file>>temp>>cellsize; ///ignore word and read cellsize
	file>>temp>>noData_value; ///ignore word and read NODATA_value

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
	file.write((char*)&xllcorner,sizeof(xllcorner));
	file.write((char*)&yllcorner,sizeof(yllcorner));
	file.write((char*)&cellsize,sizeof(cellsize));
	file.write((char*)&noData_value,sizeof(noData_value));

	for(int i=0; i<nRows; i++)
		file.write((char*)&levelData[i*this->nCols],sizeof(float)*nCols);
}
void Level::readBinaryData(const std::string &fileName)
{
	//data from file
	std::ifstream file(fileName,std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");
	file.read((char*)&nCols,sizeof(nCols));
	file.read((char*)&nRows,sizeof(nRows));
	file.read((char*)&xllcorner,sizeof(xllcorner));
	file.read((char*)&yllcorner,sizeof(yllcorner));
	file.read((char*)&cellsize,sizeof(cellsize));
	file.read((char*)&noData_value,sizeof(noData_value));

	levelData = new float[nRows*nCols];
	file.read((char*)levelData,sizeof(float)*nRows*nCols);
}
