#include "Water.h"
#include "Vertex.h"
#include "ResourceMngr.h"

#include <glm/gtc/matrix_transform.hpp>

const unsigned int nRows = 300;
const unsigned int nCols = 300;
const float cellSize = 100;

const float waveWidth = 200.6f;
const float waveHeight = 5.0f;

namespace Engine
{
	Water::Water(void) :
		program(nullptr)
	{
		buildModel();
	}

	Water::~Water(void)
	{
		if (program)
			delete program;
	}

	void Water::Render(const CameraSpectator &camera, float deltaTime)
	{
		if(!program)
			initShader();

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, ResourceMngr::GetTexture("Textures/water.jpg").id);
		program->Use();
		glUniform1f(program->GetUniformLocation("time"), deltaTime);
		glm::vec3 negativeCamPos = glm::vec3(0.0, 1300, 0.0);
		glm::mat4 mvp = glm::translate(camera.GetCameraMatrix(), negativeCamPos);
		glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		program->UnUse();
		glDisable(GL_BLEND);
	}

	void Water::buildModel()
	{
		//compute vertices
		SimpleVertex *vertices = new SimpleVertex[nRows*nCols];
		int halfRow = nRows/2;
		int halfCol = nCols/2;
		for (unsigned int i=0; i<nRows; i++)
			for (unsigned int j=0; j<nCols; j++)
			{
				vertices[i*nCols + j].SetPosition((-halfCol + (int)j) * cellSize, 0, (-halfRow + (int)i) * cellSize);
			}

		//compute indices
		unsigned int *indices = new unsigned int[(nCols-1)*(nRows-1)*2*3];
		for(unsigned int i=0,k=0; i<nRows-1; i++)
			for(unsigned int j=0; j<nCols-1; j++)
			{
				indices[k++] = i*nCols + j;
				indices[k++] = (i+1)*nCols + j;
				indices[k++] = i*nCols + j+1;

				indices[k++] = i*nCols + j+1;
				indices[k++] = (i+1)*nCols + j;
				indices[k++] = (i+1)*nCols + j+1;
			}

		//upload to GPU
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex)*nRows*nCols, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(nCols-1)*(nRows-1)*2*3, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Water::initShader()
	{
		program = new GLSLProgram;

		//links shader
		program->CompileShaders("shaders/waterShading.vert","shaders/waterShading.frag");
		program->AddAttribute("vertexPosition");
		program->LinkShader();

		program->Use();
		glUniform1f(program->GetUniformLocation("waveWidth"), waveWidth);
		glUniform1f(program->GetUniformLocation("waveHeight"), waveHeight);
		glUniform1i(program->GetUniformLocation("waterTexture"), 0);
		glUniform1f(program->GetUniformLocation("fogDistance"), nRows * cellSize / 2);
		program->UnUse();
	}
}
