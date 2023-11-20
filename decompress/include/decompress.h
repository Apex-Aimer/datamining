#include <string>

#include "rtech.h"

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint8_t flags;
    bool is_compressed;
    uint64_t created_file_time;
    uint64_t hash;

    uint64_t compressed_size;
    uint64_t embedded_starpak_offset;
    uint64_t alignment1;
    uint64_t decompressed_size;
    uint64_t embedded_starpak_size;
    uint64_t alignment2;

    uint16_t starpak_ref_size;
    uint16_t opt_starpak_ref_size;
    uint16_t virtual_segment_count;
    uint16_t mem_page_count;

    uint16_t patch_index;
    uint16_t alignment3;

    uint32_t descriptor_count;
    uint32_t asset_entry_count;
    uint32_t guid_descriptor_count;
    uint32_t relations_count;

    uint32_t unk3[7];
} RpakApexHeader;

class Decompress
{
public:
    static void processFile(std::string path);
};