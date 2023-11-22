#include <iostream>

#include "RpakAsset.h"
#include "WrappedAsset.h"

#include "common.h"

std::string assetMagicToString(RpakApexAssetEntry *assetEntry)
{
    // Extract individual ASCII values using bit manipulation
    char char1 = static_cast<char>((assetEntry->magic >> 24) & 0xFF);
    char char2 = static_cast<char>((assetEntry->magic >> 16) & 0xFF);
    char char3 = static_cast<char>((assetEntry->magic >> 8) & 0xFF);
    char char4 = static_cast<char>(assetEntry->magic & 0xFF);

    // Create a string from the extracted characters
    std::string resultString = {char1, char2, char3, char4};

    return resultString;
}

std::unique_ptr<RpakAsset> RpakAsset::processRawAsset(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment)
{
    // std::cout << "Asset entry magic: " << assetMagicToString(assetEntry) << std::endl;

    if (assetEntry->magic == (uint32_t)RpakAssetType::Wrap)
    {
        return std::make_unique<WrappedAsset>(assetEntry, fileStream, segment);
    }

    return nullptr;
}