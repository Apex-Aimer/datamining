#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "decompress.h"

std::filesystem::path Decompress::processFile(std::filesystem::path filePath, std::filesystem::path outputDir)
{
	std::string fileName = filePath.filename().string();
	std::filesystem::path pathUncompressed = outputDir / fileName;

	std::vector<uint8_t> buffer; // Compressed region.
	std::ifstream inputFile(filePath, std::fstream::binary);

	if (!inputFile.is_open())
	{
		std::cerr << "Error code: " << strerror(errno) << std::endl;

		return "";
	}

	/**
	 * Seek to the end of the file to determine its size
	 */
	inputFile.seekg(0, std::fstream::end);
	/**
	 * Get the size and resize
	 */
	std::streampos fileSize = inputFile.tellg();

	buffer.resize(fileSize);
	inputFile.seekg(0, std::fstream::beg);
	inputFile.read((char *)buffer.data(), buffer.size());

	RpakApexHeader *rheader = (RpakApexHeader *)buffer.data();

	if (rheader->magic != 'kaPR')
	{
		std::cout << "Error: pak file '" << fileName << "' has invalid magic!" << std::endl;
		return "";
	}
	if (!rheader->is_compressed)
	{
		std::cout << "Error: pak file '" << fileName << "' already decompressed!" << std::endl;
		return "";
	}
	if (rheader->compressed_size != buffer.size())
	{
		std::cout << "Error: pak file '" << fileName << "' decompressed size '" << buffer.size() << "'!" << std::endl;
		return "";
	}
	// TODO: patching
	if (rheader->patch_index > 0)
	{
		return "";
	}

	std::int64_t params[18];
	uint64_t dsize = RTech::DecompressPakfileInit((std::int64_t)(params), buffer.data(), buffer.size(), 0, sizeof(RpakApexHeader));

	if (dsize == rheader->compressed_size)
	{
		std::cout << "Error: calculated size: '" << dsize << "' expected: '" << rheader->decompressed_size << "'!" << std::endl;
		return "";
	}
	else
	{
		std::cout << "Calculated size: '" << dsize << "'" << std::endl;
	}

	std::vector<uint8_t> pakbuf(rheader->decompressed_size, 0);

	params[1] = std::int64_t(pakbuf.data());
	params[3] = -1LL;

	uint8_t decomp_result = RTech::DecompressPakfile(params, buffer.size(), pakbuf.size());

	if (decomp_result != 1)
	{
		std::cout << "Error: decompression failed for '" << fileName << "' return value: '" << +decomp_result << "'" << std::endl;
		return "";
	}
	else
	{
		std::cout << "Successfuly decompressed" << std::endl;
	}

	rheader->is_compressed = 0x0;
	rheader->compressed_size = rheader->decompressed_size; // Equal compressed size with decompressed

	std::ofstream out_block(pathUncompressed, std::fstream::binary);
	std::ofstream out_header(pathUncompressed, std::fstream::binary);

	out_block.write((char *)pakbuf.data(), rheader->decompressed_size);
	out_header.write((char *)rheader, sizeof(RpakApexHeader));

	return pathUncompressed;
}