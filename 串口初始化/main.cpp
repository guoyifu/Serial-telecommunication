#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <tchar.h>
using namespace std;

HANDLE hComm;



bool openport(const char * portname);

bool setupdcb(int rate_arg);

bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant);

int main()
{
	bool open;
	

	for (int i = 0; i < 10; i++)
	{
		string port = "com";
		char a[5];
		sprintf_s(a, "%d", i); //转换
		port += a;
		open = openport(port.c_str());
		if (open)
			cout << "open scuess";
		/*else
		{
			cout << port.c_str() << " open failed" << endl;
		}*/
		if (setupdcb(9600))
			cout << "setupDCB success" << endl;
		if (setuptimeout(0, 0, 0, 0, 0))
			cout << "setuptimeout success" << endl;
		SetCommMask(hComm, EV_RXCHAR); //当有字符在inbuf中时产生这个事件
									   //清除串口的所有操作
		PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}


	system("pause");
	return 0;
}

bool openport(const char * portname)
//"char *" 类型的实参与 "LPCWSTR" 类型的形参不兼容	(方法： 用TCHAR代替char)
{
	
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

//设置DCB,先获取DCB配置，再设置，最后看是否设置好   
bool setupdcb(int rate_arg)
{
	DCB  dcb;
	int rate = rate_arg;
	memset(&dcb, 0, sizeof(dcb));//在一段内存块中填充某个给定的值，是对较大的结构//体或数组进行清零操作的一种最快方法
	if (!GetCommState(hComm, &dcb))//获取当前DCB配置
		return FALSE;
	// set DCB to configure the serial port
	dcb.DCBlength = sizeof(dcb);//DCB结构体大小
	/* ---------- Serial Port Config ------- */
	dcb.BaudRate = rate;  //波特率大小
	dcb.Parity = NOPARITY;//奇偶效验
	dcb.fParity = 0;    
	dcb.StopBits = ONESTOPBIT;   //停止读数
	dcb.ByteSize = 8;			 //数据宽度，一般为8，有时候为7 
	dcb.fOutxCtsFlow = 0;		 //CTS线上的硬件握手
	dcb.fOutxDsrFlow = 0;		 //Dsr线上的硬件握手
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutX = 0;				//是否使用XON/XOFF协议
	dcb.fInX = 0;				 //是否使用XON/XOFF协议
	/* ----------------- misc parameters ----- */
	dcb.fErrorChar = 0;
	dcb.fBinary = 1;
	dcb.fNull = 0;
	dcb.fAbortOnError = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 2;
	dcb.XoffLim = 4;
	dcb.XonChar = 0x13;	  //设置表示XON的字符，一般采用Ox11
	dcb.XoffChar = 0x19;  //设置表示XOFF字符的字符， 一般采用Ox13
	dcb.EvtChar = 0;    //reserved ;do not use
	// set DCB
	if (!SetCommState(hComm, &dcb))
		return false;
	else
		return true;
}


bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant)
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = ReadInterval;
	timeouts.ReadTotalTimeoutConstant = ReadTotalconstant;
	timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant = WriteTotalconstant;
	timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier;
	if (!SetCommTimeouts(hComm, &timeouts))
		return false;
	else
		return true;
}


