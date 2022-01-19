# Tiny MOV Loader and Writer library

Header-only simple&limited MOV (Apple QuickTime Movie) loader in C++.

## Features

* [x] Opening existing MOV files
* [x] Some MOV info is supported
  * [x] Tracks info
    * [x] Track media info
      * [x] Sample offsets in MOV file
      * [x] Sample sizes in MOV file
  * [x] Metadata info
* [x] Atoms supported for reading (not checked - skipped for now):
  * [x] ftyp
  * [ ] jpeg
  * [ ] RECO
  * [ ] free
  * [x] moov
    * [x] mvhd
    * [x] trak
      * [x] tkhd
      * [ ] edts
      * [ ] tref
      * [x] mdia
        * [x] mdhd
        * [ ] hdlr
        * [x] minf
          * [ ] vmhd
          * [ ] hdlr
          * [ ] dinf
          * [x] stbl
            * [ ] stsd
            * [ ] stts
            * [ ] stss
            * [ ] sdtp
            * [ ] stsc
            * [x] stsz
            * [x] stco
    * [x] meta
      * [ ] hdlr
        * [x] keys
      * [x] mdta
    * [x] ilst
        * [x] data
    * [x] udta
  * [ ] wide
  * [ ] mdat

## Usage

```
#include "TinyMovFileReader.hpp"

int main()
{
    // Create temporary reader
    TinyMovFileReader reader;

    // Here file is opened, parsed to abstractions and closed
    auto mov = reader.OpenMovFile("./example_0_93.ZRAW");

    // Check metadata correctness
    if (!mov.Metadata().IsOk())
    {
        std::cout << "Metadata is not ok!" << std::endl;
        return -1;
    }

    // Process metadata
    for (int i = 0; i < mov.Metadata().Keys().size(); ++i)
    {
        // Print metadata keys and values
        std::cout << mov.Metadata().Keys()[i] << " - " << mov.Metadata().Values()[i] << std::endl;
    }

    // Show tracks count
    std::cout << "Tracks: " << mov.Tracks().size() << std::endl;

    // Process tracks
    for (int i = 0; i < mov.Tracks().size(); ++i)
    {
        auto& track = mov.Tracks()[i];

        // Check track media info correctness
        if (!track.Media().Info().IsOk())
        {
            std::cout << "Track media info is not ok!" << std::endl;
            return -1;
        }

        // ... (here you do any stuff with tracks)
    }

    return 0;
}

```

## Resource

* QuickTime File Format Specification - https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html

## TODO

* [ ] ".ZRAW" encrypted MOV files support
* [ ] Improve MOV reader
  * [ ] Support more atoms
* [ ] Support Big MOV (4GB+)
* [ ] MOV writer
* [ ] Allow MOV file repack (MOV -> TinyMovReader -> TinyMovWriter -> MOV)

## License

TinyMovFileReader is licensed under MIT license.
