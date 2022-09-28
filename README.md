# Tiny MOV Loader and Writer library

Header-only simple&limited MOV (Apple QuickTime Movie) loader in C++.

## Features

* [x] Opening existing MOV files
* [x] Editing opened MOV info before writing
* [x] Writing new MOV files (remuxing)
* [x] Some MOV info is supported
  * [x] Tracks info
    * [x] Track media info
      * [x] Type (Audio/Video/Timecode/...)
      * [x] Chunk offsets in MOV file
      * [x] Sample sizes in MOV file
      * [x] Sample-to-Chunk info
      * [x] Time-to-Sample info
      * [x] Sample Description Extensions
        * [x] zraw
  * [x] Metadata info
* [x] Atoms supported for reading/writing (not checked - skipped for now):
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
        * [x] hdlr
        * [x] minf
          * [x] vmhd (for video)
          * [x] smhd (for audio)
          * [x] gmhd (for timecode)
            * [x] gmin
            * [x] tcmd
              * [x] tcmi
          * [x] hdlr
          * [x] dinf
            * [x] dref
          * [x] stbl
            * [x] stsd
            * [x] stts
            * [ ] stss
            * [ ] sdtp
            * [x] stsc
            * [x] stsz
            * [x] stco
    * [x] meta
      * [x] hdlr
      * [x] keys
      * [x] mdta
      * [x] ilst
    * [x] udta
  * [ ] wide
  * [x] mdat

## Usage

```
#include "TinyMovFileReader.hpp"

int main()
{
    // Create temporary reader
    TinyMovFileReader reader;

    // Here file is opened, parsed to abstractions and closed
    auto mov = reader.OpenMovFile("./example_0_93.ZRAW");

    // Show tracks count
    std::cout << "Tracks: " << mov.Tracks().size() << std::endl;

    // Process tracks
    for (int i = 0; i < mov.Tracks().size(); ++i)
    {
        auto& track = mov.Tracks()[i];

        // ... (here you do any stuff with tracks)
    }

    return 0;
}

```

## Resource

* QuickTime File Format Specification - https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html
* QuickTime File Format Specification - https://wikileaks.org/sony/docs/05/docs/Apple/qtff.pdf

## TODO

* [ ] ".ZRAW" encrypted MOV files support
* [ ] Improve MOV reader
  * [ ] Support more atoms
* [ ] Support Big MOV (4GB+)
* [x] MOV writer
* [x] Allow MOV file repack (MOV -> TinyMovReader -> TinyMovWriter -> MOV)

## License

TinyMovFileReader is licensed under MIT license.
