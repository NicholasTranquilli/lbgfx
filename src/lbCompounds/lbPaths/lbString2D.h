#pragma once

// TODO: New category for Default Path types
#include "../../lbDrawable/lbPipeline/lbPipelineObject.h"
#include "../lbPrimitives/lbDefaultPrimitive/lbDefaultPrimitive.h"

// Include lbPath
#include "lbPath.h"

// Include Simple OTF Parser
#include "../../Dependencies/SotfP/SotfP.h"

namespace lb
{
	// TODO: this is the first ever Compund Container type class, theyer should probably be a custom interface for things of this nature
	// Maybe it can wrap the interface that is provided via Template to utilize its methods?
	// Probably wont work cause the interface stuff is private, would need to inherit...
	// Containers could inherit both? but it would technically make it a drawable... Not necessarily a bad thing tho, just not technically true
	class String2D : public lb::IDefaultPrimitive
	{
		lb::SotfP vFont;
		std::string vStrContents;
		std::vector<std::unique_ptr<Path>> vPathContents;

	protected:
		void Build() override;

	public:
		String2D(std::string v_fontPath);
		String2D(std::string v_fontPath, std::string v_strContents);

		void operator=(std::string v_strContents);
		void SetString(std::string v_strContents);
	};
}
