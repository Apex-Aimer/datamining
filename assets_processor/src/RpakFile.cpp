#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "RpakFile.h"
#include "RpakFileStream.h"

std::ifstream RpakFile::openStream()
{
    std::ifstream rpakSource(this->rpakFilePath, std::fstream::binary);

    if (!rpakSource.is_open())
    {
        std::cerr << "Error code: " << strerror(errno) << std::endl;

        throw std::runtime_error(strerror(errno));
    }

    return rpakSource;
};

void RpakFile::parse()
{
    // Open the binary file in input mode
    auto rpakSource = this->openStream();

    std::cout << "Processing: " << this->rpakFilePath.filename().string() << std::endl;

    // Read the file header into a struct
    RpakApexHeader header;
    rpakSource.read(reinterpret_cast<char *>(&header), sizeof(RpakApexHeader));

    std::vector<std::string> starpaks = RpakFileStream::readPathsFromFile(&rpakSource, header.starpak_ref_size);
    /**
     * It's either `optimal` or `optional`, need to figure it out and change the naming
     */
    std::vector<std::string> optStarpaks = RpakFileStream::readPathsFromFile(&rpakSource, header.opt_starpak_ref_size);

    this->virtualSegmentsStart = rpakSource.tellg();
    this->virtualSegmentsSize = sizeof(RpakVirtualSegment) * header.virtual_segment_count;

    this->memPagesStart = this->virtualSegmentsStart + this->virtualSegmentsSize;
    this->memPagesSize = sizeof(RpakMemPage) * header.mem_page_count;

    this->descriptorsStart = this->memPagesStart + this->memPagesSize;
    this->descriptorsSize = sizeof(RpakDescriptor) * header.descriptor_count;

    this->assetsStart = this->descriptorsStart + this->descriptorsSize;
    this->assetsSize = sizeof(RpakApexAssetEntry) * header.asset_entry_count;

    this->guidDescriptorsStart = this->assetsStart + this->assetsSize;
    this->guidDescriptorsSize = sizeof(RpakDescriptor) * header.guid_descriptor_count;

    this->relationsStart = this->guidDescriptorsStart + this->guidDescriptorsSize;
    this->relationsSize = sizeof(RpakFileRelation) * header.relations_count;

    /**
     * TODO: There's also a patch data
     */

    // ---- Segment Data ----
    std::size_t segmentDataStart = this->relationsStart + this->relationsSize;
    // Seek to the end of the file to get the total size
    rpakSource.seekg(0, std::fstream::end);
    // Get the position at the end of the file
    std::size_t endPosition = rpakSource.tellg();

    this->segment = std::make_shared<RpakSegment>(
        segmentDataStart,
        // Calculate the size from the current position to the end of the file
        endPosition - segmentDataStart);

    this->segment.get()->parseSegmentBlocks(
        &rpakSource,
        this->memPagesStart,
        header.mem_page_count,
        header.page_offset);
    // ---- Segment Data End ----

    // ---- Init assets ----
    rpakSource.seekg(this->assetsStart, std::fstream::beg);

    std::vector<RpakApexAssetEntry> assetsEntry;

    for (int i = 0; i < header.asset_entry_count; i += 1)
    {
        RpakApexAssetEntry assetEntry;
        rpakSource.read(reinterpret_cast<char *>(&assetEntry), sizeof(RpakApexAssetEntry));

        assetsEntry.push_back(assetEntry);
    }

    for (auto assetEntry : assetsEntry)
    {
        std::shared_ptr<RpakAsset> asset = RpakAsset::processRawAsset(&assetEntry, &rpakSource, this->segment);

        if (asset == nullptr)
        {
            continue;
        }

        this->assets.push_back(std::move(asset));
    }
    // ---- Init assets end ----

    std::cout << "Processed file" << std::endl;
};

void RpakFile::extractAssets()
{
    std::filesystem::path dir = this->outputPath.parent_path();
    std::filesystem::path outputDir = dir / "output";

    auto rpakSource = this->openStream();

    for (auto asset : this->assets)
    {
        asset->extract(&rpakSource, this->segment, outputDir);
    }
}