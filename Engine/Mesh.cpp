#include "Mesh.h"

namespace Engine
{
	Mesh::Mesh(void)
	{
		//empty
	}


	Mesh::~Mesh(void)
	{
		//empty
	}

	void Mesh::Init(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialId)
	{
		this->materialId = materialId;

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
}
