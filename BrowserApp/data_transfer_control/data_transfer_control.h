
/****************************************************************************************************

 * \file data_transfer_control.h
 * \date 2019/05/05 10:33
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief : c++��js ���ݽ������� 

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++���������ͷ�ļ�
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream> 

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	bool ParseImageOperationData(std::string json_data, std::string& js_data);



private:
	DataTransferController();
	static DataTransferController* instance;

	bool ImageZoom(std::string str_rate, std::string& in_image_data, std::string& out_image_data);
	//��ʱ����תͼ��degree�Ƕȣ�ԭ�ߴ磩
	bool ImageRotate1(std::string str_angel, std::string& in_image_data, std::string& out_image_data);
	//��תͼ�����ݲ��䣬�ߴ���Ӧ���
	bool ImageRotate2(std::string str_angel, std::string& in_image_data, std::string& out_image_data);

	// ƽ�ƣ���С����
	bool ImageMove1(std::string str_move_position, std::string& in_image_data, std::string& out_image_data);
	// ƽ�ƣ�����ı�ͼ���С
	bool ImageMove2(std::string str_move_position, std::string& in_image_data, std::string& out_image_data);


	// opencv Mat��base64�Ļ�ת
	static std::string base64Decode(const char* Data, int DataByte);
	static std::string base64Encode(const unsigned char* Data, int DataByte);
	static std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	static cv::Mat Base2Mat(std::string &base64_data);
};
