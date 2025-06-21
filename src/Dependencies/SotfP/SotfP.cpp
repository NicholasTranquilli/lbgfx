#include "SotfP.h"

void lb::SotfP::AppendCubicBezier(std::vector<DrawableData::Vertex>& r_vertexData, DrawableData::Vertex& r_lastVertex, int precision, std::vector<int> v_points)
{
	float rate = 1.f / ((precision <= 0) ? 1 : precision);

	struct { int x, y; } p1, p2, p3;

	p1.x = v_points[0];
	p1.y = v_points[1];

	p2.x = p1.x + v_points[2];
	p2.y = p1.y + v_points[3];

	p3.x = p2.x + v_points[4];
	p3.y = p2.y + v_points[5];

	for (float i = rate; i < 1.f; i += rate)
	{
		float L1x = std::lerp(p1.x, p2.x, i);
		float L1y = std::lerp(p1.y, p2.y, i);

		float L2x = std::lerp(p2.x, p3.x, i);
		float L2y = std::lerp(p2.y, p3.y, i);

		float Q1x = std::lerp(L1x, L2x, i);
		float Q1y = std::lerp(L1y, L2y, i);

		r_vertexData.push_back({
			Q1x + r_lastVertex.x,
			Q1y + r_lastVertex.y
			});
	}

	// Push final point
	r_vertexData.push_back({
		(float)p3.x + r_lastVertex.x,
		(float)p3.y + r_lastVertex.y
		});

	// Update relative position
	r_lastVertex = {
		(float)(p3.x + (int)r_lastVertex.x),
		(float)(p3.y + (int)r_lastVertex.y)
	};
}

void lb::SotfP::AppendIndexing(DrawableData& r_drawableData, int v_start)
{
	for (int i = v_start + 1; i < r_drawableData.vertices.size() - 1 && v_start < r_drawableData.vertices.size();)
	{
		r_drawableData.indices.push_back(v_start);
		r_drawableData.indices.push_back(i);
		r_drawableData.indices.push_back(++i);
	}
}

float lb::SotfP::NormalizeValue(int v_value, int v_min, int v_max)
{
	return (float)(v_value - v_min) / (float)v_max;
}

lb::SotfP::SotfP(std::string v_file, SotfP::Processing_type v_proc)
	: vOTF(v_file), vProc(v_proc)
{
	if (vProc == lb::SotfP::PreProcess)
	{
		// Loop all chars and call 'GetDrawableData(c)' + save results in vInternalStorage
	}
}

std::vector<lb::SotfP::PathElement> lb::SotfP::GetGlyph(char v_character)
{
	std::vector<unsigned char> vRawGlyph = this->vOTF.GetGlyph(v_character);
	std::vector<lb::SotfP::PathElement> vGlyph = this->ParseRawGlyph(vRawGlyph);
	return vGlyph;
}

lb::SotfP::DrawableData lb::SotfP::GetDrawableData(char v_character)
{
	std::map<char, DrawableData>::iterator it = vInternalStorage.find(v_character);
	if (it != vInternalStorage.end())
		return it->second;

	// Value was not found in internal storage
	lb::SotfP::DrawableData result;

	// Get glyph string
	std::vector<lb::SotfP::PathElement> vGlyph = this->GetGlyph(v_character);

	// Parse into absolute vector
	result = ParseRawDrawableData(vGlyph, 10);

	// Normalize vector
	lb::CFF::BoundsBox bounds = this->vOTF.GetCFF().GetFontBounds();
	for (auto& item : result.vertices)
	{
		item.x = this->NormalizeValue(item.x, bounds.minX, bounds.maxX);
		item.y = this->NormalizeValue(item.y, bounds.minY, bounds.maxY);
	}

	// Fix width
	result.width /= bounds.maxX;

	// If memoization is enabled
	if (this->vProc == lb::SotfP::Processing_type::Memoize)
		vInternalStorage.emplace(v_character, result);

	return result;
}

std::vector<unsigned short> lb::SotfP::GetAvalibleGlyphs()
{
	return this->vOTF.GetCMAP().GetAvailableChars();
}

