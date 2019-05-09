/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vol_data.h"
#include "actor/actor_3d.h"
#include "data/render_mode.h"
#include "render/renderer_3d.h"

namespace DW {

	class RenderSource
	{
	public:
		~RenderSource() { instance_ = nullptr; }
	
		static RenderSource *Get();
		
		/// ��������(series_uid)�ʹ��ڱ��(control_id)��ȡIThreedImaging����
		IThreedImaging* GetRenderControl(string control_id);
		vector<IThreedImaging*> GetRenderControls() {return imaging_list_;}
		/// ����IThreedImaging����
		IThreedImaging* CreateRenderControl(string control_id, RenderControlType type);

	private:
		RenderSource() {}
		/// Create a renderer
		IThreedRenderer *CreateRenderer(RenderControlType type);

	private:
		static RenderSource *instance_;
		vector<IThreedImaging *> imaging_list_;
	};
	
}