#include "common.h"
#include "RpakSegment.h"

void RpakSegment::parseSegmentBlocks(std::ifstream *fileStream, std::size_t memPagesStart, uint32_t memPagesCount, uint64_t initialOffset)
{
    fileStream->seekg(memPagesStart, std::fstream::beg);
    uint64_t segmentOffset = initialOffset;

    for (uint32_t i = 0; i < memPagesCount; i++)
    {
        RpakMemPage memPage;
        fileStream->read(reinterpret_cast<char *>(&memPage), sizeof(RpakMemPage));

        std::unique_ptr<RpakFileSegmentBlock> segmentBlock = std::make_unique<RpakFileSegmentBlock>(segmentOffset, memPage.data_size);

        this->segmentBlocks.push_back(std::move(segmentBlock));

        segmentOffset += memPage.data_size;
    }
};

uint64_t RpakSegment::getSegmentOffset(uint32_t segmentIndex, uint32_t segmentOffset)
{
    if (segmentIndex < 0)
    {
        return 0;
    }

    uint32_t normalizedSegmentIndex = segmentIndex - this->startSegmentIndex;

    return this->segmentDataStart +
           this->segmentBlocks[normalizedSegmentIndex].get()->offset +
           segmentOffset;
}