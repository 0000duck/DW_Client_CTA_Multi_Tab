/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_vr.h
  author: 	 zhangjian
  Brief:	 ��ʾһ�������VRͼ��
			 VRͼ������ж����ؽ��������ɲ�ͬ����Ⱦ����renderer��ʵ��

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "imaging_3d.h"
#include "vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VRImaging : public IThreedImaging
		{
		public:
			VRImaging(string id);
			~VRImaging();

			/// ��ȡԭʼ��άͼ�����ݳ�
			VolData* GetData() override;
			/// ����ԭʼ��άͼ�����ݳ� 
			void SetData(VolData* data) override;
			/// ��ȡ�����ʾͼ��
			ShowBuffer* GetShowBuffer() override;
			/// ��Ⱦͼ��
			void Render() override;
			/// ����
			void Zoom(float scale) override;
			/// ��������Ӧ����
			void ZoomToFitWindow() override;
			/// �ƶ�
			void Move(float dx, float dy) override;
			/// ƽ��
			void Move(float motionVector[3]) override;
			/// ƽ������ת
			void Rotate(float angle) override;
			/// ������������תһ���Ƕ�
			void Rotate3D(Vector3f axis, float angle) override;
			/// ����λ
			void WindowWidthLevel(int width, int level) override;
			void WindowWidthLevelOffset(int offset_width, int offset_level) override;
			/// ����renderer
			void SetRenderer(IThreedRenderer* renderer) override;

		protected:
			void ConvertToHBITMAP() override;

		private:


		};
	}
}