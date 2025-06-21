#include "OTF.h"
#include "../utils.h"

int lb::OTF::MakeTagId(char* tag)
{
    return lb::ByteToVal<int>((unsigned char*)tag);
}

void lb::OTF::ParseGlyphTable(lb::OTF::TableDirectory::TableRecord v_tr)
{
    vFile.seekg(v_tr.offset);
    std::vector<unsigned char> bytes(v_tr.length);
    vFile.read(reinterpret_cast<char*>(bytes.data()), v_tr.length);

    this->vCffTable = lb::CFF(bytes);
}

void lb::OTF::ParseMappingTable(lb::OTF::TableDirectory::TableRecord v_tr)
{
    vFile.seekg(v_tr.offset);
    std::vector<unsigned char> bytes(v_tr.length);
    vFile.read(reinterpret_cast<char*>(bytes.data()), v_tr.length);

    this->vCmapTable = lb::CMAP(bytes);
}

void lb::OTF::FillTableDirectory()
{
	vFile.read((char*)&this->vTableDirectory, lb::OTF::TableDirectory::PackedHeaderSize());
	lb_SwapEndien(this->vTableDirectory.sfntVersion);
	lb_SwapEndien(this->vTableDirectory.numTables);
	lb_SwapEndien(this->vTableDirectory.searchRange);
	lb_SwapEndien(this->vTableDirectory.entrySelector);
	lb_SwapEndien(this->vTableDirectory.rangeShift);

	for (int i = 0; i < this->vTableDirectory.numTables; i++)
	{
		lb::OTF::TableDirectory::TableRecord tr;
        vFile.read((char*)&tr, lb::OTF::TableDirectory::TableRecord::PackedSize());

		lb_SwapEndien(tr.checksum);
		lb_SwapEndien(tr.offset);
		lb_SwapEndien(tr.length);

		this->vTableDirectory.tableRecords.push_back(std::move(tr));
	}
}

void lb::OTF::ParseTable(lb::OTF::TableDirectory::TableRecord v_tr)
{
    switch (this->MakeTagId(v_tr.tag))
    {
    case lb::OTF::TableID::CFF:
        this->ParseGlyphTable(v_tr);
        break;
    case lb::OTF::TableID::GSUB:
        break;
    case lb::OTF::TableID::OS2:
        break;
    case lb::OTF::TableID::cmap:
        this->ParseMappingTable(v_tr);
        break;
    case lb::OTF::TableID::head:
        break;
    case lb::OTF::TableID::hhea:
        break;
    case lb::OTF::TableID::hmtx:
        break;
    case lb::OTF::TableID::maxp:
        break;
    case lb::OTF::TableID::name:
        break;
    case lb::OTF::TableID::post:
        break;
    default:
        break;
    }
}

lb::OTF::OTF(std::string v_fileName)
	: vFile(v_fileName, std::ios::in | std::ios::binary)
{
	this->FillTableDirectory();

	for (auto& i : this->vTableDirectory.tableRecords)
		this->ParseTable(i);
}

std::vector<unsigned char> lb::OTF::GetGlyph(char v_character)
{
	int vGid = this->vCmapTable.GetGlyphId(v_character);
	std::vector<unsigned char> vRawPath = this->vCffTable.GetPathAt(vGid);
	return vRawPath;
}

lb::CMAP& lb::OTF::GetCMAP()
{
    return this->vCmapTable;
}

lb::CFF& lb::OTF::GetCFF()
{
    return this->vCffTable;
}


/************************************/
//          PACKED SIZES            //
/************************************/


const int lb::OTF::TableDirectory::TableRecord::PackedSize()
{
    return 16;
}

const int lb::OTF::TableDirectory::PackedHeaderSize()
{
    return 12;
}

const int lb::OTF::Header::PackedSize()
{
    return 12;
}
