#pragma once

#include "OTF/OTF.h"
#include <map>

namespace lb
{
	class SotfP
	{
	public:
		/// <summary>
		/// Structure containing glyph information for one element in a glyph
		/// </summary>
		struct PathElement
		{
			unsigned char op[2];
			std::vector<int> data;
		};

		/// <summary>
		/// Structure containing a vector of vertices and indices for use in graphical contexts
		/// </summary>
		struct DrawableData
		{
			typedef struct { float x, y; } Vertex;
			typedef int Index;

			float width;
			std::vector<Vertex> vertices;
			std::vector<Index> indices;
		};

		/// <summary>
		/// PreProcess: Process all drawable data on initialization
		/// Memoize:	Memoize drawable data after it is called by GetDrawableData for the first time
		/// None:		Perform no processing optimization
		/// </summary>
		enum Processing_type
		{
			PreProcess,
			Memoize,
			None
		};

	private:
		lb::OTF vOTF;
		std::map<char, DrawableData> vInternalStorage;
		Processing_type vProc;

		void AppendCubicBezier(std::vector<DrawableData::Vertex>& r_vertexData, DrawableData::Vertex& r_lastVertex, int precision, std::vector<int> v_points);

		void AppendIndexing(DrawableData& r_drawableData, int v_start = 0);

		float NormalizeValue(int v_value, int v_min, int v_max);

	public:
		/// <summary>
		/// Constructor to instantiate OTF class
		/// </summary>
		/// <param name="v_file">Path to OTF font file</param>
		/// <param name="v_proc">define how to optimize processing (PreProcess, Memoize, or None)</param>
		SotfP(std::string v_file, Processing_type v_proc);

		/// <summary>
		/// One of the main functions of this class.
		/// Directly query any character in the font and recieve its structured glyph data in return.
		/// </summary>
		/// <param name="v_character">character thats glyph data will be returned</param>
		/// <returns>Structured glyph data for supplied character</returns>
		std::vector<PathElement> GetGlyph(char v_character);

		/// <summary>
		/// Another main function of this class.
		/// Directly query any character in the font and recieve its vertex and index data (aka drawable data)
		/// Particularly useful in graphical applications where font rendering is needed
		/// </summary>
		/// <param name="v_character">character thats drawable data will be returned</param>
		/// <returns>Drawable data for supplied character</returns>
		DrawableData GetDrawableData(char v_character);

		/// <summary>
		/// Helper function
		/// Returns all possible glyph characters (via 2 byte id) availible within the selected font
		/// </summary>
		/// <returns>Array of avalible glyphs</returns>
		std::vector<unsigned short> GetAvalibleGlyphs();

		/// <summary>
		/// Parses raw glyph data into structured glyph data.
		/// This function is for debugging and special case situations.
		/// </summary>
		/// <param name="v_rawGlyph">Vector of raw glyph data bytes</param>
		/// <returns>Structured glyph data</returns>
		std::vector<PathElement> ParseRawGlyph(std::vector<unsigned char> v_rawGlyph);

		/// <summary>
		/// Parses raw drawable data into vertices and indices.
		/// This function is for debugging and special case situations.
		/// </summary>
		/// <param name="v_glyphData">Vector of structured glyph data</param>
		/// <param name="v_bezierPrecision">Number of vectors produced from bezier calculations (higher number = more detail, lower number = less computation)</param>
		/// <returns>Vector of vertices</returns>
		DrawableData ParseRawDrawableData(std::vector<PathElement> v_glyphData, int v_bezierPrecision);
	};
}