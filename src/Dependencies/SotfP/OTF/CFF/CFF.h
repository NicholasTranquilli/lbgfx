#pragma once

#include <vector>
#include <string>
#include <map>
#include <variant>

namespace lb
{
    /******************************************/
    //               CFF ENUMS                //
    /******************************************/

	namespace cff
	{
        struct DICT_OPERATOR
        {
            enum BYTE
            {
                version,
                Notice,
                FullName,
                FamilyName,
                Weight,
                FontBBox,
                BlueValues,
                OtherBlues,
                FamilyBlues,
                FamilyOtherBlues,
                StdHW,
                StdVW,
                escape,
                UniqueID,
                XUID,
                charset,
                Encoding,
                CharStrings,
                Private,
                Subrs,
                defaultWidthX,
                nominalWidthX,
                // RESERVED 22 - 27 
                shortint = 28,
                longint,
                BCD
                // RESET RESERVED
            };
            enum TWO_BYTE
            {
                Copyright,
                isFixedPitch,
                ItalicAngle,
                UnderlinePosition,
                UnderlineThickness,
                PaintType,
                CharstringType,
                FontMatrix,
                StrokeWidth,
                BlueScale,
                BlueShift,
                BlueFuzz,
                StemSnapH,
                StemSnapV,
                ForceBold,
                // RESERVED 15 - 16 
                LanguageGroup = 17,
                ExpansionFactor,
                initialRandomSeed,
                SyntheticBase,
                PostScript,
                BaseFontName,
                BaseFontBlend,
                // RESERVED 24 - 29
                ROS = 30,
                CIDFontVersion,
                CIDFontRevision,
                CIDFontType,
                CIDCount,
                UIDBase,
                FDArray,
                FDSelect,
                FontName
                // REST RESERVED
            };
        };

        struct CFF_OPERATOR // Appendix A in Type 2 charstring format docs
        {
            enum BYTE
            {
                // RESERVED
                hstem = 1,
                // RESERVED
                vstem = 3,
                vmoveto,
                rlineto,
                hlineto,
                vlineto,
                rrcurveto,
                // RESERVED
                callsubr = 10,
                sub_return,
                escape,
                // RESERVED
                endchar = 14,
                // RESERVED 15 - 17
                hstemhm = 18,
                hintmask,
                cntrmask,
                rmoveto,
                hmoveto,
                vstemhm,
                rcurveline,
                rlinecurve,
                vvcurveto,
                hhcurveto,
                shortint,
                callgsubr,
                vhcurveto,
                hvcurveto,
            };
            enum TWO_BYTE
            {
                // DEPRICATED / RESERVED
                // RESERVED 1 - 2
                cff_and = 3,
                cff_or,
                cff_not,
                // RESERVED 6 - 8
                cff_abs = 9,
                cff_add,
                cff_sub,
                cff_div,
                // RESERVED
                cff_neg = 14,
                cff_eq,
                // RESERVED 16 - 17
                cff_drop = 18,
                // RESERVED
                cff_put = 20,
                cff_get,
                cff_ifelse,
                cff_random,
                cff_mul,
                // RESERVED
                cff_sqrt = 26,
                cff_dup,
                cff_exch,
                cff_index,
                cff_roll,
                // RESERVED 31 - 33
                hflex = 34,
                flex,
                hflex1,
                flex1,
            };
        };

		struct DictElement
		{
			std::vector<std::variant<unsigned char, long long, double>> bytes;
			int op = 0;
			int op2 = 0;
		};
	}

    /******************************************/
    //               CFF CLASS                //
    /******************************************/

	class CFF
	{
    public:
        struct BoundsBox { int minX, minY, maxX, maxY; };

    private:
		std::vector<unsigned char> bytes;
		std::map<std::string, lb::cff::DictElement> TopDictElements;
		std::map<std::string, lb::cff::DictElement> PrivateDictElements;
		std::vector<std::string> CharstringElements;
		unsigned short nGlyphs;

        struct Index
        {
            unsigned short count;
            unsigned char offSize;
            std::vector<unsigned int> offset;
            std::vector<unsigned char> data;

            int GetDataStart();
        };

        struct Header
        {
            unsigned char major;
            unsigned char minor;
            unsigned char hdrSize;
            unsigned char offSize;

        };

        struct Charsets
        {
            struct R1
            {
                unsigned short first;
                unsigned char nLeft;
            };
            struct R2
            {
                unsigned short first;
                unsigned short nLeft;
            };

            unsigned char format;
            std::vector<std::variant<unsigned short, R1, R2>> glyphRange;
        };

        Header Head;
        Index NameIndex, TopDictIndex, PrivateDictIndex, StringIndex, GlobalSubrIndex, CharstringIndex, FontDictIndex, LocalSubrIndex;
        Charsets Charset;

        BoundsBox FontBBox;

        int MakeIndex(Index& r_idx, int v_start);
        int MakeIndexData(Index& r_idx, int& v_start);
        void ParseDict(Index& r_idx, std::map<std::string, lb::cff::DictElement>& r_map);
        void ParseCharset(Charsets& r_chrst, int v_start);

	public:
		CFF();
		CFF(std::vector<unsigned char>& v_bytes);

        int DecodeInt(Index& idx, int& i);
        int DecodeInt(std::vector<unsigned char>& idx, int& i);
		double DecodeRealNumber(Index& idx, int& i);
		std::vector<unsigned char> GetPathAt(int v_id);

        BoundsBox GetFontBounds();
	};
}

#undef CREATE_CFF_STRUCT