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
::�õ������Ŀ¼
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cd /d %~dp0
cd ..
SET AppDir=%cd%
cd install

::
::�����Զ�����
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo �������ÿ����Զ�����...

::start %cd%\BrowserApp.exe   %����Dr. Wise Client%
::setlocal EnableDelayedExpansion 

reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /t REG_SZ /d "%AppDir%\BrowserApp.exe" /f   %������д��ע���,����������%
::reg add "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /t REG_SZ /d "%AppDir%\BrowserApp.exe" /f   %������д��ע���,����������%

::
::��echo���������д�뵽p.reg
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo �������ÿͻ����Զ���Э��...

echo Windows Registry Editor Version 5.00 >p.reg
echo.
echo [HKEY_CLASSES_ROOT\dpclient] >>p.reg
echo @="URL:dpclient Protocol Handler" >>p.reg
echo "URL Protocol"="" >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell] >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell\open] >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell\open\command] >>p.reg
echo @="%AppDir:\=\\%\\BrowserApp.exe -risurl=%%1" >>p.reg

::
::ִ��p.reg�ļ�
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo ����д��ע�����Ϣ...

regedit /s p.reg

::
:: ɾ����ʱ���ɵ��ļ�
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
del /q p.reg

echo ��װ��ɣ����س����رմ���......

::
:: ��ͣ�鿴echo�������Ϣ
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
pause>nul %>nul�������ǲ���ʾ�밴���������%