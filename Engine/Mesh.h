#pragma once

#include "Vertex.h"

#include <vector>

namespace Engine
{
	class Mesh
	{
		friend class Model;
	public:
		Mesh(void);
		~Mesh(void);

		void Init(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialId);
	private:
		unsigned int vboId, iboId, materialId;
	};
}


