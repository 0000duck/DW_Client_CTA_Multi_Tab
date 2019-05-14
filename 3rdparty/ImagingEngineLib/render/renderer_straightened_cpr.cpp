/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_straightened_cpr.cpp
  author: 	 zhangjian
  Brief:	 ƽ������CPR��Ⱦ����
			 �㷨���̣�
			 1.���ո���Ȥ����(vector of interest)����cprͼ�������ȣ�
			 �߾��԰�Χ�з�ΧΪ׼�����Գ�ʼ����������ұ߾�����ʾ
			 2.������ת�Ƕȵ�sin��cos���Եõ��µľֲ�����ϵƽ��ķ��߷���
			 3.�ھֲ�����ϵƽ���ڣ����ŷ��߷������ÿ����ά�����
			 4.ʹ�������Բ�ֵ�������ά�����Ӧ��CTֵ
			 4.����2,3��4������γ���curve����Ϊ�߶ȵ�cprͼ��
			 5.Ӧ�ô���λ�����ͼ��
			 ͼ����ת��
			 �ϲ㴫����ת�Ƕȣ�ͨ�������2��������ת���ͼ��

  =========================================================================*/
#include "render/renderer_straightened_cpr.h"
#include "vtkImageEllipsoidSource.h"
#include "vtkLineSource.h"
#include "filter/spline_driven_image_slicer.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "render/render_param_cpr.h"
#include <vtkMath.h>
#include "tools/math.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::Render;

StraightededCPRRenderer::StraightededCPRRenderer()
{
	render_mode_ = RenderMode::STRAIGHTENED_CPR;
	show_buffer_ = new ShowBuffer();
}

StraightededCPRRenderer::~StraightededCPRRenderer()
{
	if (show_buffer_) {
		delete show_buffer_;
		show_buffer_ = NULL;
	}
}

void StraightededCPRRenderer::DoRender()
{
	CPRRenderParam* param_imp = dynamic_cast<CPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;
	VolCurve* curve = param_imp->GetCurve();
	if (curve == NULL) return;

	// ͼ������
	int image_width = volume_data_->GetSliceWidth();
	int image_height = volume_data_->GetSliceHeight();
	int image_count = volume_data_->GetSliceCount();

	float direction[3];
	param_imp->GetDirection(direction[0], direction[1], direction[2]);
	float left_distance = 0.0f, right_distance = 0.0f;
	CalculateImageSize(param_imp->GetCurve(), direction, 1.0, left_distance, right_distance);
	// �����һ�����������ұ߾�
	origin_padding_left_ = left_distance;
	origin_padding_right_ = right_distance;
	int width = (int)(left_distance + 0.5) + (int)(right_distance + 0.5);
	int height = curve->GetNumberOfSamplePoint();
	// ������ת���ĵ����ת��
	curve->GetCurveDirection(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	param_imp->SetAxis(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	float middle_sample[3] = {0};
	curve->GetSamplePoint(height / 2, middle_sample[0], middle_sample[1], middle_sample[2]);
	rotation_center_.x = middle_sample[0];
	rotation_center_.y = middle_sample[1];
	rotation_center_.z = middle_sample[2];
	param_imp->SetCenter(middle_sample[0], middle_sample[1], middle_sample[2]);
	// END

	int rows=height;
	int cols=width;

	show_buffer_->InitBufferData(cols, rows, 16);
	// ��vtk��ȡ��ImageData����ֵ�Ǿ���ƫ��ת���ģ���˴��ŷ��ŵģ����967����С-3024
	// ��˴˴���short����ȡֵ
	short* ct_buffer = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());

	double origins[3], spaces[3];
	int dim[3];
	volume_data_->GetPixelData()->GetVtkImageData()->GetOrigin(origins);
	volume_data_->GetPixelData()->GetVtkImageData()->GetSpacing(spaces);
	dim[0] = width;
	dim[1] = height;
	dim[2] = 1;
	short* ptr = NULL;
	VtkImageDataCreator imageDataCreator;
	imageDataCreator.SetOrigin(origins);
	imageDataCreator.SetSpacing(spaces);
	imageDataCreator.SetDimensions(dim);
	imageDataCreator.SetNumberOfComponents(1);
	output_vtk_image_data_ = imageDataCreator.Create(ptr);
	ptr = (short*)output_vtk_image_data_->GetScalarPointer();
	
	float angle = param_imp->GetAngle() * PI / 180;
	float fsin = sinf(angle);
	float fcos = cosf(angle);

	float delta = 1.0f;//���Ʒֱ��ʲ���
	float x[3];
	double distance_from_origin = 0.0;
	float origin[3], current_sample[3], projection[3];
	param_imp->GetCurve()->GetSamplePoint (0, origin[0], origin[1], origin[2]);
	int minval=100000, maxval = -100000;
	for (int row = 0; row < rows; row++)
	{
		param_imp->GetCurve()->GetSamplePoint (row, current_sample[0], current_sample[1], current_sample[2]);
		Vector3f normal, binormal;
		curve->GetLocalPlaneVector(row, normal, binormal);
		// ������Ҫ������ת�Ƕȣ�����Ϊʵ�ʵ�normal
		normal[0] = binormal[0] * fsin + normal[0] * fcos;
		normal[1] = binormal[1] * fsin + normal[1] * fcos;
		normal[2] = binormal[2] * fsin + normal[2] * fcos;

		for (int col = 0; col < cols; col++)
		{
			float half_width = -(col - 0.5 * cols) * delta;
			x[0] = current_sample[0] + half_width * normal[0];
			x[1] = current_sample[1] + half_width * normal[1];
			x[2] = current_sample[2] + half_width * normal[2];

			if (x[0] < 0 || x[0] > image_width - 1 || 
				x[1] < 0 || x[1] > image_height - 1 || 
				x[2] < 0 || x[2] > image_count - 1){
					continue;
			}

			short val = 0;
			TrilinearInterpolation(val, x[0], x[1], x[2], volume_data_);
			*(ptr + row * cols + col) = val;
		}
	}

	BufferTransform();
}