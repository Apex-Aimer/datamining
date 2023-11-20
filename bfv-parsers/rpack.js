registerFileType((fileExt, filePath, fileData) => {
  // Check for wav extension
  if (fileExt == "rpak") {
    return true;
  }
  return false;
});

function readApexHeader() {
  read(4);
  addRow("Magic", getStringValue(), "Rpak file description header");

  read(2); // 6
  addRow("Version", getNumberValue(), "Version");
  read(1); // 7
  addRow("Flags", getHex0xValue());
  read(1); // 8
  addRow("isCompressed", getNumberValue(), "isCompressed");
  read(8); // 16
  addRow("CreatedFileTime", getHex0xValue(), "CreatedFileTime");
  read(8); // 24
  addRow("Hash", getHex0xValue(), "Hash");

  // ---

  read(8); // 32
  addRow("CompressedSize", getNumberValue(), "CompressedSize");
  read(8); // 40
  addRow("EmbeddedStarpakOffset", getNumberValue(), "");
  read(8); // 48
  addRow("Padding", getNumberValue(), "");
  read(8); // 56
  addRow("DecompressedSize", getNumberValue(), "");
  // Until this point seems to be correct
  read(8); // 64
  addRow("EmbeddedStarpakSize", getNumberValue(), "");
  read(8); // 72
  addRow("Padding2", getNumberValue(), "");

  // ---
  read(2); // 74
  const starpakPathsSize = getNumberValue();
  addRow(
    "StarpakReferenceSize",
    starpakPathsSize,
    "starpakPathsSize - size in bytes of the section containing mandatory starpak paths"
  );
  read(2); // 76
  const optStarpakPathsSize = getNumberValue();
  addRow(
    "StarpakOptReferenceSize",
    optStarpakPathsSize,
    "optStarpakPathsSize - size in bytes of the section containing optional starpak paths"
  );
  read(2); // 78
  const virtualSegmentCount = getNumberValue();
  addRow("VirtualSegmentCount", virtualSegmentCount, "");
  read(2); // 80
  const memPageCount = getNumberValue();
  addRow("MemPageCount", memPageCount, "");

  read(2); // 82
  const patchIndex = getNumberValue();
  addRow("PatchIndex", patchIndex, "");
  read(2); // 84
  addRow("alignment", getHex0xValue(), "");

  read(4); // 88
  const descriptorCount = getNumberValue();
  addRow("DescriptorCount", descriptorCount, "");
  read(4); // 92
  const assetEntryCount = getNumberValue();
  addRow("AssetEntryCount", assetEntryCount, "");
  read(4); // 96
  const guidDescriptorCount = getNumberValue();
  addRow("GuidDescriptorCount", guidDescriptorCount, "");
  read(4); // 100
  const relationsCount = getNumberValue();
  addRow("RelationsCount", relationsCount, "");

  //---

  // read(4); // 104
  // addRow("unk7count", getHex0xValue());
  // read(4); // 108
  // addRow("unk8count", getHex0xValue());
  read(28); // 128
  addRow("unk3", getHex0xValue());

  return {
    patchIndex,
    starpakPathsSize,
    optStarpakPathsSize,
    virtualSegmentCount,
    memPageCount,
    descriptorCount,
    assetEntryCount,
    guidDescriptorCount,
    relationsCount,
  };
}

function readRpakVirtualSegment() {
  read(4); // DataFlag
  read(4); // DataType aka alignment
  read(8); // DataSize
}

function readRpakMemPage() {
  read(4); // VirtualSegmentIndex
  read(4); // Flags aka alignment
  read(4); // DataSize
}

function readRpakDescriptor() {
  read(4); // PageIdx
  read(4); // PageOffset
}

function readRpakFileRelation() {
  read(4); // AssetIndex
}

/**
 * @param {number} count
 * @param {boolean} print
 */
