#pragma once

#include <string>

using namespace std;

#ifndef AES_INFO
#define AES_INFO

#define AES_KEY "zV4pLw69R3gGFqrM"		//AES 16B����Կ
#define AES_IV "vN2jMe7INLXFCRUE"		//AES CBC�ӽ���ģʽ�����ƫ����

#endif 

class EncryptDecryptUtils {
public:
	EncryptDecryptUtils(){};
	~EncryptDecryptUtils(){};
	//�������
	static string ExecDecodeDecrypt(const string& content);
	//���ܱ���
	static string ExecEncryptEncode(const string& content);
	//����
	static string ExecDecode(const string& content);
	//����
	static string ExecEncode(const string& content);
private:
	//ȥ���ַ����еĿո񡢻��з�
	static string FormatText(const string& content);
};