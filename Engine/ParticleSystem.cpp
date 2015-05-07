#include "ParticleSystem.h"

#include "ModelLoader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Engine
{
	const  glm::vec3 OY(0.0f, 1.0f, 0.0f);

	ParticleSystem::ParticleSystem(char *filePath)
	{
		model = ModelLoader::LoadModelRawAssimp(filePath, vertices, indices);
	}

	
	ParticleSystem::~ParticleSystem(void)
	{
		//empty
	}

	void ParticleSystem::Add(glm::vec3 &position, float &rotateY, float &scale)
	{
		//bring down on y, to cover eventual fails
		position.y -= 5;
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, rotateY, OY);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		transforms.push_back(modelMatrix);
	}

	Model* ParticleSystem::Finalize()
	{
		std::vector<Vertex> finalVertices;
		finalVertices.reserve(vertices.size() * transforms.size());
		std::vector<unsigned int> finalIndices;
		finalIndices.reserve(indices.size() * transforms.size());

		for(unsigned int iMesh = 0; iMesh < model->meshes.size(); iMesh++)
		{
			Mesh *mesh = &model->meshes[iMesh];
			for(unsigned int iTransform = 0; iTransform < transforms.size(); iTransform++)
			{
				for (unsigned int iIndex = 0; iIndex < mesh->nIndices; iIndex++)
				{
					unsigned int crtIndex = iIndex + mesh->baseIndex;
					Vertex crtVertex = vertices[crtIndex];
					transform(crtVertex.position, transforms[iTransform]);
					finalVertices.push_back(crtVertex);
					finalIndices.push_back(finalVertices.size()-1);
				}
 			}
			if(mesh->baseIndex != 0)
			{
				mesh->baseIndex = model->meshes[iMesh-1].nIndices + model->meshes[iMesh-1].baseIndex;
				mesh->baseVertex = model->meshes[iMesh-1].nVertices + model->meshes[iMesh-1].baseVertex;
			}
			mesh->nIndices = finalIndices.size();
			mesh->nVertices = finalVertices.size();
		}

		ModelLoader::UploadData(model, finalVertices, finalIndices);
		printf("Particle with: %d components!\n", transforms.size());
		return model;
	}

	void ParticleSystem::transform(Position &pos, glm::mat4 &modelMatrix)
	{
		glm::vec4 glmPos(pos.x, pos.y, pos.z, 1.0f);
		glmPos = modelMatrix * glmPos;
		pos.x = glmPos.x;
		pos.y = glmPos.y;
		pos.z = glmPos.z;
	}
}
