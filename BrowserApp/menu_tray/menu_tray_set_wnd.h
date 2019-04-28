#ifndef MENU_TRAY_SET_WND_H_
#define MENU_TRAY_SET_WND_H_

#pragma once
using namespace DuiLib;
#include "Utils/WinImplBase.h"

enum ENUM_TREE_NODE_SELECTED{
    TREE_NODE_SCREEN_SET,
    TREE_NODE_SCREEN_FIXED,
    TREE_NODE_SCREEN_ZOOM,
    TREE_NODE_SCREEN_ABOUT,

    TREE_NODE_SELECTED_COUNT
};
class MenuTraySet_Wnd : public WindowImplBase
{
public:
    MenuTraySet_Wnd(void);
    ~MenuTraySet_Wnd(void);

public:
    virtual CDuiString GetSkinFile();
    virtual LPCTSTR GetWindowClassName(void) const;
    virtual void OnFinalMessage( HWND );
    virtual void Notify(TNotifyUI &msg);
    virtual void InitWindow();
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);

    std::wstring GetChromeCoreVersionText();

    // ����Slider��index
    void SetSliderIndex(int index);         
    // ���õ�ǰ���ű���
    void SetZoomButtonValue(double value);

    DUI_DECLARE_MESSAGE_MAP()
    virtual void OnClick(TNotifyUI& msg);

private:
    // ����������ʾ����Ļ�б���list�ؼ�
    void RefreshScreenListUI(bool is_force);

    // ����ÿ������Ļ������ʾ�ı�
    void UpdateScreensShowText(std::wstring cur_click_screen);

    // ��Ļѡ��� д�������ļ�
    void SetSelectedScreenIndex(size_t shot_index);

    // Tabҳǩ���л�
    void TabSwitchSelected(ENUM_TREE_NODE_SELECTED select_index);

    // �õ���Ļ��Ӧ�Ŀؼ�����
    std::wstring GetControlNameText(std::wstring display_name);

    //�õ���Ļ�ڿؼ��ϣ���ʾ���ı�
    std::wstring GetShowText(int index);

    // �ж���Ļ�Ƿ����������ʾ���磬�Ͽ���Դʱ��������ʾ
    bool IsNormalOfDisplay(std::wstring control_name, std::wstring& display_name);
    CListUI* m_pTagsListUI;
    std::map<std::wstring, std::wstring> m_screen_shots;
};

#endif //MENU_TRAY_SET_WND_H_