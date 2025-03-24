#pragma once

#include "Matrix.h"

namespace lb
{
	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixPerspectiveLH(_T v_viewWidth, _T v_viewHeight, _T v_nearZ, _T v_farZ);

	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixLookAtLH(lb::Vector<_T, 3> v_cameraPosition, lb::Vector<_T, 3> v_focusPosition, lb::Vector<_T, 3> v_upDirection);

	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixTranslate(_T x, _T y, _T z);

	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixRotateX(_T v_angle);

	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixRotateY(_T v_angle);

	template <typename _T>
	lb::Matrix<_T, 4, 4> MXMatrixRotateZ(_T v_angle);

	template <typename _T>
	Matrix<_T, 4, 4> MXMatrixRotateYawPitchRoll(_T v_yaw, _T v_pitch, _T v_roll);

	template <typename _T, int length>
	float MXVectorDotProduct(Vector<_T, length> v_vec1, Vector<_T, length> v_vec2);

	template <typename _T>
	Vector<_T, 4> MXVector4MultiplyAdd(Vector<_T, 4> v_vec1, Vector<_T, 4> v_vec2, Vector<_T, 4> v_vec3);

	template <typename _T>
	Vector<_T, 3> MXVector3CrossProduct(Vector<_T, 3> v_vec1, Vector<_T, 3> v_vec2);

	template <typename _T>
	Vector<_T, 4> MXVector3Transform(Vector<_T, 3> v_vec1, Matrix<_T, 4, 4> v_mx1);

}

template <typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixPerspectiveLH(_T v_viewWidth, _T v_viewHeight, _T v_nearZ, _T v_farZ)
{
	if (v_nearZ <= 0.0)
		throw 1;

	_T vTwoNearZ = v_nearZ + v_nearZ;
	_T vRange = v_farZ / (v_farZ - v_nearZ);

	lb::Matrix4f4 vMx;
	vMx.Fill(0);
	vMx[0][0] = vTwoNearZ / v_viewWidth;
	vMx[1][1] = vTwoNearZ / v_viewHeight;
	vMx[2][2] = vRange;
	vMx[2][3] = 1.f;
	vMx[3][2] = -vRange * v_nearZ;

	return vMx;
}

template <typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixLookAtLH(lb::Vector<_T, 3> v_cameraPosition, lb::Vector<_T, 3> v_focusPosition, lb::Vector<_T, 3> v_upDirection)
{
	lb::Vector3f vFocusDirection = (v_focusPosition - v_cameraPosition).Normalize();

	lb::Vector3f vDirectionVectorX = lb::MXVector3CrossProduct(v_upDirection, vFocusDirection).Normalize();

	lb::Vector3f vUpVector = lb::MXVector3CrossProduct(vFocusDirection, vDirectionVectorX);

	v_cameraPosition = v_cameraPosition * -1;

	float vRCx = lb::MXVectorDotProduct(vDirectionVectorX, v_cameraPosition);
	float vRCy = lb::MXVectorDotProduct(vUpVector, v_cameraPosition);
	float vRCz = lb::MXVectorDotProduct(vFocusDirection, v_cameraPosition);

	lb::Matrix4f4 vMx = {
		{vDirectionVectorX[0],	vUpVector[0],	vFocusDirection[0],	0.f},
		{vDirectionVectorX[1],	vUpVector[1],	vFocusDirection[1],	0.f},
		{vDirectionVectorX[2],	vUpVector[2],	vFocusDirection[2],	0.f},
		{vRCx,					vRCy,			vRCz,				1.f},
	};

	return vMx;
}

template<typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixTranslate(_T v_x, _T v_y, _T v_z)
{
	return {
		{1,		0,		0,		0},
		{0,		1,		0,		0},
		{0,		0,		1,		0},
		{v_x,	v_y,	v_z,	1},
	};
}

template<typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixRotateX(_T v_angle)
{
	return {
		{1,		0,				0,				0},
		{0,		cos(v_angle),	-sin(v_angle),	0},
		{0,		sin(v_angle),	cos(v_angle),	0},
		{0,		0,				0,				1},
	};
}

template<typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixRotateY(_T v_angle)
{
	return {
		{cos(v_angle),	0,	sin(v_angle),	0},
		{0,				1,	0,				0},
		{-sin(v_angle),	0,	cos(v_angle),	0},
		{0,				0,	0,				1},
	};
}

template<typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixRotateZ(_T v_angle)
{
	return {
		{cos(v_angle),	-sin(v_angle),	0,	0},
		{sin(v_angle),	 cos(v_angle),	0,	0},
		{0,				0,				1,	0},
		{0,				0,				0,	1},
	};
}

template<typename _T>
lb::Matrix<_T, 4, 4> lb::MXMatrixRotateYawPitchRoll(_T v_yaw, _T v_pitch, _T v_roll)
{
	return lb::MXMatrixRotateZ(v_roll) * lb::MXMatrixRotateY(v_pitch) * lb::MXMatrixRotateX(v_yaw);
}

template<typename _T, int length>
float lb::MXVectorDotProduct(lb::Vector<_T, length> v_vec1, lb::Vector<_T, length> v_vec2)
{
	lb::Matrix<_T, length, 1> vTmpVec2 = v_vec2.Transpose();

	return (v_vec1 * vTmpVec2)[0][0];
}

template<typename _T>
lb::Vector<_T, 4> lb::MXVector4MultiplyAdd(lb::Vector<_T, 4> v_vec1, lb::Vector<_T, 4> v_vec2, lb::Vector<_T, 4> v_vec3)
{
	return Vector<_T, 4>({
		v_vec1[0] * v_vec2[0] + v_vec3[0],
		v_vec1[1] * v_vec2[1] + v_vec3[1],
		v_vec1[2] * v_vec2[2] + v_vec3[2],
		v_vec1[3] * v_vec2[3] + v_vec3[3],
		});
}

template<typename _T>
lb::Vector<_T, 3> lb::MXVector3CrossProduct(lb::Vector<_T, 3> v_vec1, lb::Vector<_T, 3> v_vec2)
{
	lb::Vector<_T, 3> vVector;

	vVector[0] = v_vec1[1] * v_vec2[2] - v_vec1[2] * v_vec2[1];
	vVector[1] = v_vec1[2] * v_vec2[0] - v_vec1[0] * v_vec2[2];
	vVector[2] = v_vec1[0] * v_vec2[1] - v_vec1[1] * v_vec2[0];

	return vVector;
}

template<typename _T>
lb::Vector<_T, 4> lb::MXVector3Transform(lb::Vector<_T, 3> v_vec1, lb::Matrix<_T, 4, 4> v_mx1)
{
	lb::Vector<_T, 4> vVecX = { v_vec1[0], v_vec1[0], v_vec1[0], v_vec1[0] };
	lb::Vector<_T, 4> vVecY = { v_vec1[1], v_vec1[1], v_vec1[1], v_vec1[1] };
	lb::Vector<_T, 4> vVecZ = { v_vec1[2], v_vec1[2], v_vec1[2], v_vec1[2] };

	lb::Vector<_T, 4> vResult = lb::MXVector4MultiplyAdd(vVecZ, v_mx1.GetRow(2), v_mx1.GetRow(3));
	vResult = lb::MXVector4MultiplyAdd(vVecY, v_mx1.GetRow(1), vResult);
	vResult = lb::MXVector4MultiplyAdd(vVecX, v_mx1.GetRow(0), vResult);

	return vResult;
}