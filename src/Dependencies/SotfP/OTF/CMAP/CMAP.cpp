#include "CMAP.h"
#include "../../utils.h"

lb::CMAP::CMAP(std::vector<unsigned char> v_bytes)
{
    int start = 0;
    this->version = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->numTables = ByteToVal<unsigned short>(&v_bytes[start], start);

    for (int i = 0; i < this->numTables; i++)
    {
        lb::CMAP::EncodingRecord er;
        er.platformID = ByteToVal<unsigned short>(&v_bytes[start], start);
        er.encodingID = ByteToVal<unsigned short>(&v_bytes[start], start);
        er.subtableOffset = ByteToVal<unsigned int>(&v_bytes[start], start);
        this->encodingRecords.push_back(er);
    }

    start = this->encodingRecords[0].subtableOffset;
    this->format = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->length = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->language = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->segCountX2 = ByteToVal<unsigned short>(&v_bytes[start], start);

    int segCount = this->segCountX2 / 2;

    // TODO: Hardcoded Format 4 only 
    this->searchRange = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->searchRange = pow(2, (floor(log2(segCount)))) * 2;
    this->entrySelector = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->entrySelector = log2(this->searchRange / 2);
    this->rangeShift = ByteToVal<unsigned short>(&v_bytes[start], start);
    this->rangeShift = (segCount * 2) - searchRange;

    for (int i = 0; i < segCount; i++)
        this->endCode.push_back(ByteToVal<unsigned short>(&v_bytes[start], start));

    this->reservedPad = ByteToVal<unsigned short>(&v_bytes[start], start);

    for (int i = 0; i < segCount; i++)
        this->startCode.push_back(ByteToVal<unsigned short>(&v_bytes[start], start));

    for (int i = 0; i < segCount; i++)
        this->idDelta.push_back(ByteToVal<short>(&v_bytes[start], start));

    for (int i = 0; i < segCount; i++)
        this->idRangeOffset.push_back(ByteToVal<unsigned short>(&v_bytes[start], start));

    // TODO: Remove?
    // Leaving this here for now, caused some issues before, I dont think this is relevant anymore
    //for (int i = start; i < this->length; i++)
    //    this->glyphIdArray.push_back(ByteToVal<unsigned short>(&v_bytes[start], start));

    // TODO: this glyphId approach wont work for all fonts
    // see: https://learn.microsoft.com/en-us/typography/opentype/spec/cmap
    if (this->glyphIdArray.size() == 0)
        for (int i = 0; i < segCount; i++)
            this->glyphIdArray.push_back(this->startCode[i] + this->idDelta[i]);
}

int lb::CMAP::GetGlyphId(char v_char)
{
    for (int i = 0; i < this->segCountX2 / 2; i++)
        if (v_char == this->startCode[i])
            return this->glyphIdArray[i];

    return 0;
}

std::vector<unsigned short> lb::CMAP::GetAvailableChars()
{
    return this->startCode;
}
