#pragma once

#include "Model.h"

namespace Engine
{
	class ModelLoader
	{
	public:
		static Model* LoadAssimp(char* filePath);
	};
}

