#include "stdafx.h"

#include "image_process.h"

#include <algorithm>
#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>

#include "api/studycontextmy.h"
#include "curve_source.h"
#include "data/data_definition.h"
#include "data/data_loader.h"
#include "data_source.h"
#include "io/dcm_reader.h"
#include "render_source.h"
#include "render_facade.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::IO;

// only once
static DW::IO::IDicomReader* reader = NULL;


ImageProcessBase::ImageProcessBase(std::string str_paras, std::string& in_image_data)
	: m_in_image_data(in_image_data)
	, m_str_paras(str_paras)
	, req_type(0)
{
}

ImageProcessBase::~ImageProcessBase()
{
	
}

void ImageProcessBase::SetRequestType(std::string str_req_type)
{
	if (str_req_type == JSON_VALUE_REQUEST_TYPE_MPR) {
		req_type = (int)RenderControlType::MPR;
	} else if (str_req_type == JSON_VALUE_REQUEST_TYPE_VR) {
		req_type = (int)RenderControlType::VR;
	} else if (str_req_type == JSON_VALUE_REQUEST_TYPE_CPR) {
		req_type = (int)RenderControlType::STRAIGHTENED_CPR;
	}
}

void ImageProcessBase::SetImageOperationParas(std::string str_paras) 
{ 
	m_str_paras = str_paras; 
}
void ImageProcessBase::SetInImageData(std::string& in_image_data) 
{
	m_in_image_data = in_image_data;
}

bool ImageProcessBase::Excute(std::string& out_image_data)
{
	return true;
}
std::string ImageProcessBase::base64Decode(const char* Data, int DataByte) {
	//�����
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte) {
		if (*Data != '\r' && *Data != '\n') {
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=') {
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=') {
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else {
			Data++;
			i++;
		}
	}
	return strDecode;
}


