#include <filesystem>
#include <vector>

#include "common.h"

#include "RpakAsset.h"
#include "RpakSegment.h"

#pragma once

class RpakFile
{
public:
    RpakFile(std::filesystem::path inPath, std::filesystem::path outPath) : rpakFilePath(inPath), outputPath(outPath){};

    void parse();
    void extractAssets();

private:
    std::ifstream openStream();

    std::filesystem::path rpakFilePath;
    std::filesystem::path outputPath;

    std::vector<std::shared_ptr<RpakAsset>> assets;
    std::shared_ptr<RpakSegment> segment;

    uint32_t virtualSegmentsStart;
    uint32_t virtualSegmentsSize;
    uint32_t memPagesStart;
    uint32_t memPagesSize;
    uint32_t descriptorsStart;
    uint32_t descriptorsSize;

    uint32_t assetsStart;
    uint32_t assetsSize;

    uint32_t guidDescriptorsStart;
    uint32_t guidDescriptorsSize;
    uint32_t relationsStart;
    uint32_t relationsSize;
};