// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "UIlib.h"

using namespace DuiLib;
using namespace Gdiplus;

#define WM_PRINT_WND_CLOSED			WM_USER+1001
#define WM_PRINT_PREVIEW			WM_USER+1002

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "base/control/UICefBrowser.h"
#include "util/Configurations.h"
#include "encrypt/Utf8String.h"
//#include <glog/logging.h>
#include "util/Logger.h"

typedef CCefBrowserUI CCefUI;
#define ENABLE_ENCODE

////OpenCV���Ŀ�ͷ�ļ�
//#include <opencv2\core\core.hpp>
////OpenCVͼ�δ���ͷ�ļ�
//#include <opencv2\highgui\highgui.hpp>

#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_69_3497/libcef.lib")
#else
    #pragma comment(lib, "libcef_49_2623/libcef.lib")
#endif


#ifdef _DEBUG
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper_d.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper_d.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper_d.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper_d.lib")
#endif
    #pragma comment(lib, "Duilib/DuiLib_d.lib")
    #pragma comment(lib, "glog/glog_d.lib")
	////OpenCV���Ķ�̬���ӿ⣬��core.hppͷ�ļ���Ӧ
	//#pragma comment(lib,"Debug/opencv_core320d.lib")
	////OpenCVͼ�δ���̬���ӿ⣬��highgui.hppͷ�ļ���Ӧ
	//#pragma comment(lib,"Debug/opencv_highgui320d.lib")
#else	
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper.lib")
#endif
    #pragma comment(lib, "Duilib/DuiLib.lib")
    #pragma comment(lib, "glog/glog.lib")
	////OpenCV���Ķ�̬���ӿ⣬��core.hppͷ�ļ���Ӧ
	//#pragma comment(lib,"Release/opencv_core320.lib")
	////OpenCVͼ�δ���̬���ӿ⣬��highgui.hppͷ�ļ���Ӧ
	//#pragma comment(lib,"Release/opencv_highgui320.lib")
#endif

//#include <vld.h>