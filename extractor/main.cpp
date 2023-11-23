#include <iostream>
#include <filesystem>
#include <string>

#include <decompress.h>
#include <RpakFile.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::filesystem::path passedPath(argv[1]);
    std::filesystem::path rpaksDir = std::filesystem::absolute(passedPath);

    if (!std::filesystem::exists(rpaksDir))
    {
        std::cerr << "Directory " << argv[1] << " doesn't exist" << std::endl;

        return 1;
    }

    std::filesystem::path rootDir = rpaksDir.parent_path();

    std::filesystem::path tempDir = rootDir / ".tmp";
    if (!std::filesystem::exists(tempDir))
    {
        std::filesystem::create_directory(tempDir);
    }

    std::filesystem::path outputDir = rootDir / "output";
    if (!std::filesystem::exists(outputDir))
    {
        std::filesystem::create_directory(outputDir);
    }

    for (const auto &entry : std::filesystem::directory_iterator(rpaksDir))
    {
        std::filesystem::path rpakFilePath = entry.path();

        if (!std::filesystem::exists(rpakFilePath))
        {
            std::cerr << "File " << argv[1] << " doesn't exist" << std::endl;

            return 1;
        }

        std::cout << "Decompressing: " << rpakFilePath.filename().string() << std::endl;
        std::filesystem::path decompressedRpakFile = Decompress::processFile(rpakFilePath, tempDir);
        std::cout << "Decompressed" << std::endl;

        if (decompressedRpakFile.string().size() == 0)
        {
            continue;
        }

        RpakFile file = RpakFile(decompressedRpakFile, outputDir);

        std::cout << "Parsing: " << decompressedRpakFile << std::endl;
        file.parse();
        std::cout << "Parsed file" << std::endl;

        std::cout << "Extracting assets for: " << rpakFilePath.filename().string() << std::endl;
        file.extractAssets();
        std::cout << "Extracted assets" << std::endl;
    }

    std::filesystem::remove_all(tempDir);

    return 0;
}