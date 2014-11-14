#include "IOManager.h"
#include <fstream>

namespace Engine
{
	bool IOManager::ReadFileToBuffer(char* filepath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (file.fail())
		{
			perror(filepath);
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		int fileSize = (int) file.tellg();
		file.seekg(0, std::ios::beg);

		//reduce the file size by any header bytes that might be present
		fileSize -= (int) file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		return true;
	}
}