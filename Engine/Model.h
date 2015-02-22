#pragma once

#include "Mesh.h"

#include <vector>

namespace Engine
{
	class Model
	{
		friend class ModelLoader;

	public:
		Model(void);
		~Model(void);

		void Render();
	private:
		std::vector<Mesh> meshes;
		std::vector<int> materials;
	};
}


