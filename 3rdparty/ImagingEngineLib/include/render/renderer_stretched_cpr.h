/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_stretched_cpr.h
  author: 	 zhangjian
  Brief:	 CPRͼ��Stretched������Ⱦ��

=========================================================================*/
#pragma once
#include "vtk_include.h"
#include "data_definition.h"
#include "render/renderer_cpr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render {
		///
		// ƽ������CPR
		///
		class StretchedCPRRenderer : public CPRRenderer
		{
		public:
			StretchedCPRRenderer();
			~StretchedCPRRenderer();
		protected:
			void DoRender() override;
		private:
		};

	}
}