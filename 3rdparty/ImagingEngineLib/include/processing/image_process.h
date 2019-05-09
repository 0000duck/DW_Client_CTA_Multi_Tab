/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include "data/data_definition.h"

//using namespace DW::IMAGE;

namespace DW {
	namespace CV {
		
		class ImageProcessBase
		{
		public:
			ImageProcessBase(HBITMAP& in_image_data, int width, int height, int bits);
			~ImageProcessBase();

			virtual bool Excute(HBITMAP& out_image_data) = 0;

		protected:	
			/// opencv Mat��Buffer�Ļ�ת
			void Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output);
			void Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img);
			/// 
			BOOL ImageProcessBase::HBitmapToMat(HBITMAP& _hBmp, cv::Mat& _mat);
			BOOL ImageProcessBase::MatToHBitmap(cv::Mat& _mat, HBITMAP& _hBmp);

			/// hbitmap convert to IplImage   
			IplImage *HBitmapToIplImage(HBITMAP hBmp);

		protected:
			/// ͼ��ԭʼ����
			HBITMAP in_image_data_;
			int image_width_;
			int image_height_;
			int bits_per_pixel_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ����ͼ��
		class ImageZoomProcess : public ImageProcessBase
		{
		public:
			ImageZoomProcess(HBITMAP& in_image_data, int width, int height, int bits, float scale);
			~ImageZoomProcess();

			/// ͼ�����ź�����
			bool Excute(HBITMAP& out_image_data) override; 

		private:
			/// ���ű���
			float scale_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ѡ��ͼ��
		class ImageRotateProcess : public ImageProcessBase
		{
		public:
			ImageRotateProcess(HBITMAP& in_image_data, int width, int height, int bits, float angle);
			~ImageRotateProcess();

			bool Excute(HBITMAP& out_image_data) override;

		private:
			/// ��ת�Ƕ�
			float angle_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ƽ��ͼ��
		class ImageMoveProcess : public ImageProcessBase
		{
		public:
			ImageMoveProcess(HBITMAP& in_image_data, int width, int height, int bits, float dx, float dy);
			~ImageMoveProcess();

			bool Excute(HBITMAP& out_image_data) override;

		private:
			/// ƽ��ƫ����x/y
			float offset_x_;
			float offset_y_;
		};
		
	}
}