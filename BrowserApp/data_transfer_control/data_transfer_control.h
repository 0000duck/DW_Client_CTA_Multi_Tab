
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

#define USE_RAPID_JSON

#ifdef USE_RAPID_JSON
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#endif

class ImageProcessBase;

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	bool ParseImageOperationData(char* json_data, std::string& js_data);

	// ֱ�Ӷ�ȡdcm�ļ�����web������
	bool ParseDcmOperationData(char* json_data, std::string& js_data);

#ifdef USE_RAPID_JSON
	bool ParseImageOperationDataUseRapidJson(char* json_data, std::string& js_data);
	std::string GetJsonDataString(Document& doc , std::string key);
#endif


private:
	DataTransferController();
	static DataTransferController* instance;
	
	ImageProcessBase* image_process;
};
