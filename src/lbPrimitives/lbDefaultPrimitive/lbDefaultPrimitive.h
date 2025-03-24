#pragma once

#include "../../lbDrawable/lbPipeline/lbPipelineObject.h"

namespace lb
{
	class IDefaultPrimitive : public IPipelineObject
	{
	protected:
		lb::Matrix4f4 vTransformationMatrix;
		lb::sColor4f vColor;

		virtual void Build() override = 0;

	public:
		void SetTransformMatrix(lb::Matrix4f4 v_transformationMatrix);
		void SetUiTransformMatrix(lb::Matrix4f4 v_transformationMatrix);
		void SetColor(lb::sColor4f v_color);
	};

	struct VertexDefault3D
	{
		struct 
		{
			float x;
			float y;
			float z;
		}pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	struct VertexDefaultUI
	{
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
}