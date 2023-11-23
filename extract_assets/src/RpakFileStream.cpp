#include "RpakFileStream.h"

std::string RpakFileStream::readCStringFromFile(std::ifstream *fileStream)
{
    // Read characters until null terminator is encountered
    char currentChar;
    std::string dynamicString;

    while (fileStream->get(currentChar) && currentChar != '\0')
    {
        dynamicString += currentChar;
    }

    return dynamicString;
};

std::vector<std::string> RpakFileStream::readPathsFromFile(std::ifstream *fileStream, int size)
{
    std::vector<std::string> paths;
    int i = size;

    while (i > 0)
    {
        auto path = readCStringFromFile(fileStream);

        if (path.size() > 0)
        {
            paths.push_back(path);
        }

        i -= (path.size() + 1);
    }

    return paths;
}