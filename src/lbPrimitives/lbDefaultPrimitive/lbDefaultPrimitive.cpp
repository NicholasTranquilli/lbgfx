#include "lbDefaultPrimitive.h"

void lb::IDefaultPrimitive::SetTransformMatrix(lb::Matrix4f4 v_transformationMatrix)
{
	this->vTransformationMatrix = v_transformationMatrix;
	auto vConstBuffer = this->GetConstBuffer<lb::Matrix4f4, lb::ShaderType::VS>(0);

	if (vConstBuffer != nullptr) 
		vConstBuffer->SetData(
			(
				this->vTransformationMatrix *
				lb::LB_ENGINE_NAME.GetCamera().GetCameraxMx() *
				lb::MXMatrixPerspectiveLH(1.0f, (float)lb::LB_WINDOW_NAME.GetInverseAspectRatio(), lb::LB_ENGINE_NAME.GetNearZ(), lb::LB_ENGINE_NAME.GetFarZ())
			).Transpose()
		);
}

void lb::IDefaultPrimitive::SetUiTransformMatrix(lb::Matrix4f4 v_transformationMatrix)
{
	this->vTransformationMatrix = v_transformationMatrix;
	auto vConstBuffer = this->GetConstBuffer<lb::Matrix4f4, lb::ShaderType::VS>(0);

	if (vConstBuffer != nullptr)
		vConstBuffer->SetData(
			(
				this->vTransformationMatrix *
				lb::MXMatrixPerspectiveLH(1.0f, (float)lb::LB_WINDOW_NAME.GetInverseAspectRatio(), lb::LB_ENGINE_NAME.GetNearZ(), lb::LB_ENGINE_NAME.GetFarZ())
			).Transpose()
		);
}

void lb::IDefaultPrimitive::SetColor(lb::sColor4f v_color)
{
	this->vColor = v_color;
	auto vConstBuffer = this->GetConstBuffer<lb::sColor4f, lb::ShaderType::VS>(1);

	if (vConstBuffer != nullptr)
		vConstBuffer->SetData(this->vColor);
}
