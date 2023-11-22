#include <cstdint>

#pragma once

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

    uint8_t unk1[0x10];
    uint32_t page_offset;
    uint8_t unk2[0x2];
} RpakApexHeader;

#pragma GCC diagnostic ignored "-Wmultichar"

enum class RpakAssetType : uint32_t
{
    Model = '_ldm',           // mdl_ - 0x5F6C646D
    Texture = 'rtxt',         // txtr - 0x72747874
    TextureAnimated = 'naxt', // txan - 0x6e617874
    UIIA = 'aiiu',            // uiia - 0x61696975
    DataTable = 'lbtd',       // dtbl - 0x6C627464
    Settings = 'sgts',        // stgs - 0x73677473
    SettingsLayout = 'tlts',  // stlt - 0x746c7473
    Material = 'ltam',        // matl - 0x6C74616D
    AnimationRig = 'gira',    // arig - 0x67697261
    Animation = 'qesa',       // aseq - 0x71657361
    Subtitles = 'tbus',       // subt - 0x74627573
    ShaderSet = 'sdhs',       // shds - 0x73646873
    Shader = 'rdhs',          // shdr - 0x72646873
    UIImageAtlas = 'gmiu',    // uimg - 0x676D6975
    RSON = 'nosr',            // rson - 0x72736F6E
    RUI = 'iu',               // ui - 0x75690000
    Map = 'pamr',             // rmap - 0x70616D72
    Effect = 'tcfe',          // efct - 0x74636665
    Wrap = 'parw',            // wrap -
};

typedef struct
{
    uint32_t page_idx;
    uint32_t page_offset;
} RpakDescriptor;

typedef struct
{
    uint32_t data_flag; // Flags (Streamed, Internal...?)
    uint32_t data_type; // What this data contains...?
    uint64_t data_size; // The size of the data block... (Sometimes bigger than data in-file, probably also contains streamed shit...)
} RpakVirtualSegment;

typedef struct
{
    uint32_t virtual_segment_index; // Corrosponds to a data block entry
    uint32_t flags;                 // Unknown right now
    uint32_t data_size;             // Total size of the block
} RpakMemPage;

typedef struct
{
    uint32_t AssetIndex;
} RpakFileRelation;

union RPakPtr
{
    struct
    {
        uint32_t index;
        uint32_t offset;
    };
    uint64_t value;
};