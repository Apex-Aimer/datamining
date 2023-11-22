#include <fstream>
#include <iostream>

#include "RpakFileStream.h"
#include "WrappedAsset.h"

WrappedAsset::WrappedAsset(RpakApexAssetEntry *assetEntry, std::ifstream *fileStream, std::shared_ptr<RpakSegment> segment)
{
    fileStream->seekg(
        segment.get()->getSegmentOffset(
            assetEntry->sub_header_data_block_index,
            assetEntry->sub_header_data_block_offset),
        std::fstream::beg);

    // Asset.SubHeaderIndex / asset->sub_header_data_block_index
    // Asset.SubHeaderOffset / asset->sub_header_data_block_offset

    WrapAssetHeader_v7 assetHeader;
    fileStream->read(reinterpret_cast<char *>(&assetHeader), sizeof(WrapAssetHeader_v7));

    fileStream->seekg(
        segment.get()->getSegmentOffset(
            assetHeader.name.index,
            assetHeader.name.offset),
        std::fstream::beg);

    std::string name = RpakFileStream::readCStringFromFile(fileStream);

    std::cout << "Asset name: " << name << std::endl;

    return;
    // auto RpakStream = this->GetFileStream(Asset);
    // IO::BinaryReader Reader = IO::BinaryReader(RpakStream.get(), true);

    // RpakStream->SetPosition(this->GetFileOffset(Asset, Asset.SubHeaderIndex, Asset.SubHeaderOffset));
    // WrapAssetHeader_v7_t Header = Reader.Read<WrapAssetHeader_v7_t>();

    // if (Header.name.Index || Header.name.Offset)
    //     Info.Name = this->ReadStringFromPointer(Asset, Header.name);
    // else
    //     Info.Name = string::Format("wrap_0x%llx", Asset.NameHash);

    // Info.Type = ApexAssetType::Wrap;
    // Info.Status = ApexAssetStatus::Loaded;
    // Info.Info = string::Format("Size: %i", Header.dcmpSize);
}

std::vector<uint8_t> WrappedAsset::extract()
{
    std::vector<uint8_t> todo(1);

    return todo;
}