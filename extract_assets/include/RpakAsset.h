#include <vector>
#include <cstdint>

#pragma once

#include "RpakSegment.h"

typedef struct
{
	uint64_t name_hash;
	uint64_t padding;

	uint32_t sub_header_data_block_index;
	uint32_t sub_header_data_block_offset;
	uint32_t raw_data_block_index;
	uint32_t raw_data_block_offset;

	uint64_t starpak_offset;
	uint64_t optimal_starpak_offset;

	uint16_t page_end; // number of the highest page that is used by this asset
	uint16_t un2;

	uint32_t relations_start_index;

	uint32_t uses_start_index;
	uint32_t relations_count;
	uint32_t uses_count;

	uint32_t sub_header_size;
	uint32_t version;
	// char magic[0x4];
	uint32_t magic;
} RpakApexAssetEntry;

class RpakAsset
{
public:
	static std::shared_ptr<RpakAsset> processRawAsset(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment);

	virtual bool extract(std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment, std::filesystem::path outDirectory) = 0;

	virtual ~RpakAsset(){};
};