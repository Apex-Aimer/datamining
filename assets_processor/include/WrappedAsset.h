#include "common.h"
#include "RpakAsset.h"

struct WrapAssetHeader_v7
{
    RPakPtr name;
    RPakPtr data;
    char unk[4];
    int cmpSize;
    int dcmpSize;
    uint16_t nameLength;
    char unk_2[4];
    uint16_t flags;
    char unk_3[12];
};

class WrappedAsset : public RpakAsset
{
public:
    WrappedAsset(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment);

    std::vector<uint8_t> extract();
};