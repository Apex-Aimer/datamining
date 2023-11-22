#include <vector>
#include <fstream>

#pragma once

struct RpakFileSegmentBlock
{
    uint64_t offset;
    uint32_t data_size;

    RpakFileSegmentBlock(uint32_t off, uint32_t size) : offset(off), data_size(size) {}
};

class RpakSegment
{
public:
    RpakSegment(std::size_t start, uint32_t size) : segmentDataStart(start), segmentDataSize(size){};

    void parseSegmentBlocks(std::ifstream *fileStream, std::size_t memPagesStart, uint32_t memPagesCount, uint64_t initialOffset);

    uint64_t getSegmentOffset(uint32_t SegmentIndex, uint32_t SegmentOffset);

    std::size_t segmentDataStart;
    uint32_t segmentDataSize;

    uint32_t startSegmentIndex = 0;

private:
    std::vector<std::unique_ptr<RpakFileSegmentBlock>> segmentBlocks;
};