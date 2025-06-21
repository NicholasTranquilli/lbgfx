#include "lbString2D.h"

void lb::String2D::Build()
{
	// TODO: Make values in relation to the String matrix

	float xSpacing = -1;
	lb::Matrix4f4 mx = lb::Matrix4f4::Identity();
	mx[0][0] = 0.4;
	mx[1][1] = 0.4;

	for (auto& i : this->vStrContents)
	{
		if (i == '\n')
		{
			// Subtract y pos
			mx[3][1] -= mx[1][1];
			// reset x pos
			xSpacing = -1.0f;
			continue;
		}
		if (i == ' ')
		{
			xSpacing += mx[0][0];
			continue;
		}

		lb::SotfP::DrawableData data = this->vFont.GetDrawableData(i);

		std::vector<VertexDefaultUI> vertices;
		for (const auto& i : data.vertices)
			vertices.push_back({i.x, i.y});

		std::vector<unsigned short> indices;
		for (const auto& i : data.indices)
			indices.push_back(i);

		this->vPathContents.push_back(std::make_unique<Path>(i, vertices, indices));

		// Apply spacing to most recent char
		mx[3][0] = xSpacing;
		this->vPathContents.back()->SetTransformMatrix(mx);
		xSpacing += data.width * mx[0][0];
	}
}

lb::String2D::String2D(std::string v_fontPath)
	: vFont(v_fontPath, lb::SotfP::Processing_type::Memoize)
{
	// TODO: (reminder)
	// Should not have to emplace containers since they dont actually draw to the screen
	// "Non visible containers" anyway
	// You "can" safely call emplace, however, since there are no const buffers or pipeline components, it wont do anything
	// Let the path handle updating and this container handles access to the paths
	//this->Emplace(); 
	this->Build();
}

lb::String2D::String2D(std::string v_fontPath, std::string v_strContents)
	: vFont(v_fontPath, lb::SotfP::Processing_type::Memoize), vStrContents(v_strContents)
{
	this->Build();
}

void lb::String2D::operator=(std::string v_strContents)
{
	this->SetString(v_strContents);
}

void lb::String2D::SetString(std::string v_strContents)
{
	this->vStrContents = v_strContents;
}
