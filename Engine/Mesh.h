#pragma once

#include "Vertex.h"

#include <vector>

namespace Engine
{
	class Mesh
	{
	public:
		Mesh(void);
		~Mesh(void);

		void Init(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		void Render();
	private:
		unsigned int vboId, iboId;
	};
}


