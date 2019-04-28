#include "StdAfx.h"

#include <map>;
#include "BrowserWndManager.h"
#include "WndFrameBase.h"
#include "WndSimpleFrame.h"
#include "WndSubFrame.h"
#include "print_preview.h"
#include "base/Cef3/browser/cef_main_handler.h"
#include "menu_tray/menu_tray_set_wnd.h"
#include "menu_tray/menu_tray_wnd.h"
#include "screen/screen_info_controller.h"

const char kOcrUrl[] = "ocrurl"; //OCR��������URL 
const char kRisUrl[] = "risurl"; //�ƶ�Ris��������URL

TrayWindowManager::TrayWindowManager() 
    : browser_wnd(NULL)
    , tray_icon(NULL)
    , m_menu_tray_set_wnd(NULL) {
}

TrayWindowManager* TrayWindowManager::m_pInstance = 0;
TrayWindowManager* TrayWindowManager::getInstance()
{
	if(m_pInstance == 0)
	{
		m_pInstance = new TrayWindowManager;
		m_pInstance->Create(NULL, L"TrayWnd"
			, WS_MINIMIZE
			, NULL
			, 0, 0, 0, 0);

		m_pInstance->tray_icon = new CTrayIcon;
		m_pInstance->tray_icon->CreateTrayIcon(m_pInstance->GetHWND(), 101/*IDI_ICON1*/, _T("Dr. wise"), UIMSG_TRAYICON);
		m_pInstance->tray_icon->SetShowIcon();
	}
	return m_pInstance;
}

TrayWindowManager::~TrayWindowManager()
{
	browser_wnd = NULL;
    m_menu_tray_set_wnd = NULL;
	delete tray_icon;
	tray_icon = NULL;

	if (m_pCefMainHandler){
		m_pCefMainHandler->Quit();
	}
}

