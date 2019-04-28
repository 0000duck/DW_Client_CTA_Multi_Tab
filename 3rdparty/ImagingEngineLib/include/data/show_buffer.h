/*=========================================================================

  Program:   ImagingEngine
  Module:    show_buffer.h
  author: 	 zhangjian
  Brief:	 UI��ʾ����࣬����UNITDATASHOWָ�����͵�buffer
			 UNITDATASHOW����Ӧ����Ϊ����������

=========================================================================*/
#pragma once

#include "data_definition.h"

using namespace std;

namespace DW {
	namespace IMAGE{

		///
		// show buffer is associated with one imaging object
		///
		class ShowBuffer
		{
		public:
			// ����һ��ָ����С��buffer
			void InitBufferData(int width, int height, BYTE bits_per_pixel);
			// ���ֽ����ݸ�ֵ����buffer��
			void SetBufferData(UNITDATASHOW* bytes, int width, int height, BYTE bits_per_pixel);
			// ��ȡ��ǰbuffer
			UNITDATASHOW* GetShowBuffer();
			// ���
			int GetWidth() {return buffer_width_;}
			// �߶�
			int GetHeight() {return buffer_height_;}
			// λ��
			BYTE GetBitsPerPixel() {return bits_per_pixel_;}
			// ͨ����
			BYTE GetNumberOfComponent() { return bits_per_pixel_ / 8;}
			
			// methods for testing
			void DumpBufferToFile(const char* file_path);

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			UNITDATASHOW* buffer_data_;			//pixel data for display
			int buffer_width_;					//width of the pixel data
			int buffer_height_;					//height of the pixel data
			float x_dpi_;						//indicate how long in mm unit is each pixel in x direction
			float y_dpi_;						//indicate how long in mm unit is each pixel in y direction
			string time_stamp_;					//time stamp uid

		private:


		};
	}
}