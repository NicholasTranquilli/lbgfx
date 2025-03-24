#pragma once

#include "../lbUtils/lbUtils.h"

namespace lb
{
	class Camera
	{
		lb::Vector3f vPos;
		lb::Vector3f vOrientation;

	public:
		Camera();

		void SetCameraPos(lb::Vector3f v_pos);
		void SetCameraOrientation(lb::Vector3f v_orientation);
		lb::Matrix4f4 GetCameraxMx();
	};
}