#pragma once

#include "CFF/CFF.h"
#include "CMAP/CMAP.h"
#include <fstream>

namespace lb
{
	class OTF
	{
        struct TableDirectory
        {
            struct TableRecord
            {
                char tag[4];
                unsigned int checksum;
                unsigned int offset;
                unsigned int length;

                static const int PackedSize();
            };

            unsigned int sfntVersion;
            unsigned short numTables;
            unsigned short searchRange;
            unsigned short entrySelector;
            unsigned short rangeShift;

            std::vector<TableRecord> tableRecords;

            static const int PackedHeaderSize();
        };

        struct Header
        {
            unsigned int sfntVersion;
            unsigned short numTables;
            unsigned short searchRange;
            unsigned short entrySelector;
            unsigned short rangeShift;

            static const int PackedSize();
        };

        struct TableID
        {
            enum  {
                CFF = 0x43464620,
                FFTM = 0x4646544d,
                GDEF = 0x47444546,
                GPOS = 0x47504f53,
                GSUB = 0x47535542,
                JSTF = 0x4a535446,
                OS2 = 0x4f532f32,
                cmap = 0x636d6170,
                head = 0x68656164,
                hhea = 0x68686561,
                hmtx = 0x686d7478,
                maxp = 0x6d617870,
                name = 0x6e616d65,
                post = 0x706f7374,
            };
        };

		std::ifstream vFile;

		Header vOtfHeader;
		TableDirectory vTableDirectory;
		lb::CMAP vCmapTable;
		lb::CFF vCffTable;

        int MakeTagId(char* v_tag);
        void ParseGlyphTable(TableDirectory::TableRecord v_tr);
        void ParseMappingTable(TableDirectory::TableRecord v_tr);
        void ParseTable(TableDirectory::TableRecord v_tr);
        void FillTableDirectory();

	public:
		OTF(std::string v_fileName);
		std::vector<unsigned char> GetGlyph(char v_character);

        lb::CMAP& GetCMAP();
        lb::CFF& GetCFF();
	};
}