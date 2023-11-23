#include <fstream>
#include <iostream>

#include "oodle2.h"

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

uint8_t *decompressOodleData(const uint8_t *data, uint64_t dataSize)
{
    int sizeNeeded = OodleLZDecoder_MemorySizeNeeded(OodleLZ_Compressor_Invalid, -1);

    uint8_t *decoder = new uint8_t[sizeNeeded]{};
    uint8_t *outBuf = new uint8_t[dataSize]{};

    OodleLZDecoder_Create(OodleLZ_Compressor::OodleLZ_Compressor_Invalid, dataSize, decoder, sizeNeeded);

    int decPos = 0;
    int dataPos = 0;

    OodleLZ_DecodeSome_Out out{};
    if (!OodleLZDecoder_DecodeSome((OodleLZDecoder *)decoder, &out, outBuf, decPos, dataSize, dataSize - decPos, data + dataPos, dataSize - dataPos, OodleLZ_FuzzSafe_No, OodleLZ_CheckCRC_No, OodleLZ_Verbosity::OodleLZ_Verbosity_None, OodleLZ_Decode_ThreadPhaseAll))
    {
        // If it fails it shouldn't be compressed?
        delete[] decoder;
        delete[] outBuf;

        return const_cast<uint8_t *>(data);
    }

    while (true)
    {
        decPos += out.decodedCount;
        dataPos += out.compBufUsed;

        if (out.compBufUsed + out.decodedCount == 0)
            break;

        if (decPos >= dataSize)
            break;

        bool decodeResult = OodleLZDecoder_DecodeSome((OodleLZDecoder *)decoder, &out, outBuf, decPos, dataSize, dataSize - decPos, data + dataPos, dataSize - dataPos, OodleLZ_FuzzSafe_No, OodleLZ_CheckCRC_No, OodleLZ_Verbosity::OodleLZ_Verbosity_None, OodleLZ_Decode_ThreadPhaseAll);
    }

    delete[] decoder;
    // delete[] data;

    return outBuf;
}

bool WrappedAsset::extract(std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment, std::filesystem::path outputDir)
{
    std::filesystem::path exportPath = outputDir / this->name;

    if (!this->isCanExtract)
    {
        return false;
    }

    fileStream->seekg(
        segment.get()->getSegmentOffset(
            this->dataPtr.index,
            this->dataPtr.offset),
        std::fstream::beg);

    if (!this->isCompressed)
    {

        char *buffer = new char[this->decompressedSize];
        fileStream->read(buffer, this->decompressedSize);

        if (!std::filesystem::exists(outputDir))
        {
            std::filesystem::create_directory(outputDir);
        }

        std::ofstream ofs(exportPath, std::ios::out | std::ios::binary);
        ofs.write(buffer, this->decompressedSize);

        delete[] buffer;
        ofs.close();

        return true;
    }

    char *compressedBuffer = new char[this->compressedSize];
    fileStream->read(compressedBuffer, this->compressedSize);

    uint8_t *stream = decompressOodleData((uint8_t *)compressedBuffer, this->decompressedSize);

    if (!std::filesystem::exists(outputDir))
    {
        std::filesystem::create_directory(outputDir);
    }

    std::ofstream ofs(exportPath, std::ios::out | std::ios::binary);
    ofs.write((char *)stream, this->decompressedSize - 1);

    delete[] stream;
    delete[] compressedBuffer;

    ofs.close();

    return true;
}