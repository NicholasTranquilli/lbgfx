#include "lbCamera.h"

lb::Camera::Camera()
	: vPos({ 0, 0, 0}), vOrientation({ 0, 0, 0 })
{
}

void lb::Camera::SetCameraPos(lb::Vector3f v_pos)
{
	this->vPos = v_pos;
}

void lb::Camera::SetCameraOrientation(lb::Vector3f v_orientation)
{
	this->vOrientation = v_orientation;
}

lb::Matrix4f4 lb::Camera::GetCameraxMx()
{
	lb::Vector4f vLookVector = lb::MXVector3Transform(
		{0.f, 0.f, 1.f}, lb::MXMatrixRotateYawPitchRoll(this->vOrientation[0], this->vOrientation[1], this->vOrientation[2])
	);

	lb::Vector4f vUpVector = lb::MXVector3Transform(
		{ 0.f, 1.f, 0.f },
		lb::MXMatrixRotateYawPitchRoll(this->vOrientation[0], this->vOrientation[1], this->vOrientation[2])
	);

	return lb::MXMatrixLookAtLH(this->vPos, (Vector3f)(this->vPos + vLookVector.xyz()), vUpVector.xyz());
}
