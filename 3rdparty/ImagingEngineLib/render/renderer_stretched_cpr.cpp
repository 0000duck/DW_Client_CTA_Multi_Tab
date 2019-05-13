/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_stretched_cpr.cpp
  author: 	 zhangjian
  Brief:	 ƽ������CPR��Ⱦ����
			 �㷨���̣�
			 1.���ո���Ȥ����(vector of interest)����cprͼ�������ȣ�
			 �߾��԰�Χ�з�ΧΪ׼�����Գ�ʼ����������ұ߾�����ʾ
			 2.����curve������ÿ�������㣬������õ㲢���Ÿ���Ȥ������߶���
			 ÿ�������ά����
			 3.ʹ�������Բ�ֵ�������ά�����Ӧ��CTֵ
			 4.����2��3������γ���curve����Ϊ�߶ȵ�cprͼ��
			 5.Ӧ�ô���λ�����ͼ��
			 ͼ����ת��
			 �ϲ㴫�����Ȥ���򣬲��Դ˼�����ת���ͼ��

=========================================================================*/
#include "render/renderer_stretched_cpr.h"
#include "vtkImageEllipsoidSource.h"
#include "vtkLineSource.h"
#include "filter/spline_driven_image_slicer.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "render/render_param_cpr.h"
#include <vtkMath.h>
#include "tools/math.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/timer.h"
#include "tools/logger.h"

using namespace DW::Render;

StretchedCPRRenderer::StretchedCPRRenderer()
{
	render_mode_ = RenderMode::STRETECHED_CPR;
	show_buffer_ = new ShowBuffer();
}

StretchedCPRRenderer::~StretchedCPRRenderer()
{
	if (show_buffer_) {
		delete show_buffer_;
		show_buffer_ = NULL;
	}
}

void StretchedCPRRenderer::DoRender()
{
	Timer::begin("CPR::DoRender");

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
	// ����ϵ��
	float scale = param_imp->GetScale();

	int width = ((int)(left_distance + 0.5) + (int)(right_distance + 0.5));
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

	///// ��ʱ�Է��������ֿ�߱�������ı���һ�£������ϲ����ͼ��
	//int image_width = param_imp->GetWidth();
	//int image_height = param_imp->GetHeight();
	//double ratio = (double)image_width / (double)image_height;
	//double tmp_ratio = (double)cols / (double)rows;
	//int actual_width = ratio >= tmp_ratio ? cols : (double)rows * ratio;
	//int actual_height = ratio >= tmp_ratio ? (double)cols / ratio : rows;

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
	//if (output_vtk_image_data_){
	//	output_vtk_image_data_->Delete();
	//}
	output_vtk_image_data_ = imageDataCreator.Create(ptr);
	ptr = (short*)output_vtk_image_data_->GetScalarPointer();

	float x[3];
	double distance_from_origin = 0.0;
	float origin[3], current_sample[3], projection[3];
	param_imp->GetCurve()->GetSamplePoint (0, origin[0], origin[1], origin[2]);
	int minval=100000, maxval = -100000;
	for (int row = 0; row < rows; row++)
	{
		param_imp->GetCurve()->GetSamplePoint (row, current_sample[0], current_sample[1], current_sample[2]);
		// ���curpnt������LineOri�Ϲ�prepnt���ͶӰ�����ꡢ
		MathTool::Compute3DProjectionOnLine (direction, origin, current_sample, projection);
		// ����ͶӰ����,ͬ��Ϊ��,����Ϊ��
		float tmp_distance = MathTool::ComputeDistanceBetweenPoints (origin, projection, direction);
		for (int col = 0; col < cols; col++)
		{
			distance_from_origin = (col - origin_padding_left_) + tmp_distance;
			float p[3];
			param_imp->GetCurve()->GetSamplePoint (row, p[0], p[1], p[2]);
			x[0] = p[0] + direction[0] * distance_from_origin;
			x[1] = p[1] + direction[1] * distance_from_origin;
			x[2] = p[2] + direction[2] * distance_from_origin;

			if (x[0] < 0 || x[0] > image_width - 1 || 
				x[1] < 0 || x[1] > image_height - 1 || 
				x[2] < 0 || x[2] > image_count - 1){
				continue;
			}

			short val = 0;
			TrilinearInterpolation(val, x[0], x[1], x[2], volume_data_);
			*(ptr + row * cols + col) = val;

			// ���ԣ��ҵ������СCTֵ
			if (val < minval)
				minval = val;
			if (val > maxval)
				maxval = val;
		}
	}

	BufferTransform();

	Timer::end("CPR::DoRender");
	CGLogger::Info(Timer::summery());
}
