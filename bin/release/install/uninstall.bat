@echo off

::
::��ȡ����ԱȨ��
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo �������ó�������ԱȨ��... 
%1 %2
ver|find "5.">nul&&goto :st
mshta vbscript:createobject("shell.application").shellexecute("%~s0","goto :st","","runas",1)(window.close)&goto :eof

:st
copy "%~0" "%windir%\system32\"

::
::ж�أ�ɾ��ע�����Ϣ
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo ����ɾ��ע�����Ϣ...

taskkill /F /IM browserapp.exe 2>nul
reg delete "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /f 2>nul
reg delete "HKEY_CLASSES_ROOT\dpclient" /f 2>nul

echo ж����ɣ����س����رմ���...

::
:: ��ͣ�鿴echo�������Ϣ
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
pause>nul %>nul�������ǲ���ʾ�밴���������%