#include <string>
#include <fstream>

#pragma once

class RpakFileStream
{
public:
    static std::string readCStringFromFile(std::ifstream *fileStream);
    static std::vector<std::string> readPathsFromFile(std::ifstream *fileStream, int size);
};