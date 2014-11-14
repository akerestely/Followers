#pragma once

#include <vector>

namespace Engine
{
	class IOManager
	{
	public:
		static bool ReadFileToBuffer(char* filepath, std::vector<unsigned char>& buffer);
	};
}

