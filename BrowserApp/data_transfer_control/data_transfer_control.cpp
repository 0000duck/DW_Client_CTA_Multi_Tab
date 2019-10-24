#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

#include <algorithm>

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>
#include <chrono>

#include "util/md5.h"

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
	image_process = NULL;
}


DataTransferController::~DataTransferController()
{
	instance = nullptr;
}

bool DataTransferController::ParseImageOperationData(char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����
	Json::Reader reader;
	Json::Value root;
	bool ret = reader.parse(json_data, root, false);
 	if (!ret) {
		return false;
	}

	// ��ùؼ��ԵĲ���
	std::string key_name1("");
	std::string key_name2("");
	std::string key_name3("");
	
	if (root[JSON_KEY_REQUEST_TYPE].isString()) {
		key_name1 = root[JSON_KEY_REQUEST_TYPE].asString();
		// ��תΪСд�ַ�
		std::transform(key_name1.begin(), key_name1.end(), key_name1.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_OPERATION].isString()) {
		key_name2 = root[JSON_KEY_IMAGE_OPERATION].asString();
		// ��תΪСд�ַ�
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_PARAS].isString()) {
		key_name3 = root[JSON_KEY_IMAGE_PARAS].asString();
		// ��תΪСд�ַ�
		std::transform(key_name3.begin(), key_name3.end(), key_name3.begin(), ::tolower);
	}	

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		image_process = new ImageVRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {	
		image_process = new ImageCPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {		
		return true;
	}

	bool is_use_json = false; // ���ؽ��ͼ���ļ�ʱ���Ƿ�ʹ��json��ʽ
	if (is_use_json) {
		// ģ���ٷ��͸������
		Json::FastWriter writer;
		// ֻ�޸Ŀͻ��˹��ĵ�key
		//Json::Value inputjson;
		//root[JSON_KEY_REQUEST_TYPE]	= key_name1;
		//root[JSON_KEY_IMAGE_OPERATION] = key_name2;
		//root[JSON_KEY_IMAGE_PARAS]		= key_name3;
		root[JSON_KEY_IMAGE_DATA]		= out_image_data;
		std::string jsonstr = writer.write(root);
		// �л��з���json�ַ����� JS���ܴ���
		if (*jsonstr.rbegin() == '\n') {
			jsonstr.erase(jsonstr.end() - 1);
		}
		// ʹ��XML Request �Ļص����ƣ�����json�ַ���
		js_data = jsonstr;
	} else {
		// ʹ��XML Request �Ļص����ƣ�ֱ�Ӵ���ͼ���base64
		js_data = out_image_data;
	}

	return true;
}

// ��web������ʱ��ֱ�Ӵ���dcm�ļ���
bool DataTransferController::ParseDcmOperationData(char* json_data, std::string& js_data)
{
	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	image_process = new ImageDcmProcess("dcm");
	image_process->Excute(js_data);

	return true;
}

// ��web������ʱ��web�����ļ������ͻ���д�뱣��
bool DataTransferController::ParseWriteFileOperationData(
	char* json_data, unsigned int length, 
    std::vector<std::string>& vec_url_elements, std::string url, 
	std::string& js_data)
{
	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	image_process = new ImageWriteProcess("write");
	//image_process->SaveFile(json_data);
	std::string path = "C:\\ztest2\\save_file\\";
	if (vec_url_elements.size() < 4) {
		return false;
	}

	int len = std::string("http://image_controller/write_file/").length();
	std::string file_name = url.substr(len);
	printf("write---url payload : %s\n", file_name.c_str());
	MD5 md5(file_name);
	std::string file_name_md5 = md5.toString();
	printf("write---file_name_md5 : %s\n", file_name_md5.c_str());

#if _DEBUG
	std::string file_key_value = path + "file_key_log.csv";
	{
		FILE* fp = fopen(file_key_value.c_str(), "w+");
		if (fp) {
			fprintf(fp, "%s,%s\n", file_name_md5.c_str(), file_name.c_str());
			fclose(fp);
		}

	}
#endif

	//std::string decode_str = md5.
	//path += vec_url_elements[3];
	path += file_name_md5;

	FILE* fp = fopen(path.c_str(), "wb");
	if (fp) {
		//fprintf(fp, "%s", json_data);
		fwrite(json_data, 1, length, fp);
		fclose(fp);
	}

	image_process->Excute(js_data);

	return true;
}
// ��web������ʱ��web���󣬿ͻ��˶�ȡ���棬����web
bool DataTransferController::ParseReadFileOperationData(
	char* json_data, std::vector<std::string>& vec_url_elements, 
	std::string url, std::string& js_data)
{
	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	image_process = new ImageReadDcmProcess("read");
	if (vec_url_elements.size() < 4) {
		return false;
	}

	// web�����read_file�����������ʱ���,��Ҫ���е�
	//std::vector<std::string> v = SplitString(vec_url_elements[3], "&");
	//if (v.size() == 0) {
	//	return false;
	//}
	//image_process->SetFileName(v[0]);

	int len = std::string("http://image_controller/read_file/").length();
	std::string file_name = url.substr(len);
	printf("read---url payload : %s\n", file_name.c_str());

	MD5 md5(file_name);
	std::string file_name_md5 = md5.toString();
	printf("read---file_name_md5 : %s\n", file_name_md5.c_str());

	image_process->SetFileName(file_name_md5);

	image_process->Excute(js_data);

	return true;
}