function readRpakAssetEntry(count, print) {
  read(8);
  print && addRow(`${count}: NameHash`, getHex0xValue());
  read(8);
  print && addRow(`${count}: Padding`, getHex0xValue());

  read(4);
  print && addRow(`${count}: SubHeaderDataBlockIndex`, getHex0xValue());
  read(4);
  print && addRow(`${count}: SubHeaderDataBlockOffset`, getHex0xValue());
  read(4);
  print && addRow(`${count}: RawDataBlockIndex`, getHex0xValue());
  read(4);
  print && addRow(`${count}: RawDataBlockOffset`, getHex0xValue());

  read(8);
  print && addRow(`${count}: StarpakOffset`, getHex0xValue());
  read(8);
  print && addRow(`${count}: OptimalStarpakOffset`, getHex0xValue());

  read(2);
  print && addRow(`${count}: PageEnd`, getHex0xValue());
  read(2);
  print && addRow(`${count}: Un2`, getHex0xValue());

  read(4);
  print && addRow(`${count}: RelationsStartIndex`, getHex0xValue());

  read(4);
  print && addRow(`${count}: UsesStartIndex`, getHex0xValue());
  read(4);
  print && addRow(`${count}: RelationsCount`, getNumberValue());
  read(4);
  print && addRow(`${count}: UsesCount`, getNumberValue());

  read(4);
  print && addRow(`${count}: SubHeaderSize`, getHex0xValue());
  read(4);
  print && addRow(`${count}: Version`, getHex0xValue());
  read(4);
  print && addRow(`${count}: Magic`, getStringValue());
}

registerParser(() => {
  const {
    patchIndex,
    starpakPathsSize,
    optStarpakPathsSize,
    virtualSegmentCount,
    memPageCount,
    descriptorCount,
    assetEntryCount,
    guidDescriptorCount,
    relationsCount,
  } = readApexHeader();

  /**
   * Like parsing this it looks like incorrect
   * in PePak it's a part of unk3,
   * but in Legion they're exist
   */
  // read(4);
  // addRow("PatchDataSize", getNumberValue());
  // read(4);
  // addRow("PatchSegmentIndex", getNumberValue());

  // --- rpak file header
  // assert(136) or assert(128)

  // Until this point seems to be correct too

  if (patchIndex > 0) {
    read(8);
    addRow("PatchCompressedSize", getNumberValue(), "RpakPatchCompressPair");
    read(8);
    addRow("PatchDecompressedSize", getNumberValue(), "RpakPatchCompressPair");

    read(2);
    addRow("PatchIndicesToFile", getNumberValue(), "PatchIndicesToFile");

    read(6);
  }

  let i = starpakPathsSize;
  let j = 0;

  while (i > 0) {
    readUntil(0);
    const str = getStringValue();
    addRow(`path:${++j}`, str);

    read(1);
    i -= str.length + 1;
  }

  i = optStarpakPathsSize;
  j = 0;

  while (i > 0) {
    readUntil(0);
    const str = getStringValue();
    addRow(`optPath:${++j}`, str);

    i -= str.length + 1;
  }

  // // This one is for sure
  // read(8);
  // // addRow("Starpak unknown1", getStringValue());
  // addMemDump();
  // // read(26);
  // readUntil(0);
  // addRow("Starpak", getStringValue());
  // read(starpakPathsSize - 26);
  // addMemDump();
  // if (starpakPathsSize - 26 > 0) {
  //   addRow("Starpak unknown2", getStringValue());
  // }

  // if (optStarpakPathsSize > 0) {
  //   read(optStarpakPathsSize);
  //   addRow("Opt starpak paths", getStringValue());
  // }

  for (let i = 0; i < virtualSegmentCount; i += 1) {
    readRpakVirtualSegment();
  }
  for (let i = 0; i < memPageCount; i += 1) {
    readRpakMemPage();
  }
  for (let i = 0; i < descriptorCount; i += 1) {
    readRpakDescriptor();
  }
  for (let i = 0; i < assetEntryCount; i += 1) {
    readRpakAssetEntry(i + 1, i < 10);
  }
  // for (let i = 0; i < guidDescriptorCount; i += 1) {
  //   readRpakDescriptor();
  // }
  // for (let i = 0; i < relationsCount; i += 1) {
  //   readRpakFileRelation();
  // }

  // read(1000);
  // addMemDump();
});
