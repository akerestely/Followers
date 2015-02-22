#pragma once

#include "Mesh.h"

namespace Engine
{
	class ModelLoader
	{
	public:
		static Mesh LoadAssimp(char* filePath);
	};
}

