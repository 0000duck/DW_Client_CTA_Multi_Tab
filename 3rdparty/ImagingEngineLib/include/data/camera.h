/*=========================================================================

  Program:   ImagingEngine
  Module:    camera.h
  author: 	 zhangjian
  Brief:	 ����࣬��װ��vtkCamera�Լ���������ӿ�
			 �ο���https://blog.csdn.net/wzheng92/article/details/79935059

=========================================================================*/
#pragma once

#include <vtkCamera.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include "data_definition.h"
#include "vector3.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		
		///
		// ����࣬��װ��vtkCamera�Լ���������ӿ�
		// �ο���https://blog.csdn.net/wzheng92/article/details/79935059
		///		
		class Camera
		{
		public:
			Camera();
			~Camera();
			// ����������Ϸ�����ת
			void Rotate(float angle);
			// ����ָ������ת
			void Rotate(Vector3d axis, float angle);
			// ����ƽ������ת
			void Rotate(float x, float y, float angle);
			// ��ȡ���
			vtkSmartPointer<vtkCamera> GetCamera();
			// �������
			void DeepCopy(Camera*);
		private:
			void SetCamera(vtkSmartPointer<vtkCamera> camera);

		private:
			// vtk�����
			vtkSmartPointer<vtkCamera> vtk_camera_;
			// �������ƽ��
			vtkSmartPointer<vtkPlane> view_plane_;
			// �������λ�ã�������ƽ�����ĵ�
			Point3d focal_point_;
			// ���ĵ�λ��
			Point3d position_;
			// ����ƽ�����ĵ�: �����һ��offsetƫ����
			// ������������ͼ��������ģ�Focal Point�����ڴ��ڵ����ģ���ôWindowCenter����(0, 0)
			// WindowCenter�����������������ϵġ�������ƽ�������ϵ�������������µ�
			// ƽ�ƵĶ�����ͼ�񣬶���������ά�ռ��ƶ����
			Point3d window_center_;
			// ͼ���������
			Vector3d view_up_;
			// �ӽǣ�Ĭ����30��
			// ��������ͼ�������ݵı�����С��ͨ��������һ��������ʵ��ͼ��ķ���
			float view_angle_;
			// ���ΪTrue��ô����ƽ��ͶӰ������Ⱦ������Ĭ���ǰ���͸��ͶӰPerspectiveProjection������Ⱦ
			bool parallel_projection_;
			// �ӹ���λ��Positionָ�򽹵�λ��FocalPoint
			Vector3d direction_of_projection_;
			// ͶӰ�棨�����棩�ķ���������direction_of_projection_ʸ�������෴
			Vector3d view_plane_normal_;
			// ����
			float distance_;
			// �������������
			vtkSmartPointer<vtkMatrix4x4> view_transform_;
			// Ӧ�����������ı任����
			vtkSmartPointer<vtkMatrix4x4> camera_light_transform_;
		private:

		};
	}
}