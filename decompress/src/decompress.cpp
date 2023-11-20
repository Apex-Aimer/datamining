#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "decompress.h"

void Decompress::processFile(std::string filePath)
{
	std::filesystem::path pathObj(filePath);
	std::string fileName = pathObj.filename().string();

	std::cout << fileName << std::endl;

	std::string uncompressedFileName = fileName + ".uncompressed";
	std::filesystem::path pathUncompressed = pathObj.replace_filename(uncompressedFileName);

	std::vector<uint8_t> buffer; // Compressed region.
	std::ifstream inputFile(pathObj, std::fstream::binary);

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
		return;
	}
	if (!rheader->is_compressed)
	{
		std::cout << "Error: pak file '" << fileName << "' already decompressed!" << std::endl;
		return;
	}
	if (rheader->compressed_size != buffer.size())
	{
		std::cout << "Error: pak file '" << fileName << "' decompressed size '" << buffer.size() << "'!" << std::endl;
		return;
	}

	RpakDecompressionState state;
	uint64_t dsize = RTech::DecompressPakfileInit(&state, buffer.data(), buffer.size(), 0, sizeof(RpakApexHeader));

	if (dsize == rheader->compressed_size)
	{
		std::cout << "Error: calculated size: '" << dsize << "' expected: '" << rheader->decompressed_size << "'!" << std::endl;
		return;
	}
	else
	{
		std::cout << "Calculated size: '" << dsize << "'" << std::endl;
	}

	std::vector<uint8_t> pakbuf(rheader->decompressed_size, 0);
	uint8_t decomp_result = RTech::DecompressPakfile(&state, buffer.size(), pakbuf.size());

	if (decomp_result != 1)
	{
		std::cout << "Error: decompression failed for '" << fileName << "' return value: '" << +decomp_result << "'" << std::endl;
		return;
	}

	rheader->is_compressed = 0x0;
	rheader->compressed_size = rheader->decompressed_size; // Equal compressed size with decompressed

	std::ofstream out_block(pathUncompressed, std::fstream::binary);
	std::ofstream out_header(pathUncompressed, std::fstream::binary);

	out_block.write((char *)pakbuf.data(), state.decompressed_size);
	out_header.write((char *)rheader, sizeof(RpakApexHeader));
}