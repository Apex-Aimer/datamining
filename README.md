# datamining

A tool to extract assets from Apex Legend `rpak` files.

Thanks to:

- [LegionPlus](https://github.com/r-ex/LegionPlus)
- [r5apexsdk](https://github.com/Mauler125/r5apexsdk_alpha)
- [RePak](https://github.com/IJARika/RePak)

The difference with the tools above is that it works for `macos` (and possibly for linux, but haven't tested it).

## How to

Make sure `cmake` is installed.

Create a folder (for example `input` - `mkdir input`) and put `.rpak` files from the Apex Legends source files.

Build the tool:

```sh
cd build

cmake ..

cd ..

cmake --build build
```

Execute is:

```sh
./build/extractor/extractor input
```

> `input` is the folder where `.rpak` files are put
