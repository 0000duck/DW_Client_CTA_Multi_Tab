/****************************************************************************************************

 * \file image_process.h
 * \date 2019/05/06 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++���������ͷ�ļ�
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream> 

class ImageProcessBase
{
public:
	ImageProcessBase();
	~ImageProcessBase();

	virtual bool Excute(std::string& out_image_data);

protected:	
	// opencv Mat��base64�Ļ�ת
	static std::string base64Decode(const char* Data, int DataByte);
	static std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	cv::Mat Base2Mat(std::string &base64_data);
};

//////////////////////////////////////////////////////////////////////////
class ImageZoomProcess : public ImageProcessBase
{
public:
	ImageZoomProcess(std::string str_rate, std::string& in_image_data);
	~ImageZoomProcess();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	std::string m_str_rate;			// ���ű���
	std::string m_in_image_data;	// ͼ��ԭʼ���ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
class ImageRotateProcess : public ImageProcessBase
{
public:
	ImageRotateProcess(std::string str_angel, std::string& in_image_data);
	~ImageRotateProcess();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	std::string m_str_angel;		// ��ת�Ƕ�
	std::string m_in_image_data;	// ͼ��ԭʼ���ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
// ƽ�ƣ���С����
class ImageMoveProcess1 : public ImageProcessBase
{
public:
	ImageMoveProcess1(std::string str_move_position, std::string& in_image_data);
	~ImageMoveProcess1();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	std::string m_str_move_position;// �ƶ����ص�
	std::string m_in_image_data;	// ͼ��ԭʼ���ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
// ƽ�ƣ�����ı�ͼ���С
class ImageMoveProcess2 : public ImageProcessBase
{
public:
	ImageMoveProcess2(std::string str_move_position, std::string& in_image_data);
	~ImageMoveProcess2();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	std::string m_str_move_position;// �ƶ����ص�
	std::string m_in_image_data;	// ͼ��ԭʼ���ݣ�base64����
};