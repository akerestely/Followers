#include "LevelRenderer.h"
#include "Engine/ResourceMngr.h"
#include "Engine/Vertex.h"

#define PI (float)3.14159265358979323846264338327950288

LevelRenderer::LevelRenderer(const Level *level)
{
	buildModel(level);
	initShader(level);
}

LevelRenderer::~LevelRenderer(void)
{
	if(program)
		delete program;
}

void LevelRenderer::Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Engine::ResourceMngr::GetTexture("Textures/sand.jpg").id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Engine::ResourceMngr::GetTexture("Textures/grass.png").id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Engine::ResourceMngr::GetTexture("Textures/rock.jpg").id);

	program->Use();
	//update uniforms
	glUniform3fv(program->GetUniformLocation("lightPos"), 1, &sun->GetPosition()[0]);
	glUniform3fv(program->GetUniformLocation("lightColor"), 1, &sun->GetSunColor()[0]);
	glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &camera.GetCameraMatrix()[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,position));
	glVertexAttribPointer(1, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,uv));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	program->UnUse();
}

void LevelRenderer::buildModel(const Level *level)
{
	const float* levelData = level->GetData();

	//construct vertex data based on level data
	Engine::Vertex *vertices = new Engine::Vertex[ ROW * COL ];
	double cosMeridian = cos(level->GetYLLCorner()*PI/180);
	for(int z=0; z<ROW; z++)
		for (int x=0; x<COL; x++)
		{
			float y = levelData[z*level->GetCols() + x];
			vertices[z*COL + x].SetPosition(x*CELL_SIZE*cosMeridian, y, z*CELL_SIZE);
			vertices[z*COL + x].SetUV(x,z);
		}

	//calculate vertex normals based on triangles formed with adjacent vertexes
	//interior
	for(int z=1; z<ROW-1; z++)
		for (int x=1; x<COL-1; x++)
		{
			Engine::Position leftUpper = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[(z-1)*ROW + x].position, vertices[z*ROW + x-1].position);
			Engine::Position centerUpper = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[(z-1)*ROW + x+1].position, vertices[(z-1)*ROW + x].position);
			Engine::Position rightUpper = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[z*ROW + x+1].position, vertices[(z-1)*ROW + x+1].position);
			Engine::Position rightLower = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[(z+1)*ROW + x].position, vertices[z*ROW + x+1].position);
			Engine::Position centerLower = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[(z+1)*ROW + x-1].position, vertices[(z+1)*ROW + x].position);
			Engine::Position leftLower = Engine::Position::Normal(vertices[z*ROW + x].position, vertices[z*ROW + x-1].position, vertices[(z+1)*ROW + x-1].position);
			vertices[z*ROW + x].normal = (leftUpper + centerUpper + rightUpper + rightLower + centerLower + leftLower);
			vertices[z*ROW + x].normal.Normalize();
		}
	//TODO corners
	//first and last row
	//first and last column

	//assign triangle indices. Two triangles at once, that form a rectangle.
	unsigned int *indices = new unsigned int[(COL-1)*(ROW-1)*2*3];
	for(int i=0,k=0; i<ROW-1; i++)
		for(int j=0; j<COL-1; j++)
		{
			indices[k++] = i*COL + j;
			indices[k++] = (i+1)*COL + j;
			indices[k++] = i*COL + j+1;

			indices[k++] = i*COL + j+1;
			indices[k++] = (i+1)*COL + j;
			indices[k++] = (i+1)*COL + j+1;
		}

	//upload to GPU
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Engine::Vertex) * ROW * COL, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertices;

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(COL-1)*(ROW-1)*2*3, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indices;
}

void LevelRenderer::initShader(const Level *level)
{
	program = new Engine::GLSLProgram;

	program->CompileShaders("shaders/terrainShading.vert", "shaders/terrainShading.frag");
	program->AddAttribute("vertexPosition");
	program->AddAttribute("vertexNormal");
	program->AddAttribute("vertexUV");
	program->LinkShader();

	program->Use();
	glUniform1i(program->GetUniformLocation("mySampler0"), 0);
	glUniform1i(program->GetUniformLocation("mySampler1"), 1);
	glUniform1i(program->GetUniformLocation("mySampler2"), 2);

	glUniform1f(program->GetUniformLocation("maxHeight"), level->GetMaxHeight());
	glUniform1f(program->GetUniformLocation("minHeight"), level->GetMinHeight());
	program->UnUse();
}
