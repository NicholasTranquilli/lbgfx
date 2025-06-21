#pragma once

#include <vector>

namespace lb
{
    class CMAP
    {
        struct EncodingRecord
        {
            unsigned short platformID;
            unsigned short encodingID;
            unsigned int subtableOffset;
        };

    public:
        unsigned short version;
        unsigned short numTables;
        std::vector<EncodingRecord> encodingRecords;

        // TODO: Only support format 4 at the moment...

        unsigned short format;
        unsigned short length;
        unsigned short language;
        unsigned short segCountX2;

        unsigned short searchRange;
        unsigned short entrySelector;
        unsigned short rangeShift;
        std::vector<unsigned short> endCode;
        unsigned short reservedPad;
        std::vector<unsigned short> startCode;
        std::vector<short> idDelta;
        std::vector<unsigned short> idRangeOffset;
        std::vector<unsigned short> glyphIdArray;

        CMAP() = default;
        CMAP(std::vector<unsigned char> v_bytes);
        int GetGlyphId(char v_char);
        std::vector<unsigned short> GetAvailableChars();
    };
}
