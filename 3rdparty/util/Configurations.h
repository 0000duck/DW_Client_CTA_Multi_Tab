
#ifndef C_PARSE_INI_FILE_H_
#define C_PARSE_INI_FILE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "StringUtil.h"

using namespace std;
#define COMMENT_CHAR '#'

class Configurations
{
public:
	static Configurations* getInstance();
	void Initialize();
	//�����Ŀ¼
	wstring GetRootDir();
	//��ԴĿ¼����ž�̬��Դ
	wstring GetResourceDir();
	//Ƥ��Ŀ¼�����Ƥ����ص�ͼ�����Դ
	wstring GetSkinDir();
	wstring GetStartupUrl();
	//��־�������
	wstring GetLogLevel() {return m_sLogLevel;}
	//Ĭ�����ضԻ����λ��
	wstring GetDefaultDownloadDir();
	//�Զ���Э������
	wstring GetLocalAppProtocol();
	//�������ɵ���ʱ�ļ�
	wstring GetTempDir();
	//�û�Ŀ¼����������в������������ú���ʱ�ļ�
	wstring GetUserDataDir();
	//��ӡ������ò���
	int GetMarginType();
	void GetMargin(int& left, int& right, int& top, int& bottom);
	bool GetHeaderEnabled();
	//end
	//renderer������־�������
	bool IsRendererLog();

	bool IsMainWndMaximize();

	Size GetMainWndSize();
	Size GetMainWndMinSize();
	Size GetNewWndSize();
	Size GetNewWndMinSize();

	void SetStartupUrl(const wstring& url);
	void SetMainWndSize(Size size);
	//��ӡ������ò���
	void SetMarginType(int margin_type);
	void SetMargin(int left, int right, int top, int bottom);
	void SetHeaderEnabled(bool enabled);
	//end
	//��������
	void SetProcessType(int type);
	int GetProcessType();
	//������ɫ
	wstring GetBackgroundColor();
	//����webѹ������Դ�ļ�
	const map<wstring, wstring>& GetArchiveFiles();
    // ���Ľ���������ķŴ���
    double GetBrowserZoomLevelValue();
    int    GetBrowserZoomLevelIndex();
    double SetBrowserZoomLevelIndex(int index);
private:
	Configurations();
	static Configurations* m_pInstance;
	void SetIniValue(const wstring& ini_file, const wstring& section, const wstring& key, const wstring& val);
	void FormatStringParam(const wstring& paramStr, Size& defaultWndSize, Size& miniWndSize);
	void FormatStringParam(const wstring& paramStr, map<wstring, wstring>& files);
	
private:
	wstring m_sRootDir;
	wstring m_sStartupUrl;
	wstring m_sLogLevel;
	Size m_zMainWndSize;
	Size m_zMainWndMinSize;
	Size m_zNewWndSize;
	Size m_zNewWndMinSize;
	int m_bIsMainWndMaximize;
	int m_bIsPromptAlert;
	wstring user_data_dir_;
	//�������ӡ���
	int margin_type_;
	int margin_left_;
	int margin_right_;
	int margin_top_;
	int margin_bottom_;
	bool header_footer_;
	//�Ƿ���renderer���̵���־
	bool renderer_log_enabled_;
	//��ǰ���̵�CEF����
	int current_process_type_;
	//�����������CSS��ɫ��ʽ����#FFFFFF
	wstring background_color_;
	//����ѹ������Դ�ļ���key��Ϊurl��ַ��valueΪ�����ļ�����
	map<wstring, wstring> archive_files_;
    // ���Ľ���������ķŴ���������
    int browser_zoom_level_index_;
};

#endif
