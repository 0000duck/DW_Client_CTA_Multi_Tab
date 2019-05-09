/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_raycasting_cpu.h
  author: 	 zhangjian
  Brief:	 VRͼ���raycasting�ؽ�������Ⱦ�࣬�ǻ���cpu����ģ����ܵ�

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "renderer_3d.h"
#include "pick_3d.h"
#include "measure_3d.h"
#include "orientation_3d.h"
#include "data/pixel_data.h"
#include "render/transfer_function.h"
#include "render/renderer_vr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RayCastingRenderer : public VolumeRenderer
		{
		public:
			RayCastingRenderer();
			virtual ~RayCastingRenderer();
			
		protected:
			virtual void DoRender(vtkSmartPointer<vtkImageData> imagedata) override;
			void DoRender2(vtkSmartPointer<vtkImageData> imagedata);

		};
	}
}