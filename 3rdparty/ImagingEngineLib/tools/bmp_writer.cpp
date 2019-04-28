#include "tools/bmp_writer.h"
#include<iostream>

using namespace std;


bool SaveBmp1(char *fileName,void *imgBuffer, int imWidth, int imHeight)
{
	if (!imgBuffer)
	{
		return 0;
	}

	int biBitCount = 32;
	int colorTablesize = 0;				// 1024;	//�Ҷ�ͼ����ɫ��
	int lineByte = (imWidth * biBitCount / 8 + 3)/ 4 * 4;
	FILE *fp = fopen(fileName, "wb");

	if (!fp)
	{
		return 0 ;
	}

	LBITMAPFILEHEADER filehead;
	filehead.bfType = 0x4D42;
	filehead.bfSize = sizeof(LBITMAPFILEHEADER) + sizeof(LBITMAPINFOHEADER) + colorTablesize + lineByte * imHeight;
	filehead.bfReserved1 = 0;
	filehead.bfReserved2 = 0;
	filehead.bfOffBits = 54 + colorTablesize;

	//дλͼ�ļ�ͷ���ļ�
	fwrite(&filehead , sizeof(LBITMAPFILEHEADER), 1, fp);

    //����λͼ�ļ���Ϣͷ�ṹ������ ��д�ļ���Ϣͷ��Ϣ
	LBITMAPINFOHEADER infoHead;
	infoHead.biBitCount = biBitCount;
	infoHead.biClrImportant = 0;
	infoHead.biClrUsed = 0;
	infoHead.biSize = 40;
	infoHead.biWidth = imWidth;
	infoHead.biHeight = imHeight;
	infoHead.biPlanes = 1;
	infoHead.biCompression = 0;
	infoHead.biSizeImage = lineByte * imHeight;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;

	fwrite(&infoHead, sizeof(LBITMAPINFOHEADER), 1, fp);
	//fwrite(&filehead, sizeof(LBITMAPFILEHEADER), 1, fp);

	LRGBQUAD *pColorTable = new LRGBQUAD[256];

	for (int i = 0 ; i < 256 ; i++)
	{
		pColorTable[i].rgbBlue = i;
		pColorTable[i].rgbGreen = i;
		pColorTable[i].rgbRed = i;
		//pColorTable[i].rgbReserved = 0;
	}
	fwrite(pColorTable, sizeof(LRGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�
	fwrite(imgBuffer, imHeight*lineByte, 1, fp);
	fclose(fp);

	return 1;
}

// �洢bitmapͼ�񣬵�����Ҫ����bmpͼƬ�洢˳��λBGRBGRBGR...
bool RmwWriteByteImg2BmpFile(const char *filename, BYTE *pImg, int width, int height, int bitcount)
{
	FILE * BinFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER BmpHeader;
	int extend=0;
	bool Suc = true;
	BYTE *pCur;
	BYTE* ex = nullptr;
	unsigned char biBitCount = bitcount;//24λ���ɫ

	extend = (width*(biBitCount/8) + 3) / 4 * 4 - width*(biBitCount / 8);//Ϊ��4�ֽڶ���ÿ����Ҫ��չ��

	// Open File
	if ((BinFile = fopen(filename, "w+b")) == NULL) { return false; }
	//д���ļ�ͷ
	FileHeader.bfType = ((WORD)('M' << 8) | 'B');
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);// +256 * 4L;//2��ͷ�ṹ��ӵ�ɫ��
	FileHeader.bfSize = FileHeader.bfOffBits + (width*(biBitCount / 8) + extend)*height;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	if (fwrite((void *)&FileHeader, 1, sizeof(FileHeader), BinFile) != sizeof(FileHeader)) Suc = false;
	
	// д����Ϣͷ
	BmpHeader.biSize = 40;				//��Ϣͷ��С���̶�40
	BmpHeader.biWidth = width;
	BmpHeader.biHeight = -height;
	BmpHeader.biPlanes = 1;//�̶�ֵ
	BmpHeader.biBitCount = biBitCount;	//����λ�� 8/24/32;
	BmpHeader.biCompression = BI_RGB;	//BI_RGB��ѹ��
	BmpHeader.biSizeImage = 0;			//λͼȫ������ռ�õ��ֽ�����BI_RGBʱ����Ϊ0
	BmpHeader.biXPelsPerMeter = 0;		//ˮƽ�ֱ���(����/��)
	BmpHeader.biYPelsPerMeter = 0;		//��ֱ�ֱ���(����/��)
	BmpHeader.biClrUsed = 0;			//λͼʹ�õ���ɫ��,���Ϊ0������ɫ��Ϊ2��biBitCount�η�
	BmpHeader.biClrImportant = 0;		//��Ҫ����ɫ����0����������ɫ����Ҫ
	if (fwrite((void *)&BmpHeader, 1, sizeof(BmpHeader), BinFile) != sizeof(BmpHeader)) Suc = false;
	
	//// д���ɫ��
	//for (i = 0, p[3] = 0; i<256; i++)
	//{
	//	p[0] = p[1] = p[2] = 255 - i; // blue,green,red;
	//	if (fwrite((void *)p, 1, 4, BinFile) != 4) { Suc = false; break; }
	//}

	if (extend)
	{
		ex = new BYTE[extend]; //��������СΪ 0~3
		memset(ex, 0, extend);
	}

	//// �Ҷ�ֵx0.5
	//for (pCur = pImg; pCur < pImg+ height*(width*(biBitCount / 8) + extend)-1; pCur++)
	//{
	//	*pCur = *pCur *0.5;
	//}
	//write data
	for (pCur = pImg + (height - 1)*(width*(biBitCount / 8) + extend); pCur >= pImg; pCur -= (width*(biBitCount / 8) + extend))
	{
		if (fwrite((void *)pCur, 1, width*(biBitCount / 8), BinFile) != (unsigned int)width) Suc = false; // ��ʵ������
		if (extend) // ��������� �������0
			if (fwrite((void *)ex, 1, extend, BinFile) != 1) Suc = false;
	}



	// return;
	fclose(BinFile);
	if (extend)
		delete[] ex;
	return Suc;
}
