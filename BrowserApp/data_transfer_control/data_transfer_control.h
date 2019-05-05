
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

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	bool ParseImageOperationData(std::string json_data, std::string& js_data);



private:
	DataTransferController();
	static DataTransferController* instance;
};
