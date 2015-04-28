#pragma once

#include "AnimModel.h"

namespace Engine
{
	class ModelLoader
	{
	public:
		static AnimModel* LoadAnimModelAssimp(char* filePath);
		static Model* LoadModelAssimp(char* filePath);
	private:
		static void loadMaterials(Model *model, const aiScene *scene, const char* filePath);

		//load vertex an indices data to GPU, and bind the ids to model
		template <class vertex_type>
		static void uploadData(Model *model, std::vector<vertex_type> &vertices, std::vector<unsigned int> &indices)
		{
			glGenBuffers(1, &model->vboId);
			glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &model->iboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}
	};
}