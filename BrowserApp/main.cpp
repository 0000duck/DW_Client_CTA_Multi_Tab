#include "stdafx.h"

#include <Windows.h>
#include <tlhelp32.h>    //CreateToolhelp32Snapshot

#include "base/Cef3/browser/cef_main_handler.h"
#include "main/WndSimpleFrame.h"
#include "main/BrowserWndManager.h"
#include "util/MiniDump.h"
#include <process.h>


#include "base/Cef3/browser/ClientSchemeHandler.h"
#include "data_transfer_control/client_xml_request_resource_handler.h"

#define SINGLE_INSTANCE		_T("DR. WISE CLIENT BROWSER")
//#define RENDERER_DEBUG


typedef struct tagWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
} WNDINFO, *LPWNDINFO;

BOOL CheckBowserRunning();
void HandleCommandLine(HWND hwnd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{ 
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	// ��ȡ��������
	ClientApp::ProcessType processType = ClientApp::GetProcessType();
	if (processType == ClientApp::ProcessType::BrowserProcess &&
		CheckBowserRunning()){
		return 0;
	}




	CPaintManagerUI::SetInstance(hInstance);
	// ��ȡ�����ļ�
	Configurations::getInstance()->Initialize();
	Configurations::getInstance()->SetProcessType((int)processType);
	// MiniDump
	InitMiniDump();

	if (processType == ClientApp::BrowserProcess || 
		(processType == ClientApp::RendererProcess &&
		Configurations::getInstance()->IsRendererLog())){
		// renderer���̵���־�����ùر� [1/9/2019 Modified by zhangjian]
		int nPid = (int)_getpid();
		// ------------------��־��ʼ��------------------
		CGLogger::InitGLog("p" + to_string(processType));

		CGLogger::Info("System begins to run. Process Id: " + std::to_string(nPid));
	}
	
#ifdef RENDERER_DEBUG
	if (processType == ClientApp::ProcessType::RendererProcess){
		MessageBox(NULL, _T("To attach renderer process!"), _T("test"), 0);
	}
#endif


#if _DEBUG
	if (processType == ClientApp::ProcessType::BrowserProcess){
		AllocConsole();
		freopen("CONOUT$", "w+t", stdout);
		printf("test console\n");
	}
#endif
	// ------------------CEF��ʼ��------------------
	CCefMainHandler CefMainHandler;
	if (!CefMainHandler.Initialize(hInstance, FALSE))
	{
		return -1;
	}
	
	// ʹ��XMLHttpRequest���н�������1����ע��Scheme
	CefRegisterSchemeHandlerFactory("http", HANDLER_POSTDATA_NAME,		new ClientSchemeHandlerFactory);
	CefRegisterSchemeHandlerFactory("http", HANDLER_IAMGE_CONTROLLER,				new ClientXMLRequestSchemeHandlerFactory);
	CefRegisterSchemeHandlerFactory("http", HANDLER_IAMGE_ARRAY_BUFFER_TRANSFER,	new ClientXMLRequestSchemeHandlerFactory);



	// ------------------duilib��ʼ��------------------
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\"));
	CPaintManagerUI::SetResourcePath(Configurations::getInstance()->GetSkinDir().c_str());
	if (FAILED(::CoInitialize(NULL)))
	{
		return -1;
	}
	
	// ------------------��ʾ������------------------
	TrayWindowManager::getInstance()->SetCefMainHandler(&CefMainHandler);
	HandleCommandLine(TrayWindowManager::getInstance()->GetHWND());

	// ------------------������Ϣѭ��------------------
	CefMainHandler.RunMessageLoop();

	CGLogger::Info("System is shutting down...");

	// ------------------������־����------------------
	google::ShutdownGoogleLogging();


	// ------------------�����ر���Դ------------------
	CefMainHandler.Shutdown();

	::CoUninitialize();

	return 0;
}

// IsBowserRunning  - �Ƿ��Ѿ�����
BOOL CheckBowserRunning()
{
	BOOL bRet = FALSE;
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, SINGLE_INSTANCE);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		bRet = TRUE;
		HWND hwnd = FindWindow(L"{C1361D01-F544-4884-99AA-32EE1E2E8887}", NULL); // �ж����̴���������Ƿ��Ѵ���
		if (hwnd){
			HandleCommandLine(hwnd);
		}
		else {
			MessageBox(NULL, _T("�����Ѿ���������ر�ԭ������ٴ򿪣�"), _T("�����Ѿ�����"), 0);
		}
	}
	if (hMutex)
	{
		::ReleaseMutex(hMutex);
	}
	return bRet;
}

void HandleCommandLine(HWND hwnd) 
{
	if (hwnd && ::IsWindow(hwnd)) {
		COPYDATASTRUCT cdsSend = {0};
		cdsSend.dwData = 0;
		cdsSend.cbData = wcslen(GetCommandLineW()) * sizeof(WCHAR);
		cdsSend.lpData = (void*)GetCommandLineW();
		::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cdsSend);
	} else {
		MessageBox(NULL, _T("�����Ѿ���������ر�ԭ������ٴ򿪣�"), _T("�����Ѿ�����"), 0);
	}
}