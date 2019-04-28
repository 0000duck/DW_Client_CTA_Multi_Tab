/*=========================================================================

  Program:   ImagingEngine
  Module:    vol_data.h
  author: 	 zhangjian
  Brief:	 ��ά������

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "data_convert.h"
#include "data/box3.h"
#include "data/aabb.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{
		class BoundingBox;

		class VolData
		{
		public:
			VolData();
			~VolData();
			UNITDATA3D* GetData();
			vtkImageData* GetVtkData();
			BYTE GetBitsPerPixel() { return bits_per_pixel_; }
			void SetBitsPerPixel(BYTE bits);
			BYTE GetBitsStored() { return bits_stored_; }
			void SetBitsStored(BYTE bits);
			void SetData(UNITDATA3D* data);
			void SetMark(UNITMARK3D* mark);
			void SetVtkData(vtkImageData* data);
			int GetSliceWidth() { return slice_width_; }
			void SetSliceWidth(int width);
			int GetSliceHeight() { return slice_height_; }
			void SetSliceHeight(int height);
			int GetSliceCount() { return slice_count_; }
			void SetSliceCount(int count);
			int GetSliceFOVWidth() { return slice_fov_width_; }
			void SetSliceFOVWidth(int fov_w);
			int GetSliceFOVHeight() { return slice_fov_height_; }
			void SetSliceFOVHeight(int fov_h);
			void SetStudyInstanceUID(string uid);
			void SetSeriesInstanceUID(string uid);
			// ���ð�Χ�еĶ���
			void SetBoundingBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);
			// ��ȡ��ǰ��Χ�ж���
			BoundingBox* GetBoundingBox() { return bounding_box_; }
			// ������ָ���л�ȡָ������λ�õ�ֵ
			void *GetDataPointer(int x, int y, int z);
			// ��ȡ����ָ���׵�ַ
			void *GetDataPointer();
			// ���û�ȡ�����޸ı�ʶ
			void SetModified(bool flag) {modified_=flag;}
			bool GetModified() {return modified_;}
			void Destroy();

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			BYTE bits_stored_;					//bits stored, the max ct value is 4095 with bits stored value 12��and is 10000 with value 16
			UNITDATA3D* series_data_;			//3d pixel data of the series
			UNITMARK3D* series_mark_;			//3d mark data of the series
			vtkImageData* vtk_series_data;		//3d data with vtkImageData format 
			int slice_width_;					//value stored in tag Columns(0028,0011)
			int slice_height_;					//value stored in tag Rows(0028,0010)
			int slice_count_;					//slice count in the series
			float slice_fov_width_;				//Reconstruction Field of View (width), pixel space = fov(width) / columns
			float slice_fov_height_;			//Reconstruction Field of View (width), pixel space = fov(height) / rows
			string study_instance_uid_;			//study instance uid, stored in tag (0020,000D)
			string series_instance_uid_;		//series instance uid, stored in tag (0020,000E)
			BoundingBox *bounding_box_;			//
			int volume_id_;						//
			bool modified_;						//

		private:


		};

		class BoundingBox : public AABB
		{
		public:
			BoundingBox() {}
			~BoundingBox() {}
		private:
		};
	}
}