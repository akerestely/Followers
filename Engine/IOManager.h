#pragma once

#include <vector>
#include <string>

namespace Engine
{
	class IOManager
	{
	public:
		static bool ReadFileToBuffer(char* filepath, std::vector<unsigned char>& buffer);

		// Extract the directory part from the file path
		static std::string GetPath(const std::string filePath);
		// Extract the extension part from the file path
		static std::string GetFileExtension(const std::string filePath);
	};
}

