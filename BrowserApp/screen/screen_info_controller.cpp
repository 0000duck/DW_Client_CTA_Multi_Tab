#include "stdafx.h"
#include "screen_info_controller.h"

ScreenInfoController* ScreenInfoController::screen_info_instance = NULL;

const LPCWSTR DEFALT_MONITOR = L"\\\\.\\DISPLAY1";

ScreenInfoController* ScreenInfoController::GetInstance()
{
	if (NULL == screen_info_instance) {
		screen_info_instance = new ScreenInfoController;
	}

	return screen_info_instance;
}

std::map<std::wstring, std::wstring> ScreenInfoController::GetScreenShot()
{
	InitDisplayMonitors();
	std::map<std::wstring, std::wstring> device_screeshot_map;
	std::wstring sreenshot_path =  config_file_path + L"ScreenShot/"; //��Ļ��ͼ����Ŀ¼
	if (!PathFileExists(sreenshot_path.c_str())) {
		::CreateDirectory(sreenshot_path.c_str(), NULL);
	}

	for (auto iter = monitor_info_map.begin(); iter != monitor_info_map.end(); ++iter) {
		std::wstring screenshot_file = std::wstring(sreenshot_path + iter->second.szDevice + L".bmp");
		SaveBitmapToFile(GetCaptureBmp(iter->second.rcVirtual), screenshot_file.c_str());
		device_screeshot_map.insert(std::pair<std::wstring, std::wstring>(iter->second.szDevice, screenshot_file));
	}

	return device_screeshot_map;
}

std::wstring ScreenInfoController::DetectionDeafaultScreenName()
{
	//�������ļ���ȡ�ϴα������ʾ������
	WCHAR monitor_name[MAX_PATH] = {0};
	::GetPrivateProfileString(L"DeepWiseCore", L"Monitor", L"", monitor_name, MAX_PATH, config_file_name.c_str());
	device_name = monitor_name;
	if (device_name.empty() || monitor_info_map.find(device_name) == monitor_info_map.end()) {
		if (!monitor_info_map.empty()) {
			device_name = monitor_info_map.begin()->first;
		} else {
			device_name = DEFALT_MONITOR;
		}
	}

	return device_name;
}

ScreenInfoController::ScreenInfoController()
{
	Initialize();
}

RECT ScreenInfoController::GetSelectedScreenRect()
{	
	InitDisplayMonitors();
	auto iter = monitor_info_map.find(device_name);
	if (iter != monitor_info_map.end()) {
		return iter->second.rcVirtual;
	} else {
		device_name = DEFALT_MONITOR;
	}

	iter = monitor_info_map.find(device_name);
	if (iter != monitor_info_map.end()) {
		return iter->second.rcVirtual;
	}

	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;
	return rect;
}

bool ScreenInfoController::SetSelectedScreen(const std::wstring &deviceName)
{
	InitDisplayMonitors();

	//����deviceName����ʾ������
	if (monitor_info_map.find(deviceName) != monitor_info_map.end()) {
		this->device_name = deviceName;
		if (!PathFileExists(config_file_path.c_str())) {
			::CreateDirectory(config_file_path.c_str(), NULL);
		}
		WritePrivateProfileString(L"DeepWiseCore", L"Monitor", deviceName.c_str(), config_file_name.c_str());
		return true;
	} 

	return false;
}

void ScreenInfoController::Initialize()
{
	config_file_path = Configurations::getInstance()->GetUserDataDir();
	config_file_name = config_file_path + L"DeepWiseUser.ini";
	InitDisplayMonitors();
}

void ScreenInfoController::InitDisplayMonitors()
{ 
	monitor_info_map.clear();
	m_iMontorNumber = 0;
	::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
	DetectionDeafaultScreenName();
}

BOOL CALLBACK ScreenInfoController::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor,  LPARAM dwData)
{
	ScreenInfoController *pThis = (ScreenInfoController *)dwData;
	MONITORINFOEX infoEx;
	memset(&infoEx, 0,  sizeof(infoEx));
	infoEx.cbSize = sizeof(infoEx);
	GetMonitorInfo(hMonitor, &infoEx);
	//������ʾ����Ϣ
	VMONITORINFO monitor_info;
	monitor_info.hMonitor = hMonitor;
	if(lprcMonitor)
	{
		monitor_info.rcVirtual = *lprcMonitor;
	}
	monitor_info.rcMonitor = infoEx.rcMonitor;
	monitor_info.rcWork = infoEx.rcWork;
	monitor_info.bPrimary = infoEx.dwFlags == MONITORINFOF_PRIMARY;

	_tcscpy_s(monitor_info.szDevice, infoEx.szDevice);

	pThis->monitor_info_map.insert(std::pair<std::wstring, VMONITORINFO>(std::wstring(infoEx.szDevice), monitor_info));
	return TRUE; 
}