std::vector<lb::SotfP::PathElement> lb::SotfP::ParseRawGlyph(std::vector<unsigned char> v_rawGlyph)
{
	std::vector<lb::SotfP::PathElement> vResult;
	std::vector<int> vDataBuffer;
	PathElement vPathElementBuffer;

#define LB_PARSE_CHAR_SWITCH(what)							\
case lb::cff::CFF_OPERATOR::what:							\
	vPathElementBuffer.op[0] = lb::cff::CFF_OPERATOR::what;	\
	vPathElementBuffer.op[1] = 0;							\
	vPathElementBuffer.data = vDataBuffer;					\
	vDataBuffer.clear();									\
	vResult.push_back(vPathElementBuffer);					\
	vPathElementBuffer = {0};								\
	break							

#define LB_PARSE_CHAR_SWITCH2(what)							\
case lb::cff::CFF_OPERATOR::what:							\
	vPathElementBuffer.op[1] = lb::cff::CFF_OPERATOR::what;	\
	vPathElementBuffer.data = vDataBuffer;					\
	vDataBuffer.clear();									\
	vResult.push_back(vPathElementBuffer);					\
	vPathElementBuffer = {0};								\
	break							

	for (int i = 0; i < v_rawGlyph.size(); i++)
	{
		int val = v_rawGlyph[i];
		switch (val)
		{
			LB_PARSE_CHAR_SWITCH(hstem);
			LB_PARSE_CHAR_SWITCH(vstem);
			LB_PARSE_CHAR_SWITCH(vmoveto);
			LB_PARSE_CHAR_SWITCH(rlineto);
			LB_PARSE_CHAR_SWITCH(hlineto);
			LB_PARSE_CHAR_SWITCH(vlineto);
			LB_PARSE_CHAR_SWITCH(rrcurveto);
			LB_PARSE_CHAR_SWITCH(callsubr);
			LB_PARSE_CHAR_SWITCH(sub_return);
		case lb::cff::CFF_OPERATOR::escape:
			vPathElementBuffer.op[0] = lb::cff::CFF_OPERATOR::escape;
			val = v_rawGlyph[++i];
			switch (val)
			{
				LB_PARSE_CHAR_SWITCH2(cff_and);
				LB_PARSE_CHAR_SWITCH2(cff_or);
				LB_PARSE_CHAR_SWITCH2(cff_not);
				LB_PARSE_CHAR_SWITCH2(cff_abs);
				LB_PARSE_CHAR_SWITCH2(cff_add);
				LB_PARSE_CHAR_SWITCH2(cff_sub);
				LB_PARSE_CHAR_SWITCH2(cff_div);
				LB_PARSE_CHAR_SWITCH2(cff_neg);
				LB_PARSE_CHAR_SWITCH2(cff_eq);
				LB_PARSE_CHAR_SWITCH2(cff_drop);
				LB_PARSE_CHAR_SWITCH2(cff_put);
				LB_PARSE_CHAR_SWITCH2(cff_get);
				LB_PARSE_CHAR_SWITCH2(cff_ifelse);
				LB_PARSE_CHAR_SWITCH2(cff_random);
				LB_PARSE_CHAR_SWITCH2(cff_mul);
				LB_PARSE_CHAR_SWITCH2(cff_sqrt);
				LB_PARSE_CHAR_SWITCH2(cff_dup);
				LB_PARSE_CHAR_SWITCH2(cff_exch);
				LB_PARSE_CHAR_SWITCH2(cff_index);
				LB_PARSE_CHAR_SWITCH2(cff_roll);
				LB_PARSE_CHAR_SWITCH2(hflex);
				LB_PARSE_CHAR_SWITCH2(flex);
				LB_PARSE_CHAR_SWITCH2(hflex1);
				LB_PARSE_CHAR_SWITCH2(flex1);
			default:
				vDataBuffer.push_back(this->vOTF.GetCFF().DecodeInt(v_rawGlyph, i));
				break;
			}
			break;
			LB_PARSE_CHAR_SWITCH(endchar);
			LB_PARSE_CHAR_SWITCH(hstemhm);
			LB_PARSE_CHAR_SWITCH(hintmask);
			LB_PARSE_CHAR_SWITCH(cntrmask);
			LB_PARSE_CHAR_SWITCH(rmoveto);
			LB_PARSE_CHAR_SWITCH(hmoveto);
			LB_PARSE_CHAR_SWITCH(vstemhm);
			LB_PARSE_CHAR_SWITCH(rcurveline);
			LB_PARSE_CHAR_SWITCH(rlinecurve);
			LB_PARSE_CHAR_SWITCH(vvcurveto);
			LB_PARSE_CHAR_SWITCH(hhcurveto);
		case lb::cff::CFF_OPERATOR::shortint:
			vDataBuffer.push_back(this->vOTF.GetCFF().DecodeInt(v_rawGlyph, i));
			break;
			LB_PARSE_CHAR_SWITCH(callgsubr);
			LB_PARSE_CHAR_SWITCH(vhcurveto);
			LB_PARSE_CHAR_SWITCH(hvcurveto);
		default:
			vDataBuffer.push_back(this->vOTF.GetCFF().DecodeInt(v_rawGlyph, i));
			break;
		}
	}

#undef LB_PARSE_CHAR_SWITCH
#undef LB_PARSE_CHAR_SWITCH2

	return vResult;
}

lb::SotfP::DrawableData lb::SotfP::ParseRawDrawableData(std::vector<lb::SotfP::PathElement> v_glyphData, int v_bezierPrecision)
{
	DrawableData result;

	int lastStart = 0;

	DrawableData::Vertex lastVertex = { 0, 0 };

	for (auto& i : v_glyphData)
	{
		int dataStart = 0;

		switch (i.op[0])
		{
		case lb::cff::CFF_OPERATOR::rrcurveto:
			AppendCubicBezier(
				result.vertices,
				lastVertex,
				v_bezierPrecision,
				i.data
			);
			break;

		case lb::cff::CFF_OPERATOR::endchar:
			AppendIndexing(result, lastStart);
			break;

		case lb::cff::CFF_OPERATOR::rmoveto:
			AppendIndexing(result, lastStart);
			lastStart = result.vertices.size();

			if (i.data.size() > 2)
				result.width = (float)i.data[0];

			dataStart = i.data.size() - 2;

			lastVertex = {
				(float)(i.data[dataStart] + (int)lastVertex.x),
				(float)(i.data[dataStart + 1] + (int)lastVertex.y)
			};
			result.vertices.push_back(lastVertex);
			break;

		case lb::cff::CFF_OPERATOR::rlineto:
			lastVertex = {
				(float)(i.data[0] + (int)lastVertex.x),
				(float)(i.data[1] + (int)lastVertex.y)
			};
			result.vertices.push_back(lastVertex);
			break;

		default:
			break;
		}
	}

	return result;
}