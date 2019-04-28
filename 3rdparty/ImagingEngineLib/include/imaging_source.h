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

namespace DW {

	class ImagingSource
	{
	public:
		~ImagingSource() { instance_ = nullptr; }
	
		static ImagingSource* Get() 
		{
			if (instance_ == nullptr)
				instance_ = new ImagingSource;
			return instance_;
		}
		// ��������(series_uid)��ͼ������(image_type)��ȡIThreedImaging����
		IThreedImaging* GetImaging(string series_uid, RenderMode image_type);
		map<RenderMode, IThreedImaging*> GetSeriesImaging(string series_uid);
	private:
		static ImagingSource* instance_;
		map<string, map<RenderMode, IThreedImaging*>> imaging_set_;
	};

}