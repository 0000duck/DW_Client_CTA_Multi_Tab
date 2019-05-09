/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_3d.h
  author: 	 zhangjian
  Brief:	 ������Ⱦ�ӿڣ������ǽ�����ά���ݣ�ִ��Render�����������ʾ���ShowBuffer����

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "render_param.h"
#include "vol_data.h"
#include "camera.h"
#include "light.h"
#include "render/pick_3d.h"
#include "render/measure_3d.h"
#include "render/orientation_3d.h"
#include "data/show_buffer.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{
		///
		// �������ݣ�render�����pixeldata
		///
		class IThreedRenderer
		{
		public:
			//TODO: change name to setinput 
			virtual void SetData(VolData* data) = 0;
			virtual void Render() = 0;
			virtual ShowBuffer* GetShowBuffer() = 0;
			virtual void SetRenderParam(RenderParam* param) { render_param_ = param; }
			virtual RenderParam* GetRenderParam() { return render_param_; }
			virtual Camera* GetCamera() { return camera_; }
			virtual RenderMode GetRenderMode() 
			{
				return render_mode_;
			}
			/// Convert world coordinate to display coordinate
			virtual void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) = 0;
			virtual void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) = 0;

			///
			// ���ظ���handler
			///
			virtual IThreedPickHandler* GetThreedPickHandler() { return NULL; }
			virtual IThreedMeasureHandler* GetThreedMeasureHandlerr() { return NULL; }
			virtual IThreedOrientationHandler* GetThreedOrientationHandlerr() { return NULL; }

			/// Ӧ�ô��ڴ�λ/����/ƽ��/
			virtual void BufferTransform() {}

		protected:
			// ͼ����Ⱦ��ʽ
			RenderMode render_mode_;
			// ԭʼ������
			VolData* volume_data_;
			// ���Ʋ���
			RenderParam* render_param_;
			// ͼ����ʾ����
			ShowBuffer* show_buffer_;
			// ���
			Camera* camera_;
			// �ƹ�
			Light* light_;
			bool is_first_render_;
			// �洢����ͼƬ�ĵ�ַ
			string dump_file_name_;

		private:


		};
	}
}