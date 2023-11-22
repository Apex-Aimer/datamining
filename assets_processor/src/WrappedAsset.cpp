#include <fstream>
#include <iostream>

#include "RpakFileStream.h"
#include "WrappedAsset.h"

std::string getAssetName(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment, WrapAssetHeader_v7 *assetHeader)
{
    if (!assetHeader->name.index && !assetHeader->name.offset)
    {
        std::string name = "wrap_0x" + std::to_string(assetEntry->name_hash);
        return name;
    }

    fileStream->seekg(
        segment.get()->getSegmentOffset(
            assetHeader->name.index,
            assetHeader->name.offset),
        std::fstream::beg);

    return RpakFileStream::readCStringFromFile(fileStream);
}

WrappedAsset::WrappedAsset(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment)
{
    // ---- Read asset header ----
    fileStream->seekg(
        segment.get()->getSegmentOffset(
            assetEntry->sub_header_data_block_index,
            assetEntry->sub_header_data_block_offset),
        std::fstream::beg);

    WrapAssetHeader_v7 assetHeader;
    fileStream->read(reinterpret_cast<char *>(&assetHeader), sizeof(WrapAssetHeader_v7));
    // ---- Read asset header end ----

    this->name = getAssetName(assetEntry, fileStream, segment, &assetHeader);

    std::cout << "Asset name: " << this->name << std::endl;

    this->isCanExtract = (assetHeader.flags & 0x10) == 0;
    this->isCompressed = assetHeader.flags & 0x1;

    this->compressedSize = assetHeader.cmpSize;
    this->decompressedSize = assetHeader.dcmpSize;

    this->dataPtr = assetHeader.data;
}

void WrappedAsset::extract(std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment, std::filesystem::path outputDir)
{
    std::filesystem::path exportPath = outputDir / this->name;

    std::filesystem::create_directory(outputDir);

    if (!this->isCanExtract)
    {
        return;
    }

    fileStream->seekg(
        segment.get()->getSegmentOffset(
            this->dataPtr.index,
            this->dataPtr.offset),
        std::fstream::beg);

    if (!this->isCompressed)
    {
        std::ofstream ofs(exportPath, std::ios::out | std::ios::binary);

        char *buffer = new char[this->decompressedSize];
        fileStream->read(buffer, this->decompressedSize);
        ofs.write(buffer, this->decompressedSize);

        delete[] buffer;
        ofs.close();

        return;
    }

    // char *compressedBuffer = new char[Header.cmpSize];
    // Reader.Read(compressedBuffer, 0, Header.cmpSize);
    // uint64_t bufferSize = Header.dcmpSize;

    // std::unique_ptr<IO::MemoryStream> stream = RTech::DecompressStreamedBuffer((uint8_t *)compressedBuffer, bufferSize, (uint8_t)CompressionType::OODLE);

    // char *decompressedBuffer = new char[Header.dcmpSize];
    // stream->Read((uint8_t *)decompressedBuffer, 0, Header.dcmpSize);
    // stream->Close();
    // ofs.write(decompressedBuffer, Header.dcmpSize - 1);

    // delete[] decompressedBuffer;
    // delete[] compressedBuffer;

    // ofs.close();
}