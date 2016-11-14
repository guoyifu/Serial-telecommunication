#include <Windows.h>
#include <iostream>

using namespace std;

bool openport(const char * portname);

int main()
{
	cout << "Serial Port: ";
	string com = "com";

	for (int i = 0; i < 11; i++)
	{
		string port = "com";
		char a[5];
		sprintf_s(a, "%d", i);
		port += a;
		bool open = openport(port.c_str());
		if (open)
		{
			cout << port.c_str() << " open scuessful" << endl;
			break;
		}
		else
		{
			cout << port.c_str() << " open failed" << endl;
		}
	}

	system("pause");
	return 0;
}
bool openport(const char * portname)
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