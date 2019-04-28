/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_cpr.h
  author: 	 zhangjian
  Brief:	 ��ʾһ�������CPRͼ��
			 CPRͼ���ΪStretched(����CPR)��Straightened(��ֱCPR)����	������CPRͼ��ͨ����ͬ����Ⱦ����renderer��ʵ��
			 �ο���������
			 Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44

=========================================================================*/
#pragma once

#include "../data/data_definition.h"
#include "imaging_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class CPRImaging : public IThreedImaging
		{
		public:
			CPRImaging();
			~CPRImaging();
			// ��ȡͼ������
			RenderMode GetRenderMode() override {return RenderMode::CPR;}
			// ��ȡԭʼ��άͼ�����ݳ�
			VolData* GetData() override;
			// ����ԭʼ��άͼ�����ݳ� 
			void SetData(VolData* data) override;
			// ��ȡ�����ʾͼ��
			ShowBuffer* GetShowBuffer() override;
			// ��Ⱦͼ��
			void Render() override;
			// ����renderer
			void SetRenderer(IThreedRenderer* renderer) override;
			// ����
			void Zoom(int scale) override {}
			// �ƶ�
			void Move(int distance) override {}
			// ƽ������ת
			void Rotate(float angle) override {}
			// ������������תһ���Ƕ�
			void Rotate3D(Vector3d axis, float angle) override {}
			// ����λ
			void WindowWidthLevel(int width, int level) override {}
		protected:

		private:


		};
	}
}