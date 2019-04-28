#include "StdAfx.h"

#include "EncryptUtil.h"
#include "AES.h"
#include "Base64.h"

//////////////////////////////////////////////////////////////////////////
// �����������
//	1.���������ַ���(content)
//	2.ȥ���ַ����еĻ��з�
//	3.base64_decode����
//	4.AES::CBCģʽ����
//	5.ȥ��AES::PKCS5Padding���
//	6.ʹ��UTF-8�����ַ���
string EncryptDecryptUtils::ExecDecodeDecrypt(const string& content)
{   
	//1.ȥ���ַ����е�\r\n���з�
	string noWrapContent = FormatText(content);
	//2. Base64����
	string strData = base64_decode(noWrapContent);
	size_t length = strData.length();

	//3. newЩ���飬��������
	char *szDataIn = new char[length + 1];
	::memcpy(szDataIn, strData.c_str(), length + 1);
	char *szDataOut = new char[length + 1];
	::memcpy(szDataOut, strData.c_str(), length + 1);

	//4. ����AES��CBCģʽ����
	AES aes;
	//�����ﴫ����Կ����ƫ�������Լ�ָ����Կ���Ⱥ�iv���ȣ���������Կ���Ȳ���16�ֽ�128bit����ô��Ҫ�����ﴫ�����Ӧ�Ĳ�����
	aes.MakeKey(string(AES_KEY).c_str(), string(AES_IV).c_str(), 16, 16);
	//��������д���ָ���ӽ��ܵ�ģʽ��AES::CBC������㲻�����ģʽ����Ҫ�������Ӧ��ģʽ��Դ���ж���ע��˵��
	aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

	//5.ȥPKCS5Padding���:���ܺ���Ҫ���ַ���������ȥ�����������������ȥ��������Ȥ��ȥ������ص�������
	if (0x00 < szDataOut[length - 1] <= 0x16)
	{
		int tmp = szDataOut[length - 1];
		for (int i = length - 1; i >= length - tmp; i--)
		{
			if (szDataOut[i] != tmp)
			{
				::memset(szDataOut, 0, length);
				break;
			}
			else
				szDataOut[i] = 0;
		}
	}

	//6. �������Ƶ����Ĵ�ת��UTF-8��ʽ�ı��뷽ʽ�����
	string srcDest = Utf8ToString(szDataOut);
	//string srcDest = szDataOut;
	delete[] szDataIn;
	delete[] szDataOut;
	return srcDest;
}

// ���ܱ�������
//	1.���������ַ���(content)
//	2.ʹ��Utf-8�����ַ���
//	3.AES::PKCS5Padding ���
//	4.AES::CBCģʽ����
//	5.base64_encode����
string EncryptDecryptUtils::ExecEncryptEncode(const string& content)
{
	//1. �Ȼ�ȡUTF-8�����Ķ����ƴ�
	string utf8Content = StringToUtf8(content);
	size_t length = utf8Content.length();
	int block_num = length / BLOCK_SIZE + 1;

	//2. new Щ���鹩�ӽ���ʹ��
	char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
	::memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
	strcpy(szDataIn, utf8Content.c_str());

	//3. ����PKCS5Padding��䣺����CBCģʽ����ǰ����Ҫ������Ĵ���ȷ�����Է������鶼����ͬ�Ĵ�С��
	// BLOCK_SIZE����AES.h�ж���ĳ�����PKCS5Padding �� PKCS7Padding ������������ BLOCK_SIZE �Ĵ�С�����õ�PKCS5Padding�����Զ���� 8���������ʹ�� PKCS7Padding����ô�͸��������˾����С���� 1-255�е��ĸ�ֵ�޸ļ��ɡ�
	int k = length % BLOCK_SIZE;
	int j = length / BLOCK_SIZE;
	int padding = BLOCK_SIZE - k;
	for (int i = 0; i < padding; i++)
	{
		szDataIn[j * BLOCK_SIZE + k + i] = padding;
	}
	szDataIn[block_num * BLOCK_SIZE] = '\0';

	char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
	::memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

	//4. ����AES��CBCģʽ����
	AES aes;
	//�����ﴫ����Կ����ƫ�������Լ�ָ����Կ���Ⱥ�iv���ȣ���������Կ���Ȳ���16�ֽ�128bit����ô��Ҫ�����ﴫ�����Ӧ�Ĳ�����
	aes.MakeKey(string(AES_KEY).c_str(), string(AES_IV).c_str(), 16, 16);
	//��������д���ָ���ӽ��ܵ�ģʽ��AES::CBC������㲻�����ģʽ����Ҫ�������Ӧ��ģʽ��Դ���ж���ע��˵��
	aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);

	//5. Base64����
	string str = base64_encode((unsigned char*)szDataOut, block_num * BLOCK_SIZE);
	delete[] szDataIn;
	delete[] szDataOut;
	return str;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Base64��������
//	1.���������ַ���(content)
//	2.base64_decode����
//	3.ʹ��UTF-8�����ַ���
string EncryptDecryptUtils::ExecDecode(const string& content)
{
	string noWrapContent = FormatText(content);
	std::string decodedStr = base64_decode(noWrapContent);
	string srcDest = Utf8ToString(decodedStr);
	return srcDest;
}

// Base64��������
//	1.���������ַ���(content)
//	2.ʹ��Utf-8�����ַ���
//	3.base64_encode����
string EncryptDecryptUtils::ExecEncode(const string& content)
{
	string utf8Content = StringToUtf8(content);
	unsigned char* data = (unsigned char*)utf8Content.c_str();
	size_t len = utf8Content.length();
	std::string encodedStr = base64_encode(data, len);

	return encodedStr;
}
//////////////////////////////////////////////////////////////////////////


//ȥ���ַ����еĿո񡢻��з�
string EncryptDecryptUtils::FormatText(const string& src)
{
	int len = src.length();
	char *dst = new char[len + 1];
	int i = -1, j = 0;
	while (src[++i])
	{
		switch (src[i])
		{
		case '\n':
		case '\t':
		case '\r':
			continue;
		}
		dst[j++] = src[i];
	}
	dst[j] = '\0';
	string rel = string(dst);
	delete dst;
	return rel;
}