HBITMAP ScreenInfoController::GetCaptureBmp(RECT rect)
{
	HDC       hDC;   
	HDC       MemDC;   
	BYTE*     Data;   
	HBITMAP   hBmp;   
	BITMAPINFO   bi;   
	LONG width  = abs(rect.right - rect.left);
	LONG height = abs(rect.bottom - rect.top);
	memset(&bi, 0, sizeof(bi));   
	bi.bmiHeader.biSize     =  sizeof(BITMAPINFO);
	bi.bmiHeader.biWidth    =  width;    //GetSystemMetrics(SM_CXSCREEN);   
	bi.bmiHeader.biHeight   =  height;   //GetSystemMetrics(SM_CYSCREEN);   
	bi.bmiHeader.biPlanes   =  1;   
	bi.bmiHeader.biBitCount =  24;    

	hDC   = GetDC(NULL);   
	MemDC = CreateCompatibleDC(hDC);   
	hBmp  = CreateDIBSection(MemDC,   &bi, DIB_RGB_COLORS,   (void**)&Data,   NULL,   0);   
	SelectObject(MemDC,   hBmp);   
	BitBlt(MemDC,   0,   0,   width,   height, hDC,   rect.left,   rect.top,   SRCCOPY);   
	ReleaseDC(NULL,   hDC);     
	DeleteDC(MemDC);   
	return   hBmp;
}

bool ScreenInfoController::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
	HDC                hDC;                //�豸������
	int                iBits;              //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD               wBitCount = 0;      //λͼ��ÿ��������ռ�ֽ���    
	DWORD              dwPaletteSize=0;    //�����ɫ���С
	DWORD              dwBmBitsSize;       //λͼ�������ֽڴ�С
	DWORD              dwDIBSize;          //λͼ�ļ���С
	DWORD              dwWritten;          //д���ļ��ֽ���
	BITMAP             Bitmap;             //λͼ�ṹ
	BITMAPFILEHEADER   bmfHdr;             //λͼ���Խṹ   
	BITMAPINFOHEADER   bi;                 //λͼ�ļ�ͷ�ṹ
	LPBITMAPINFOHEADER lpbi;               //λͼ��Ϣͷ�ṹ     ָ��λͼ��Ϣͷ�ṹ
	HANDLE             fh;                 //�����ļ����
	HANDLE             hDib;               //�����ڴ���
	HANDLE             hPal;               //�����ڴ���
	HANDLE             hOldPal=NULL;       //��ɫ����  

	//����λͼ�ļ�ÿ��������ռ�ֽ���   
	hDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);   
	DeleteDC(hDC);

	if (iBits <= 1)       
		wBitCount = 1;   
	else if (iBits <= 4)       
		wBitCount = 4;   
	else if (iBits <= 8)       
		wBitCount = 8;   
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits<=32)
		wBitCount = 24;


	//�����ɫ���С   
	if (wBitCount <= 8) {       
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);
	}

	//����λͼ��Ϣͷ�ṹ   
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);   
	bi.biSize            = sizeof(BITMAPINFOHEADER);   
	bi.biWidth           = Bitmap.bmWidth;   
	bi.biHeight          = Bitmap.bmHeight;   
	bi.biPlanes          = 1;   
	bi.biBitCount         = wBitCount;       
	bi.biCompression      = BI_RGB;   
	bi.biSizeImage        = 0;   
	bi.biXPelsPerMeter     = 0;   
	bi.biYPelsPerMeter     = 0;   
	bi.biClrUsed         = 0;   
	bi.biClrImportant      = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;

	//Ϊλͼ���ݷ����ڴ�   
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (lpbi==NULL)
	{
		return false;
	}

	*lpbi = bi;   
	// �����ɫ��
	hPal = GetStockObject(DEFAULT_PALETTE);   
	if (hPal)
	{       
		hDC  = GetDC(NULL);       
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);       
		RealizePalette(hDC);       
	}   
	// ��ȡ�õ�ɫ�����µ�����ֵ   
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,       
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);   
	//�ָ���ɫ��      
	if (hOldPal)       
	{       
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);       
		RealizePalette(hDC);       
		ReleaseDC(NULL, hDC);       
	}   
	//����λͼ�ļ�       
	fh = CreateFile(lpFileName, GENERIC_WRITE,        
		0, NULL, CREATE_ALWAYS,       
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);   

	if (fh == INVALID_HANDLE_VALUE) {
		return false;
	}

	// ����λͼ�ļ�ͷ   
	bmfHdr.bfType = 0x4D42;  // "BM"   
	dwDIBSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ dwPaletteSize + dwBmBitsSize;     
	bmfHdr.bfSize = dwDIBSize;   
	bmfHdr.bfReserved1 = 0;   
	bmfHdr.bfReserved2 = 0;   
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize;

	// д��λͼ�ļ�ͷ   
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// д��λͼ�ļ���������   
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//���      
	GlobalUnlock(hDib);   
	GlobalFree(hDib);   
	CloseHandle(fh);

	return true;
}
