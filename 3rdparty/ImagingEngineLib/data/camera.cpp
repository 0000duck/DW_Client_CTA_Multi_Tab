#pragma once

#include "camera.h"

using namespace DW::IMAGE;
using namespace std;


Camera::Camera()
{
	vtk_camera_ = vtkSmartPointer<vtkCamera>::New();
}
Camera::~Camera()
{
	vtk_camera_->Delete();
}

// ����ָ������ת
void Camera::Rotate(float angle)
{
	if (vtk_camera_){

		vtk_camera_->Azimuth(angle);
	}
}

// ����ָ������ת
void Camera::Rotate(Vector3d axis, float angle)
{
	if (vtk_camera_){

		//vtk_camera_->Azimuth(angle);
	}
}

// ����ƽ������ת
void Camera::Rotate(float x, float y, float angle)
{
	if (vtk_camera_){
		//vtk_camera_->Elevation(-90 + angle);
	}
}

void Camera::SetCamera(vtkSmartPointer<vtkCamera> camera) 
{
	vtk_camera_ = camera;
}
			
vtkSmartPointer<vtkCamera> Camera::GetCamera() 
{
	return vtk_camera_;
}

void Camera::DeepCopy(Camera* source)
{
	this->vtk_camera_->DeepCopy(source->GetCamera());
	//...
}