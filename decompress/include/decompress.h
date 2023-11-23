#include <string>
#include <filesystem>

#include "common.h"
#include "rtech.h"

class Decompress
{
public:
    static std::filesystem::path processFile(std::filesystem::path filePath, std::filesystem::path outputDir);
};