void TrayWindowManager::CreateRootWindow(const wstring& url)
{
	CGLogger::Info("Main window is created");

	// ��Ҫ����WS_CLIPCHILDREN��������
	if (NULL == browser_wnd) {
		browser_wnd = CreateMainWnd<CWndSimpleFrame>(_T("SimpleWnd"), this, url, NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	}
	
	m_mapBrowserWindows.insert(pair<string, CWndFrameBase*>(KEY_MAINWINDOW, browser_wnd));
	// ���������ļ��Ķ�������Ĭ�ϴ����С
	bool isMaximize = Configurations::getInstance()->IsMainWndMaximize();
	if (!isMaximize){
		// ����Ĭ�ϴ����С
		Size mainWndSize = Configurations::getInstance()->GetMainWndSize();
		browser_wnd->ResizeClient(mainWndSize.Width, mainWndSize.Height);
	}
	// ���ô�����С�ߴ�
	Size mainWndMinSize = Configurations::getInstance()->GetMainWndMinSize();
	browser_wnd->SetMinInfo(mainWndMinSize.Width, mainWndMinSize.Height);

	browser_wnd->CenterWindow();
	browser_wnd->ShowWindow();

	// ͨ�������е�����������ڣ���Ҫ�����ڳ�ʼ�����û����õ���Ļ
	RECT rect = ScreenInfoController::GetInstance()->GetSelectedScreenRect();
	::MoveWindow(browser_wnd->GetHWND(), rect.left, rect.top, rect.right, rect.bottom, TRUE);

	if (isMaximize){
		// ���ô�����󻯣����±�������ť״̬
		browser_wnd->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
}

void TrayWindowManager::CreatePrintWindow(const wstring& url)
{
	if (url.empty()){
		CGLogger::Error("Print window's url is empty");
		return;
	}
	// main window will be seen as parent window
	if (m_mapBrowserWindows.count(KEY_MAINWINDOW) == 0){
		CGLogger::Error("Print window cannot be opened cause main window is not existed.");
		return;
	}
	CGLogger::Info("Print window is opened.");

	// ��ȡ������
	CWndFrameBase* mainWnd = m_mapBrowserWindows[KEY_MAINWINDOW];
	mainWnd->OpenPrintWindow(url);
}

void TrayWindowManager::CreateRootWindowAsPopup(const wstring& url)
{
	if (url.empty()){
		CGLogger::Error("New window's url is empty");
		return;
	}
	if (m_mapBrowserWindows.count(KEY_MAINWINDOW) == 0){
		CGLogger::Error("New window cannot be opened cause main window is not existed.");
		return;
	}

	CGLogger::Info("New window is opened with url: " + WStringToString(url));

	// ��ȡ������
	CWndFrameBase* mainWnd = m_mapBrowserWindows[KEY_MAINWINDOW];
	// ����/�����Ӵ���
	if (m_mapBrowserWindows.count(KEY_CHILDWINDOW) == 0){
		// ����
		CWndFrameBase* childWnd = CreateMainWnd<CWndSubFrame>(_T("SubWnd"), this, url, NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		m_mapBrowserWindows.insert(pair<string, CWndFrameBase*>(KEY_CHILDWINDOW, childWnd));

		// ����Ĭ�ϴ����С
		Size newWndSize = Configurations::getInstance()->GetNewWndSize();
		childWnd->ResizeClient(newWndSize.Width, newWndSize.Height);

		// ���ô�����С�ߴ�
		Size newWndMinSize = Configurations::getInstance()->GetNewWndMinSize();
		childWnd->SetMinInfo(newWndMinSize.Width, newWndMinSize.Height);
	}
	
	CWndFrameBase* childWnd = m_mapBrowserWindows[KEY_CHILDWINDOW];	
	// ��ʾ
	childWnd->CenterWindow();
	childWnd->ShowWindow();
	childWnd->LoadUrl(url);
}


CefRefPtr<CefRequestContext> TrayWindowManager::GetRequestContext(CWndFrameBase* root_window) 
{
	return NULL;
}

void TrayWindowManager::OnExit(CWndFrameBase* root_window) 
{
	//close all windows...
}

void TrayWindowManager::OnRootWindowDestroyed(CWndFrameBase* root_window) 
{
	string windowId = root_window->GetBrowserWindowID();
	auto it = m_mapBrowserWindows.find(windowId);
	if (it != m_mapBrowserWindows.end())
		m_mapBrowserWindows.erase(root_window->GetBrowserWindowID());

	// ����������壬������ر��Ӵ���
	if (windowId == KEY_MAINWINDOW){
		it = m_mapBrowserWindows.begin();
		for (; it != m_mapBrowserWindows.end(); ++it){
			it->second->Close(IDOK);
		}
		m_mapBrowserWindows.clear();

		CGLogger::Info("Main window was closed.");

	}else{

		CGLogger::Info("New window was closed.");
	}

	if (browser_wnd == root_window) {
		browser_wnd = NULL;
	}
}

HWND TrayWindowManager::FindWndByBrowser(CefRefPtr<CefBrowser> browser)
{
	HWND hwnd = browser->GetHost()->GetWindowHandle();
	HWND hParentWnd = GetParent(hwnd);
	return hParentWnd;
}

LPCTSTR TrayWindowManager::GetWindowClassName() const
{
	return L"{C1361D01-F544-4884-99AA-32EE1E2E8887}";
}

LRESULT TrayWindowManager::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case UIMSG_TRAYICON:
		{
			switch(lParam)
			{
			case WM_RBUTTONUP:                                // �Ҽ�����ʱ�����˵�
				{
					// �õ����λ��
					LPPOINT lpoint = new tagPOINT;
					::GetCursorPos(lpoint);    

					MenuTray_Wnd* pMenu = new MenuTray_Wnd;
					pMenu->ShowCustomMenu(lpoint->x, lpoint->y);

					delete lpoint;
				}
				break;
			case WM_LBUTTONDBLCLK:                            // ˫������Ĵ���
				{
					if (browser_wnd) {
						browser_wnd->ShowWindow(SW_SHOWNORMAL);          // ��ʾ������
					}
				}
				break;
			}
		}
		break;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
			if (pCopyDataStruct != NULL) {
				LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);  
				DWORD dwLength = (DWORD)(pCopyDataStruct->cbData);  
				WCHAR szRecvData[2048] = {0};  
				memcpy(szRecvData, pszText, dwLength);
				LoadUrlByCommandLine(szRecvData);
			}
		}
		break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) 
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void TrayWindowManager::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void TrayWindowManager::DestroyedTrayManager()
{
	if (browser_wnd) {
		browser_wnd->Close();
	}
	tray_icon->DeleteTrayIcon(); //��������ɾ��ͼ��
	Close();
}

