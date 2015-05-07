#pragma once

#include "Model.h"
#include "Vertex.h"

#include <glm/glm.hpp>

#include <vector>

namespace Engine
{
	class ParticleSystem
	{
	public:
		ParticleSystem(char *filePath);
		~ParticleSystem(void);

		void Add(glm::vec3 &position, float &rotateY, float &scale);
		Model* Finalize();
	private:
		void transform(Position &pos, glm::mat4 &modelMatrix);

		Model *model;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::mat4> transforms;
	};
}

