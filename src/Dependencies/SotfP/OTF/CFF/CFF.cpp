#include "CFF.h"
#include "../../utils.h"

int lb::CFF::MakeIndex(Index& r_idx, int v_start)
{
	r_idx.count = ByteToVal<unsigned short>(&this->bytes[v_start], v_start);

	r_idx.offSize = this->bytes[v_start++];

	int offsetCount = (r_idx.count == 0) ? 0 : r_idx.count + 1;
	for (int i = 0; i < offsetCount; i++)
	{
		unsigned int result = ByteToVal<unsigned int>(&this->bytes[v_start], v_start, r_idx.offSize);
		r_idx.offset.push_back(result);
	}

	this->MakeIndexData(r_idx, v_start);

	return v_start;
}

int lb::CFF::MakeIndexData(Index& r_idx, int& v_start)
{
	int dataCount = (r_idx.count == 0) ? 0 : r_idx.offset.back() - 1;
	for (int i = 0; i < dataCount; i++)
	{
		unsigned char result = ByteToVal<unsigned char>(&this->bytes[v_start], v_start);
		r_idx.data.push_back(result);
	}
	return v_start;
}

void lb::CFF::ParseDict(Index& r_idx, std::map<std::string, lb::cff::DictElement>& r_map)
{
	lb::cff::DictElement de;
	int val;

#define LB_PARSE_DICT_SWITCH(what)			\
case lb::cff::DICT_OPERATOR::what:			\
	de.op = val;							\
	r_map.emplace(#what, de);				\
	de.bytes.clear();						\
	de.op = 0;								\
	de.op2 = 0;								\
	break							

#define LB_PARSE_DICT_SWITCH2(what)			\
case lb::cff::DICT_OPERATOR::what:			\
	de.op2 = val;							\
	r_map.emplace(#what, de);				\
	de.bytes.clear();						\
	de.op = 0;								\
	de.op2 = 0;								\
	break							

	for (int i = 0; i < r_idx.data.size(); i++)
	{
		long long number = 0;
		double real = 0;
		val = r_idx.data[i];
		switch (val)
		{
			LB_PARSE_DICT_SWITCH(version);
			LB_PARSE_DICT_SWITCH(Notice);
			LB_PARSE_DICT_SWITCH(FullName);
			LB_PARSE_DICT_SWITCH(FamilyName);
			LB_PARSE_DICT_SWITCH(Weight);
			LB_PARSE_DICT_SWITCH(FontBBox);
			LB_PARSE_DICT_SWITCH(BlueValues);
			LB_PARSE_DICT_SWITCH(OtherBlues);
			LB_PARSE_DICT_SWITCH(FamilyBlues);
			LB_PARSE_DICT_SWITCH(FamilyOtherBlues);
			LB_PARSE_DICT_SWITCH(StdHW);
			LB_PARSE_DICT_SWITCH(StdVW);
		case lb::cff::DICT_OPERATOR::escape:
			de.op = val;
			val = r_idx.data[++i];
			switch (val)
			{
				LB_PARSE_DICT_SWITCH2(Copyright);
				LB_PARSE_DICT_SWITCH2(isFixedPitch);
				LB_PARSE_DICT_SWITCH2(ItalicAngle);
				LB_PARSE_DICT_SWITCH2(UnderlinePosition);
				LB_PARSE_DICT_SWITCH2(UnderlineThickness);
				LB_PARSE_DICT_SWITCH2(PaintType);
				LB_PARSE_DICT_SWITCH2(CharstringType);
				LB_PARSE_DICT_SWITCH2(FontMatrix);
				LB_PARSE_DICT_SWITCH2(StrokeWidth);
				LB_PARSE_DICT_SWITCH2(BlueScale);
				LB_PARSE_DICT_SWITCH2(BlueShift);
				LB_PARSE_DICT_SWITCH2(BlueFuzz);
				LB_PARSE_DICT_SWITCH2(StemSnapH);
				LB_PARSE_DICT_SWITCH2(StemSnapV);
				LB_PARSE_DICT_SWITCH2(ForceBold);
				LB_PARSE_DICT_SWITCH2(LanguageGroup);
				LB_PARSE_DICT_SWITCH2(ExpansionFactor);
				LB_PARSE_DICT_SWITCH2(initialRandomSeed);
				LB_PARSE_DICT_SWITCH2(SyntheticBase);
				LB_PARSE_DICT_SWITCH2(PostScript);
				LB_PARSE_DICT_SWITCH2(BaseFontName);
				LB_PARSE_DICT_SWITCH2(BaseFontBlend);
				LB_PARSE_DICT_SWITCH2(ROS);
				LB_PARSE_DICT_SWITCH2(CIDFontVersion);
				LB_PARSE_DICT_SWITCH2(CIDFontRevision);
				LB_PARSE_DICT_SWITCH2(CIDFontType);
				LB_PARSE_DICT_SWITCH2(CIDCount);
				LB_PARSE_DICT_SWITCH2(UIDBase);
				LB_PARSE_DICT_SWITCH2(FDArray);
				LB_PARSE_DICT_SWITCH2(FDSelect);
				LB_PARSE_DICT_SWITCH2(FontName);
			default:
				number = DecodeInt(r_idx, i);
				de.bytes.push_back(number);
				break;
			}
			break;
			LB_PARSE_DICT_SWITCH(UniqueID);
			LB_PARSE_DICT_SWITCH(XUID);
			LB_PARSE_DICT_SWITCH(charset);
			LB_PARSE_DICT_SWITCH(Encoding);
			LB_PARSE_DICT_SWITCH(CharStrings);
			LB_PARSE_DICT_SWITCH(Private);
			LB_PARSE_DICT_SWITCH(Subrs);
			LB_PARSE_DICT_SWITCH(defaultWidthX);
			LB_PARSE_DICT_SWITCH(nominalWidthX);
		case lb::cff::DICT_OPERATOR::shortint:
			number = DecodeInt(r_idx, i);
			de.bytes.push_back(number);
			break;
		case lb::cff::DICT_OPERATOR::longint:
			number = DecodeInt(r_idx, i);
			de.bytes.push_back(number);
			break;
		case lb::cff::DICT_OPERATOR::BCD:
			real = DecodeRealNumber(r_idx, i);
			de.bytes.push_back(real);
			break;
		default:
			number = DecodeInt(r_idx, i);
			de.bytes.push_back(number);
			break;
		}
	}

#undef LB_PARSE_DICT_SWITCH
#undef LB_PARSE_DICT_SWITCH2
}

void lb::CFF::ParseCharset(Charsets& r_chrst, int v_start)
{
	r_chrst.format = ByteToVal<unsigned char>(&this->bytes[v_start], v_start);

	if (r_chrst.format == 0)
	{
		for (int i = 0; i < this->nGlyphs - 1; i++)
		{
			unsigned short elem = ByteToVal<unsigned short>(&this->bytes[v_start], v_start);
			r_chrst.glyphRange.push_back(elem);
		}
	}
	else if (r_chrst.format == 1)
	{
		// UNTESTED
		lb::CFF::Charsets::R1 elem = { 0, 1 };
		while (elem.nLeft != 0)
		{
			elem.first = ByteToVal<unsigned short>(&this->bytes[v_start], v_start);
			elem.nLeft = ByteToVal<unsigned char>(&this->bytes[v_start], v_start);
			r_chrst.glyphRange.push_back(elem);
		}
	}
	else if (r_chrst.format == 2)
	{
		// UNTESTED
		lb::CFF::Charsets::R2 elem = { 0, 1 };
		while (elem.nLeft != 0)
		{
			elem.first = ByteToVal<unsigned short>(&this->bytes[v_start], v_start);
			elem.nLeft = ByteToVal<unsigned char>(&this->bytes[v_start], v_start);
			r_chrst.glyphRange.push_back(elem);
		}
	}
}

int lb::CFF::DecodeInt(Index& idx, int& i)
{
	int old = i;

	if (idx.data[old] >= 32 && idx.data[old] <= 246)
	{
		return (int)idx.data[old] - 139;
	}
	else if (idx.data[old] >= 247 && idx.data[old] <= 250)
	{
		i++;
		return ((int)idx.data[old] - 247) * 256 + (int)idx.data[old + 1] + 108;
	}
	else if (idx.data[old] >= 251 && idx.data[old] <= 254)
	{
		i++;
		return -((int)idx.data[old] - 251) * 256 - (int)idx.data[old + 1] - 108;
	}
	else if (idx.data[old] == 28)
	{
		i += 2;
		short bounds = (short)((int)idx.data[old + 1] << 8 | (int)idx.data[old + 2]);
		return bounds;
	}
	else if (idx.data[old] == 29)
	{
		i += 4;
		return ((int)idx.data[old + 1] << 24 | (int)idx.data[old + 2] << 16 | (int)idx.data[old + 3] << 8 | (int)idx.data[old + 4]);
	}

	return idx.data[old];
}

int lb::CFF::DecodeInt(std::vector<unsigned char>& idx, int& i)
{
	int old = i;

	if (idx[old] >= 32 && idx[old] <= 246)
	{
		return (int)idx[old] - 139;
	}
	else if (idx[old] >= 247 && idx[old] <= 250)
	{
		i++;
		return ((int)idx[old] - 247) * 256 + (int)idx[old + 1] + 108;
	}
	else if (idx[old] >= 251 && idx[old] <= 254)
	{
		i++;
		return -((int)idx[old] - 251) * 256 - (int)idx[old + 1] - 108;
	}
	else if (idx[old] == 28)
	{
		i += 2;
		short bounds = (short)((int)idx[old + 1] << 8 | (int)idx[old + 2]);
		return bounds;
	}
	else if (idx[old] == 29)
	{
		i += 4;
		return ((int)idx[old + 1] << 24 | (int)idx[old + 2] << 16 | (int)idx[old + 3] << 8 | (int)idx[old + 4]);
	}

	return idx[old];
}

double lb::CFF::DecodeRealNumber(Index& idx, int& i)
{
	std::string s;
	for (int nibble = 0; true; nibble++)
	{
		unsigned char op = (nibble % 2 == 0) ? idx.data[++i] >> 4 : idx.data[i] & 0b00001111; // MSB : LSB

		//0 – 9 0 – 9 a . (decimal point) b E c E– d <reserved> e –(minus) f end of number

		if (op >= 0 && op <= 9) s += std::to_string((int)op);
		if (op == 0xa) s += ".";
		if (op == 0xb) s += "E";
		if (op == 0xc) s += "E-";
		//if (op == 0xd) RESERVED;
		if (op == 0xe) s += "-";
		if (op == 0xf) break;
	}
	return std::stod(s);
}

lb::CFF::CFF()
	: bytes({}), nGlyphs(this->CharstringIndex.count) { }

lb::CFF::CFF(std::vector<unsigned char>& v_bytes)
	: bytes(v_bytes), nGlyphs(this->CharstringIndex.count)
{
	// Header
	this->Head.major = this->bytes[0];
	this->Head.minor = this->bytes[1];
	this->Head.hdrSize = this->bytes[2];
	this->Head.offSize = this->bytes[3];

	// Name Index
	int NameIndexStart = this->Head.hdrSize;
	int TopDictIndexStart = this->MakeIndex(this->NameIndex, NameIndexStart);

	// TopDict Index
	int GlobalSubrIndexStart = this->MakeIndex(this->TopDictIndex, TopDictIndexStart);

	this->ParseDict(this->TopDictIndex, this->TopDictElements);

	// GlobalSubr Index
	this->MakeIndex(this->GlobalSubrIndex, GlobalSubrIndexStart);

	// PrivateDict Index
	int PrivateDictIndexStart = (int)std::get<long long>(this->TopDictElements["Private"].bytes.back());
	long long PrivateDictIndexSize = std::get<long long>(this->TopDictElements["Private"].bytes.front());

	this->PrivateDictIndex.count = 1;
	this->PrivateDictIndex.offSize = this->Head.offSize;
	this->PrivateDictIndex.offset.push_back(0);
	this->PrivateDictIndex.offset.push_back((unsigned int)PrivateDictIndexSize + 1);

	this->MakeIndexData(this->PrivateDictIndex, PrivateDictIndexStart);

	this->ParseDict(this->PrivateDictIndex, this->PrivateDictElements);

	// LocalSubr Index (if exists)
	if (!this->PrivateDictElements["Subrs"].bytes.empty())
	{
		int LocalSubrIndexStart = PrivateDictIndexStart + PrivateDictIndex.GetDataStart() + (int)std::get<long long>(this->PrivateDictElements["Subrs"].bytes.front());

		this->MakeIndex(this->LocalSubrIndex, LocalSubrIndexStart);
	}

	// Charstring Index
	long long CharstringsIndexStart = std::get<long long>(this->TopDictElements["CharStrings"].bytes.front());

	this->MakeIndex(this->CharstringIndex, CharstringsIndexStart);

	// Charsets
	long long CharsetsStart = std::get<long long>(this->TopDictElements["charset"].bytes.front());

	this->ParseCharset(this->Charset, CharsetsStart);

	// Font Bounds
	this->FontBBox.minX = (int)std::get<long long>(this->TopDictElements["FontBBox"].bytes[0]);
	this->FontBBox.minY = (int)std::get<long long>(this->TopDictElements["FontBBox"].bytes[1]);
	this->FontBBox.maxX = (int)std::get<long long>(this->TopDictElements["FontBBox"].bytes[2]);
	this->FontBBox.maxY = (int)std::get<long long>(this->TopDictElements["FontBBox"].bytes[3]);
}

std::vector<unsigned char> lb::CFF::GetPathAt(int v_id)
{
	std::vector<unsigned char> demo;
	for (int i = this->CharstringIndex.offset[v_id] - 1; i < this->CharstringIndex.offset[v_id + 1] - 1; i++)
		demo.push_back(this->CharstringIndex.data[i]);

	return demo;
}

lb::CFF::BoundsBox lb::CFF::GetFontBounds()
{
	return this->FontBBox;
}

int lb::CFF::Index::GetDataStart()
{
	// short + char + offset.size()
	return 2 + 1 + offset.size();
}
