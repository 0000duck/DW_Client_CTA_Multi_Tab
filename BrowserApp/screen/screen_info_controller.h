/****************************************************************************************************

����: mamx 2018-12-19
��ע: ʶ����ʾ����Ϣ���������Ĭ����ʾ������
���:

Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once
#include <map>
#include <tchar.h>
#include <windows.h>
class ScreenInfoController
{
public:
	static ScreenInfoController* GetInstance();
	RECT GetSelectedScreenRect();                            //��ȡ�ϴ����õ���ʾ��λ�� 
	bool SetSelectedScreen(const std::wstring &deviceName);  //����ѡ�����ʾ��
	std::map<std::wstring, std::wstring> GetScreenShot();    //��ȡ��ʾ����ͼ�ͽ�ͼ�ļ�Ŀ¼
	std::wstring DetectionDeafaultScreenName();              //���������ǰ��ʾ����ʾ������

protected:
	ScreenInfoController();
	~ScreenInfoController(){}

private:
	void Initialize();

	typedef struct VMONITORINFO
	{
		HMONITOR hMonitor;      //��ʾ�����
		TCHAR    szDevice[32];  //��ʾ����
		RECT     rcVirtual;     //������ʾ������
		RECT     rcMonitor;     //������ʾ����
		RECT     rcWork;        //������ʾ����
		BOOL     bPrimary;      //����ʾ����

		VMONITORINFO() {
			memset(this, 0, sizeof(*this));
		}
	};

	void InitDisplayMonitors();

	//ö����ʾ���ص�����
	static BOOL CALLBACK MonitorEnumProc(
		HMONITOR hMonitor,  // handle to display monitor
		HDC hdcMonitor,     // handle to monitor-appropriate device context
		LPRECT lprcMonitor, // pointer to monitor intersection rectangle
		LPARAM dwData       // data passed from EnumDisplayMonitors
		); 

	HBITMAP GetCaptureBmp(RECT rect); // ��ȡ��Ļ��ͼ
	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

private:
	static ScreenInfoController* screen_info_instance;
	VMONITORINFO m_MonitorInfoExArray[64];  
	std::map<std::wstring, VMONITORINFO> monitor_info_map; //��ʾ����Ϣ�б�
	int m_iMontorNumber;
	std::wstring device_name;      //�ϴα������ʾ������
	std::wstring config_file_name; 
	std::wstring config_file_path;
};