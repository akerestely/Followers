#include "Model.h"
#include "Vertex.h"
#include <stddef.h>

namespace Engine
{
	Model::Model(void)
	{
		//empty
	}

	Model::~Model(void)
	{
		//empty
	}

	void Model::Render()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		for (auto it = meshes.begin(); it != meshes.end(); ++it)
		{
			glBindBuffer(GL_ARRAY_BUFFER, it->vboId);
			glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,position));
			glVertexAttribPointer(1, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,normal));
			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,color));
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex,uv));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->iboId);
			int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

			glBindTexture(GL_TEXTURE_2D, materials[it->materialId]);

			glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
	}
}

