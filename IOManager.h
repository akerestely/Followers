#pragma once

#include <vector>

class IOManager
{
public:
	static bool ReadFileToBuffer(char* filepath, std::vector<char>& buffer);
};

