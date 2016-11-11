#include <Windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

bool openport(TCHAR * portname);

int main()
{
	cout << "Serial Port: ";
	string com = "com";
	
	if (openport(_T("com1")))
	{
		cout << "com1" << endl;
	}
	if (openport(_T("com2")))
	{
		cout << "com2" << endl;
	}
	if (openport(_T("com3")))
	{
		cout << "com3" << endl;
	}
	if (openport(_T("com4")))
	{
		cout << "com4" << endl;
	}
	if (openport(_T("com5")))
	{
		cout << "com5" << endl;
	}

	if (openport(_T("com6")))
	{
		cout << "com6" << endl;
	}
	
	if (openport(_T("com7")))
	{
		cout << "com7" << endl;
	}
	if (openport(_T("com8")))
	{
		cout << "com8" << endl;
	}

	if (openport(_T("com9")))
	{
		cout << "com9" << endl;
	}
	
	if (openport(_T("com10")))
	{
		cout << "com10" << endl;
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