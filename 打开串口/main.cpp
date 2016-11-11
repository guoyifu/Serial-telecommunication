//成功

#include <Windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

bool openport(TCHAR * portname);

int main()
{
	bool open;
	open = openport(_T("com9"));
	if (open)
	{
		cout << "open scuess" << endl;
	}
	else
	{
		cout << "open failed" << endl;
	}
	system("pause");
	return 0;
}

bool openport(TCHAR * portname)
//"char *" 类型的实参与 "LPCWSTR" 类型的形参不兼容	(方法： 用TCHAR代替char)
{
	HANDLE hComm;
	hComm = CreateFile(portname,
		GENERIC_READ | GENERIC_WRITE, 
		0, //必须以独占方式打开
		0, //无安全属性
		OPEN_EXISTING, //通信设备已打开
		FILE_FLAG_OVERLAPPED, //异步I/O
		0); //不能用模板打开
	if (hComm == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hComm);
		return false;
	}
	else
	{
		return true;
	}

}