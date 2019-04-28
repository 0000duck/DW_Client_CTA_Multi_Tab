/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_3d.h
  author: 	 zhangjian
  Brief:	 ��ʾһ����άͼ��������ά���ݣ�ִ�о������Ⱦ���̣��������ʾ���
			 �ṩ���ڲ���ͼ��Ľӿڣ�����ת/����/ƽ��/����λ��

=========================================================================*/
#pragma once

#include "data_definition.h"
//#include "imaging_base.h"
#include "vol_data.h"
#include "renderer_3d.h"
#include "render_param.h"
#include "show_buffer.h"
#include "vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class IThreedImaging //: public DWImaging
		{
		public:
			// ��ȡͼ������
			virtual RenderMode GetRenderMode() = 0;
			// ��ȡԭʼ��άͼ�����ݳ�
			virtual VolData* GetData() = 0;
			// ��ȡ�����ʾͼ��
			virtual ShowBuffer* GetShowBuffer() = 0;
			// ����ԭʼ��άͼ�����ݳ� 
			virtual void SetData(VolData* data) = 0;

			// ��Ⱦͼ��
			virtual void Render() = 0;
			// ����
			virtual void Zoom(int scale) = 0;
			// �ƶ�
			virtual void Move(int distance) = 0;
			// ƽ������ת
			virtual void Rotate(float angle) = 0;
			// ������������תһ���Ƕ�
			virtual void Rotate3D(Vector3d axis, float angle) = 0;
			// ����λ
			virtual void WindowWidthLevel(int width, int level) = 0;
			// ����renderer
			virtual void SetRenderer(IThreedRenderer* renderer) {renderer_=renderer;}
			// ��ȡrenderer
			virtual IThreedRenderer* GetRenderer() {return renderer_;}
		protected:
			// ͼ������
			RenderMode render_mode_;
			// ��ά����
			VolData* volume_data_;
			// ִ����Ⱦ�Ķ���
			IThreedRenderer* renderer_;
			// ����UI��ʾ�Ľ��
			ShowBuffer* show_buffer_;

		private:


		};
	}
}