#pragma once

//#include "StdAfx.h"
#include <stdarg.h>
#include <stdint.h>
#include <string> 
#include "Path.h"

#include <glog/logging.h>
#include <glog/log_severity.h> 

#include "encrypt/Utf8String.h"
using namespace std;

class CGLogger
{
public:
	CGLogger(void)
	{
	}

	~CGLogger(void)
	{
		//  here, cannot free the object.
		/*if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}*/
	}

	static void Info(string msg)
	{
		LOG(INFO) << msg;
	}

	static void Error(string msg, string ex = "")
	{
		if (ex.empty()){
			LOG(ERROR) << msg;
		}
		else{
			LOG(ERROR) << msg << " \\r\\nException:" + ex;
		}
	}

	static void Debug(string msg)
	{
#ifdef _DEBUG		
		LOG(WARNING) << msg;	
#else
		wstring logLevel = Configurations::getInstance()->GetLogLevel();		
		if (logLevel == _T("DEBUG")) {
			LOG(WARNING) << msg;
		}
#endif // _DEBUG
	}
	
	static void InitGLog(string procName)
	{
		google::InitGoogleLogging(procName.c_str());

		SYSTEMTIME st;
		::GetLocalTime(&st);
		wchar_t chYearMonth[1000] = { 0 };
		swprintf_s(chYearMonth, _T("%04d%02d"), st.wYear, st.wMonth);
		wchar_t chDay[1000] = { 0 };
		swprintf_s(chDay, _T("%02d"), st.wDay);

#ifdef DEBUG_MODE
		google::SetStderrLogging(google::GLOG_INFO);								//���ü������ google::INFO ����־ͬʱ�������Ļ
#else
		google::SetStderrLogging(google::GLOG_FATAL);								//���ü������ google::FATAL ����־ͬʱ�������Ļ
#endif
		// get log directory according to current date
		wstring logDir = _T("log\\") + wstring(chYearMonth) + _T("\\") + wstring(chDay) + _T("\\");
		// create log directory
		wstring logPath = Configurations::getInstance()->GetUserDataDir() + logDir;
		if (!PathFileExists(logPath.c_str())) {
			CreateMultipleDirectory(logPath.c_str());
		}

		FLAGS_colorlogtostderr = true;												//�����������Ļ����־��ʾ��Ӧ��ɫ
		google::SetLogDestination(google::GLOG_INFO, WStringToString(logPath).c_str());				//���� google::INFO �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_WARNING, WStringToString(logPath).c_str());				//���� google::WARNING �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_ERROR, WStringToString(logPath).c_str());				//���� google::ERROR �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_FATAL, WStringToString(logPath).c_str());				//���� google::ERROR �������־�洢·�����ļ���ǰ׺
		FLAGS_logbufsecs =0;														//������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
		FLAGS_max_log_size =1;														//�����־��СΪ 1MB
		FLAGS_stop_logging_if_full_disk = true;										//�����̱�д��ʱ��ֹͣ��־���
		google::SetLogFilenameExtension(".log");									//�����ļ�����չ����ƽ̨����������Ҫ���ֵ���Ϣ
//  		google::InstallFailureSignalHandler();										//��׽ ���� dumped
//  		google::InstallFailureWriter(&SignalHandle);								//Ĭ�ϲ�׽ SIGSEGV �ź���Ϣ���������� stderr������ͨ������ķ����Զ������>��ʽ��
	}

private:
	//����Ϣ�����LOG(FATAL)
	static void SignalHandle(const char* data, int size)
	{
		std::string str = std::string(data,size);
		/*
		std::ofstream fs("glog_dump.log",std::ios::app);
		fs<<str;
		fs.close();
		*/

		//Ҳ����ֱ�������﷢���ʼ������֪ͨ��������������Ǳ��ص���εģ�ÿ�λص�ֻ���һ�д�����Ϣ������������ļ�¼���ļ���Ҳ��Ҫ>��׷��ģʽ���ɣ���
		//�������﷢�ʼ�����Ų��Ǻ��ʺϣ��������ǿ��Ե���һ�� SHELL �� PYTHON �ű������˽ű����� sleep 3�����ң�Ȼ�󽫴�
		//����Ϣͨ���ʼ�����ŷ��ͳ�ȥ�������Ͳ���Ҫ��ؽű���ʱ��Ƶ��ִ�У��˷�Ч���ˡ�
	}

};