void TrayWindowManager::LoadUrlByCommandLine(LPCWSTR command_line_str)
{
	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(command_line_str);
	std::wstring url;
	if (command_line->HasSwitch(kOcrUrl)) {
		url = command_line->GetSwitchValue(kOcrUrl);

		CGLogger::Info("receive request from OCR. url=" + WStringToString(url));

	}else if (command_line->HasSwitch(kRisUrl)) {
		url = command_line->GetSwitchValue(kRisUrl);
		// ��ƽ̨����Ĳ�������Э�����ƣ���Ҫȥ�� [12/27/2018 Modified by zhangjian]
		wstring protocol_prefix = Configurations::getInstance()->GetLocalAppProtocol();
		if (url.find(protocol_prefix) == 0){
			int len = protocol_prefix.length();
			url = url.substr(len, url.length() - len);
		}

		CGLogger::Info("receive request from cloud. url=" + WStringToString(url));
	}

	if (!url.empty()) {

		int findIndex = url.find(L"://");
		if (findIndex < 0){
			// ����Ĭ��httpЭ�飬������ӱ����ļ���Ҫ��file:///��ͷ [1/2/2019 Modified by zhangjian]
			url = L"http://" + url;
		}

		if (NULL == browser_wnd) {
			CreateRootWindow(url);
		} else {
			if( ::IsIconic(browser_wnd->GetHWND()) ){
				// ����С���Ĵ���ָ�
				::SendMessage(browser_wnd->GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
			}
			if (IsZoomed(browser_wnd->GetHWND()))
			{
				// ��ǰ�������
				::ShowWindow(browser_wnd->GetHWND(), SW_MAXIMIZE);
			} else {
				// ������ʾ����
				::ShowWindow(browser_wnd->GetHWND(), SW_RESTORE);
			}

			// ������ʾ����
			//::ShowWindow(browser_wnd->GetHWND(), SW_RESTORE);
			::SetForegroundWindow(browser_wnd->GetHWND());
			browser_wnd->LoadUrl(url);
		}
	}			
}

void TrayWindowManager::CreateMenuTraySetWnd()
{
    CGLogger::Info("MenuTraySet window is created.");
    
    if (m_menu_tray_set_wnd) {
        SetWindowPos(m_menu_tray_set_wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);         
    } else {
        m_menu_tray_set_wnd = new MenuTraySet_Wnd();
        m_menu_tray_set_wnd->Create(NULL, _T("���ҽ��-����"), WS_POPUP | WS_VISIBLE, 0, 0, 800, 572);
        m_menu_tray_set_wnd->CenterWindow();
    }
}

void TrayWindowManager::DestroyedTraySetWnd()
{
    m_menu_tray_set_wnd = NULL;
}

void TrayWindowManager::NotifyZoomIndexChanged(int zoom_index)
{
    // ���������ļ�����������
    double value = SetConfigZoomIndex(zoom_index);    
    // ������������ű���
    SetWndFrameZoomLevel(value);
    // ����Slider�ؼ�������ֵ
    SetSliderIndex(zoom_index);
    // ����Button������ֵ
    SetZoomButtonValue(value);
}

double TrayWindowManager::SetConfigZoomIndex(int zoom_index)
{
    double value = Configurations::getInstance()->SetBrowserZoomLevelIndex(zoom_index);  
    return value;
}

void TrayWindowManager::SetWndFrameZoomLevel(double level)
{
    if (browser_wnd) {
        browser_wnd->SetBrowserZoomLevel(level);
    }
}

void TrayWindowManager::SetSliderIndex(int level_index)
{
    if (m_menu_tray_set_wnd) {
        m_menu_tray_set_wnd->SetSliderIndex(level_index);
    }
}

void TrayWindowManager::SetZoomButtonValue(double value)
{
    if (m_menu_tray_set_wnd) {
        m_menu_tray_set_wnd->SetZoomButtonValue(value);
    }
}