// ��web������ʱ��web���󣬿ͻ�������ڴ�
bool DataTransferController::ParseClearFilesOperationData(char* json_data, std::vector<std::string>& vec_url_elements, std::string& js_data)
{
	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	image_process = new ImageClearFileDcmProcess("clear_files");
	image_process->Excute(js_data);

	return true;
}

#ifdef USE_RAPID_JSON
std::string DataTransferController::GetJsonDataString(Document& doc, std::string key)
{
	std::string str_value = "";
	if (doc.HasMember(key.data())) {
		const Value& value = doc[key.c_str()];
		str_value = value.GetString();
		return str_value;
	}	
	return str_value;
}

bool DataTransferController::ParseImageOperationDataUseRapidJson(char* json_data, std::string& js_data)
{
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	static std::chrono::duration<double> total_diff = std::chrono::duration<double>(0);;
	total_diff += span;

	// ��ùؼ��ԵĲ���
	std::string key_name1("");
	std::string key_name2("");
	std::string key_name3("");
	std::string key_name4("");
	//std::string key_name5("");
	//std::string key_name6("");
	int key_name5 = 0;
	int key_name6 = 0;

	if (doc.HasMember(JSON_KEY_REQUEST_TYPE)) {
	//	const Value& value = doc[JSON_KEY_REQUEST_TYPE];
	//	key_name1 = value.GetString();
	key_name1 = GetJsonDataString(doc, JSON_KEY_REQUEST_TYPE);


		// ��תΪСд�ַ�
		std::transform(key_name1.begin(), key_name1.end(), key_name1.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_OPERATION)) {
		const Value& value = doc[JSON_KEY_IMAGE_OPERATION];
		key_name2 = value.GetString();
		// ��תΪСд�ַ�
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}

	if (doc.HasMember(JSON_KEY_IMAGE_PARAS)) {
		const Value& value = doc[JSON_KEY_IMAGE_PARAS];
		key_name3 = value.GetString();
		// ��תΪСд�ַ�
		std::transform(key_name3.begin(), key_name3.end(), key_name3.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_DATA)) {
		const Value& value = doc[JSON_KEY_IMAGE_DATA];
		key_name4 = value.GetString();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_SEQUENCE)) {
		const Value& value = doc[JSON_KEY_IMAGE_SEQUENCE];
		key_name5 = value.GetInt();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_MAX)) {
		const Value& value = doc[JSON_KEY_IMAGE_MAX];
		key_name6 = value.GetInt();
	}

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		CGLogger::Debug("Debug_CTA:MPR---begin");
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		CGLogger::Debug("Debug_CTA:MPR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		CGLogger::Debug("Debug_CTA:VR---begin");
		image_process = new ImageVRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		CGLogger::Debug("Debug_CTA:VR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {		
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {
		if (0 == key_name5) {
			total_diff = std::chrono::duration<double>(0);
			CGLogger::Debug("Debug_CTA:TIME_TEST_TOTAL ------------------begin");
		}
		if (key_name5 == key_name6 - 1) {
			CGLogger::Debug("Debug_CTA:TIME_TEST_TOTAL -------------------end");
			std::stringstream ss; 
			ss << "Debug_CTA:TIME_TEST_TOTAL ----------------------------diff :";
			ss << total_diff.count();
			CGLogger::Debug(ss.str().c_str());
		}

		CGLogger::Debug("Debug_CTA:TIME_TEST---begin");
		js_data  = "";
		CGLogger::Debug("Debug_CTA:TIME_TEST---");
		return true;
	} else {		
	}

	// ģ���ٷ��͸������
	Json::FastWriter writer;
	Json::Value inputjson;
	inputjson[JSON_KEY_REQUEST_TYPE]	= key_name1;
	inputjson[JSON_KEY_IMAGE_OPERATION] = key_name2;
	inputjson[JSON_KEY_IMAGE_PARAS]		= key_name3;
	inputjson[JSON_KEY_IMAGE_DATA]		= out_image_data;
	std::string jsonstr = writer.write(inputjson);
	// �л��з���json�ַ����� JS���ܴ���
	if (*jsonstr.rbegin() == '\n') {
		jsonstr.erase(jsonstr.end() - 1);
	}
	
	// ʹ��XML Request �Ļص����ƣ�����json�ַ���
	js_data = jsonstr;

	return true;
}
#endif