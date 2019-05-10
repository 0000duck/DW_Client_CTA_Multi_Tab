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

#include "io/dcm_reader.h"

#define JSON_KEY_REQUEST_TYPE				"request_type"
#define JSON_KEY_IMAGE_OPERATION			"image_operation"
#define JSON_KEY_IMAGE_PARAS				"image_paras"
#define JSON_KEY_IMAGE_DATA					"image_data"

//  ��һ�� "��������"    ��ö��
#define JSON_VALUE_REQUEST_TYPE_MPR			"mpr"
#define JSON_VALUE_REQUEST_TYPE_VR			"vr"
#define JSON_VALUE_REQUEST_TYPE_MIP			"mip"
#define JSON_VALUE_REQUEST_TYPE_CPR			"cpr"

//  �ڶ��� "������������"��ö��
#define JSON_VALUE_IMAGE_OPERATION_ZOOM		"zoom"
#define JSON_VALUE_IMAGE_OPERATION_ROTATE	"rotate"
#define JSON_VALUE_IMAGE_OPERATION_MOVE		"move"
#define JSON_VALUE_IMAGE_OPERATION_SKIP		"skip"

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras, std::string& in_image_data);
	virtual ~ImageProcessBase();

	void SetKey1_RequestType(std::string str_req_type);
	void SetKey2_ImageOperation(std::string str_opertation);
	void SetKey3_ImageOperationParas(std::string str_paras);
	void SetKey4_InImageData(std::string& in_image_data) ;

	virtual bool Excute(std::string& out_image_data);

protected:	
	// opencv Mat��base64�Ļ�ת
	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	cv::Mat Base2Mat(std::string &base64_data);
	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	int req_type;
	std::string m_str_req_type;		// �������ͣ���MPR VR CPR��
	std::string m_in_image_data;	// ͼ��ԭʼ���ݣ�base64����
	std::string m_str_opertation;	// ��ͬͼ��������ͣ��磬 zoom rotate move��
	std::string m_str_paras;		// ��ͬͼ��������͵Ĳ���������������в�ͬ��������Ҫ����Ʒ���
};

//////////////////////////////////////////////////////////////////////////
class ImageZoomProcess : public ImageProcessBase
{
public:
	ImageZoomProcess(std::string str_paras, std::string& in_image_data);
	~ImageZoomProcess();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
class ImageRotateProcess : public ImageProcessBase
{
public:
	ImageRotateProcess(std::string str_paras, std::string& in_image_data);
	~ImageRotateProcess();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
// ƽ�ƣ���С����
class ImageMoveProcess1 : public ImageProcessBase
{
public:
	ImageMoveProcess1(std::string str_paras, std::string& in_image_data);
	~ImageMoveProcess1();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����
};

//////////////////////////////////////////////////////////////////////////
// ƽ�ƣ�����ı�ͼ���С
class ImageMoveProcess2 : public ImageProcessBase
{
public:
	ImageMoveProcess2(std::string str_paras, std::string& in_image_data);
	~ImageMoveProcess2();

	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
};

//////////////////////////////////////////////////////////////////////////
// MPR �������͵Ĵ���
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras, std::string& in_image_data);
	~ImageMPRProcess();
	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;
	/// ��������
	std::string wnd_mpr1_;
};

//////////////////////////////////////////////////////////////////////////
// VR �������͵Ĵ���
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess(std::string str_paras, std::string& in_image_data);
	~ImageVRProcess();
	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;
	/// ��������
	std::string wnd_vr_;
};