#pragma once

// TODO: New category for Default Path types
#include "../../lbDrawable/lbPipeline/lbPipelineObject.h"
#include "../lbPrimitives/lbDefaultPrimitive/lbDefaultPrimitive.h"

namespace lb
{
	class Path : public lb::IDefaultPrimitive
	{
		std::vector<lb::VertexDefaultUI> vVertices;
		std::vector<unsigned short> vIndices;

	protected:
		void Build() override;

	public:
		Path(char c, std::vector<lb::VertexDefaultUI> v_vertices, std::vector<unsigned short> v_indices);
	};
}