std::string ImageProcessBase::base64Encode(const unsigned char* Data, int DataByte) {
	//�����
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//����ֵ
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//��ʣ�����ݽ��б���
	int Mod = DataByte % 3;
	if (Mod == 1) {
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}

//imgType ����png bmp jpg jpeg��opencv�ܹ����б��������ļ�
std::string ImageProcessBase::Mat2Base64(const cv::Mat &img, std::string imgType) {
	//Matתbase64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


cv::Mat ImageProcessBase::Base2Mat(std::string &base64_data) {
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}


bool ImageProcessBase::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
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
//////////////////////////////////////////////////////////////////////////
ImageZoomProcess::ImageZoomProcess(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
{
}

ImageZoomProcess::~ImageZoomProcess()
{
}

bool ImageZoomProcess::Excute(std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(m_in_image_data);
	//cv::imwrite("C:\\ztest2\\a.png", src_image);

	std::string::size_type sz;
	double rate = std::stod(m_str_paras, &sz);

	// ����ͼ��
	cv::Mat dst_image;
	cv::resize(src_image, dst_image, cv::Size(), rate, rate);

	out_image_data = Mat2Base64(dst_image, "png");
	//cv::imwrite("C:\\ztest2\\b.png", dst_image);

	return false;
}

//////////////////////////////////////////////////////////////////////////
ImageRotateProcess::ImageRotateProcess(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
{
}

ImageRotateProcess::~ImageRotateProcess()
{
}
bool ImageRotateProcess::Excute(std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(m_in_image_data);

	std::string::size_type sz;
	double rate = std::stod(m_str_paras, &sz);
	int degree = (int)(rate * 10) % 3;

	// ��תͼ��
	double m[6];
	cv::Mat dst_image;
	cv::rotate(src_image, dst_image, degree);

	out_image_data = Mat2Base64(dst_image, "png");

	return false;
}

//////////////////////////////////////////////////////////////////////////
ImageMoveProcess1::ImageMoveProcess1(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
{
}

ImageMoveProcess1::~ImageMoveProcess1()
{
}
bool ImageMoveProcess1::Excute(std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(m_in_image_data);

	std::string::size_type sz;
	double move_position = std::stod(m_str_paras, &sz);

	int dx = move_position * 100;
	int dy = move_position * 100;

	const int rows = src_image.rows;
	const int cols = src_image.cols;

	// ƽ��ͼ��
	cv::Mat dst_image;
	dst_image.create(rows, cols, src_image.type());

	cv::Vec3b *p;
	for (int i = 0; i < rows ; i++) {
		p = dst_image.ptr<cv::Vec3b>(i);
		for (int j = 0; j < cols ; j++) {
			//ƽ�ƺ�����ӳ�䵽ԭͼ��
			int x = j - dx;
			int y = i - dy;

			//��֤ӳ����������ԭͼ��Χ��
			if (x >=0 && y >= 0 && x < cols && y < rows) {
				p[j] = src_image.ptr<cv::Vec3b>(y)[x];
			}
		}		
	}

	out_image_data = Mat2Base64(dst_image, "png");

	return false;
}

//////////////////////////////////////////////////////////////////////////
ImageMoveProcess2::ImageMoveProcess2(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
{
}

ImageMoveProcess2::~ImageMoveProcess2()
{
}
bool ImageMoveProcess2::Excute(std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(m_in_image_data);

	std::string::size_type sz;
	double move_position = std::stod(m_str_paras, &sz);

	int dx = move_position * 100;
	int dy = move_position * 100;

	const int rows = src_image.rows + abs(dy); //���ͼ��Ĵ�С
	const int cols = src_image.cols + abs(dx);

	// ƽ��ͼ��
	cv::Mat dst_image;
	dst_image.create(rows, cols, src_image.type());

	cv::Vec3b *p;
	for (int i = 0; i < rows ; i++) {
		p = dst_image.ptr<cv::Vec3b>(i);
		for (int j = 0; j < cols ; j++) {
			//ƽ�ƺ�����ӳ�䵽ԭͼ��
			int x = j - dx;
			int y = i - dy;

			//��֤ӳ����������ԭͼ��Χ��
			if (x >=0 && y >= 0 && x < cols && y < rows) {
				p[j] = src_image.ptr<cv::Vec3b>(y)[x];
			}
		}		
	}

	out_image_data = Mat2Base64(dst_image, "png");

	return false;
}
//////////////////////////////////////////////////////////////////////////
Image3DZoomProcess::Image3DZoomProcess(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
	//, reader(NULL)
{
	wnd_mpr1_ = "mpr1";
}

Image3DZoomProcess::~Image3DZoomProcess()
{
}

bool Image3DZoomProcess::Excute(std::string& out_image_data)
{
	// ��ʱ���ȴӱ��ض�ȡDicom�ļ�
	//GNC::GCS::StudyContextMy* my = new GNC::GCS::StudyContextMy();
	//const std::string path_file(""C:\\ztest2\\dicom_test\\413");
	//my->ReadDicomFile(path_file);
	
	// 1.read dcm image from directory
	const std::string path_image_data("C:\\ztest2\\dicom_test");

	std::string::size_type sz;
	double zoom_scale = std::stod(m_str_paras, &sz);

	if (!reader) {
		reader = new VtkDcmLoader();
		reader->LoadDirectory(path_image_data.c_str());	// only once
	
		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData("series1", vol_data);
	
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(wnd_mpr1_, (RenderControlType)req_type);	// only once

		RenderFacade::Get()->ChangeSeries("series1");	
		RenderFacade::Get()->SetOrientation(wnd_mpr1_, SAGITTAL);
		RenderFacade::Get()->RenderControl(wnd_mpr1_);
	}
	
	RenderFacade::Get()->Zoom(wnd_mpr1_, zoom_scale);

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(wnd_mpr1_);
	BITMAP  bitmap ;
	GetObject (hBitmap, sizeof (BITMAP), &bitmap);
	
	std::wstring ws_screenshot_file = L"C:\\ztest2\\haha1111.bmp";
	std::string  s_screenshot_file = "C:\\ztest2\\haha1111.bmp";
	SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());
	
	cv::Mat src = cv::imread(s_screenshot_file.c_str());
	out_image_data = Mat2Base64(src, "bmp");

	return true;
}

/////////////////////////////////////////////////////////////////////////
Image3DRotateProcess::Image3DRotateProcess(std::string str_paras, std::string& in_image_data)
	: ImageProcessBase(str_paras, in_image_data)
{
	wnd_mpr1_ = "mpr1";
}

Image3DRotateProcess::~Image3DRotateProcess()
{
}

bool Image3DRotateProcess::Excute(std::string& out_image_data)
{
	// 1.read dcm image from directory
	const std::string path_image_data("C:\\ztest2\\dicom_test");

	std::string::size_type sz;
	double zoom_scale = std::stod(m_str_paras, &sz);


	if (!reader) {
		reader = new VtkDcmLoader();
		reader->LoadDirectory(path_image_data.c_str());	// only once

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData("series1", vol_data);

		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(wnd_mpr1_, (RenderControlType)req_type);	// only once
		
		RenderFacade::Get()->ChangeSeries("series1");	
		RenderFacade::Get()->SetOrientation(wnd_mpr1_, SAGITTAL);
		RenderFacade::Get()->RenderControl(wnd_mpr1_);
	}

	float f[3] = { 0.0,1.0,0.0 };
	zoom_scale *= 100;
	RenderFacade::Get()->Rotate(wnd_mpr1_, zoom_scale, f);

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(wnd_mpr1_);
	BITMAP  bitmap ;
	GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	std::wstring ws_screenshot_file = L"C:\\ztest2\\haha1111.bmp";
	std::string  s_screenshot_file = "C:\\ztest2\\haha1111.bmp";
	SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());

	cv::Mat src = cv::imread(s_screenshot_file.c_str());
	out_image_data = Mat2Base64(src, "bmp");

	return true;
}