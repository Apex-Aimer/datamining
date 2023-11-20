#include <iostream>
#include <filesystem>
#include <string>

#include <decompress.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::filesystem::path rpakFilePath(argv[1]);

    if (!std::filesystem::exists(rpakFilePath))
    {
        std::cerr << "File " << argv[1] << " doesn't exist" << std::endl;

        return 1;
    }

    Decompress::processFile(std::filesystem::absolute(rpakFilePath));

    return 0;
}