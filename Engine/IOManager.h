#pragma once

#include <vector>
#include <string>

namespace Engine
{
	class IOManager
	{
	public:
		static bool ReadFileToBuffer(char* filepath, std::vector<unsigned char>& buffer);

		// Extract the directory part from the file name
		static std::string GetPath(std::string filePath);
	};
}

