#include "Model.h"
#include "Vertex.h"

#include <stddef.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

const  glm::vec3 OY(0.0f, 1.0f, 0.0f);

namespace Engine
{
	Model::Model(void) : program(nullptr), position(glm::vec3(0.0)), rotateY(0)
	{
		//empty
	}

	Model::~Model(void)
	{
		if(program)
			delete program;
	}

	void Model::Render(const CameraSpectator &camera, const Sun *sun)
	{
		if(!program)
			initShader();
		program->Use();
		glm::vec3 sunPos = glm::rotate(sun->GetPosition(), -rotateY, OY);
		glUniform3fv(program->GetUniformLocation("lightPos"), 1, &sunPos[0]);
		glUniform3fv(program->GetUniformLocation("lightColor"), 1, &sun->GetSunColor()[0]);
		glm::mat4 mvp = glm::rotate(camera.GetCameraMatrix(), rotateY, OY);
		mvp = glm::translate(mvp, position);
		glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);

		glActiveTexture(GL_TEXTURE0);
		for (auto it = meshes.begin(); it != meshes.end(); ++it)
		{
			glBindBuffer(GL_ARRAY_BUFFER, it->vboId);
			glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,position));
			glVertexAttribPointer(1, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,uv));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->iboId);
			int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			
			glBindTexture(GL_TEXTURE_2D, materials[it->materialId]);

			glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		}
		program->UnUse();
	}

	void Model::initShader()
	{
		program = new GLSLProgram;

		program->CompileShaders("shaders/modelShading.vert", "shaders/modelShading.frag");
		program->AddAttribute("vertexPosition");
		program->AddAttribute("vertexNormal");
		program->AddAttribute("vertexUV");
		program->LinkShader();
	}
}

