#include "Sun.h"

#include "Vertex.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define PI 3.14159265358979323846264338327950288

const  glm::vec3 OX(1.0f, 0.0f, 0.0f);
const  glm::vec3 OY(0.0f, 1.0f, 0.0f);

namespace Engine
{
	Sun::Sun(void) : program(nullptr), position(glm::vec3(100, 1500, 0))
	{
		buildModel();
	}


	Sun::~Sun(void)
	{
		if (program)
			delete program;
	}

	void Sun::Render(const CameraSpectator &camera)
	{
		if (!program)
			initShaders();

		program->Use();

		//setting model-view matrix
		glm::vec3 rotation = camera.GetRotation();
		glm::mat4 mvp = glm::translate(camera.GetCameraMatrix(),position);
		mvp = glm::rotate(mvp, -rotation.y, OY);
		mvp = glm::rotate(mvp, -rotation.x, OX);
		glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);

		//render sun
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

		program->UnUse();
	}

	void Sun::buildModel()
	{
		const unsigned int nVertices = 32;
		SimpleVertex center;
		const float radius = 100.0f;

		std::vector<SimpleVertex> vertices(1+nVertices);
		std::vector<unsigned int> indices(nVertices*3);
		vertices[0] = center;

		//generate vertices
		const float slice = 2*PI/nVertices;
		for (unsigned int i=1, iVert=1; i<=nVertices; i++, iVert++)
		{
			float crtSlice = slice*i;
			vertices[iVert].SetPosition(center.position.x + cos(crtSlice) * radius, 
				center.position.y + sin(crtSlice) * radius, center.position.z);
		}

		//generate indices
		unsigned int k=0;
		for (unsigned int i=1; i<=nVertices; i++)
		{
			indices[k++] = 0;
			indices[k++] = i;
			indices[k++] = i+1;
		}
		//close circle
		indices[k-1] = 1;

		//upload to GPU
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}

	void Sun::initShaders()
	{
		program = new GLSLProgram;

		//links shaders
		program->CompileShaders("shaders/sunShading.vert","shaders/sunShading.frag");
		program->AddAttribute("vertexPosition");
		program->LinkShader();

		program->Use();

		program->UnUse();
	}

}
