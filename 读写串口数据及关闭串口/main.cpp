#include <iostream>
#include <Windows.h>
using namespace	std;


OVERLAPPED m_ov;
COMSTAT comstat;
DWORD m_dwCommEvent;
HANDLE hComm;

bool openport(const char * portname);
bool setupdcb(int rate_arg);
bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant);
void ReciveChar();
bool WriteChar(const BYTE * m_szWriteBuffer, DWORD m_nTosend);

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
			cout << port.c_str() << " open scuessful!!!" << endl << endl <<"information: "<<endl;
			if (setupdcb(9600))
			{
				cout << "setupDCB scuessful!!!" << endl;
			}
			if (setuptimeout(0,0,0,0,0))  //如果所有写超时参数均为零，则不用写
			{
				cout << "setupTimeout scuessful!!!" << endl;
			}
			PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
			// 在读写串口之前，还要用PurgeComm()函数清空缓冲区
			//PURGE_TXABORT	  中断所有写操作并立即返回，即使写操作还没有完成。
			//PURGE_RXABORT	  中断所有读操作并立即返回，即使读操作还没有完成。
			//PURGE_TXCLEAR	  清除输出缓冲区
			//PURGE_RXCLEAR	  清除输入缓冲区
			PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); // 在读写串口之前，还要用PurgeComm()函数清空缓冲区
			//PURGE_TXABORT	  中断所有写操作并立即返回，即使写操作还没有完成。
			//PURGE_RXABORT	  中断所有读操作并立即返回，即使读操作还没有完成。
			//PURGE_TXCLEAR	  清除输出缓冲区
			//PURGE_RXCLEAR	  清除输入缓冲区
			WriteChar((BYTE*)"please send data now", 20);
			

			cout << "recive data: ";
			ReciveChar();
			break;
		}
		else
		{
			cout << port.c_str() << " open failed" << endl;
		}
	}
	cout << endl;

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
							  //如果在调用CreateFile创建句柄时指
							  //定了FILE_FLAG_OVERLAPPED标志，那么
							  //调用ReadFile和WriteFile对该句柄进
							  //行的操作就应该是重叠的；如果未指定
							  //重叠标志，则读写操作应该是同步的
							  //在同步执行时，函数直到操作完成后才返回。这意味着同步执行时线程会被阻塞，从
							  //而导致效率下降。在重叠执行时，即使操作还未完成，这两个函数也会立即返回，费
							  //时的I/O操作在后台进行
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
	memset(&dcb, 0, sizeof(dcb));//在一段内存块中填充某个给定的值，是对较大的结构
								 //体或数组进行清零操作的一种最快方法
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
//在用readfile和writefile读写串行口时，需要考虑超时问题, 读写串口的超时有两
//种：间隔超时和总超时, 写操作只支持总超时，而读操作两种超时均支持, 如果所有
//写超时参数均为0，那么就不使用写超时。
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = ReadInterval; //读间隔超时
	timeouts.ReadTotalTimeoutConstant = ReadTotalconstant; //读时间系数
	timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier; //读时间常量
	timeouts.WriteTotalTimeoutConstant = WriteTotalconstant; // 写时间系数
	timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier; //写时间常
											//量, 总超时的计算公式是：总超时＝时间系数×要求读/写的字符数＋时间常量
	if (!SetCommTimeouts(hComm, &timeouts))
	{
		return false;
	}
	else
		return true;
}

void ReciveChar()
{
	BOOL bRead = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	char RXBuff;

	bool _case = 1;
	char sentenses[100];
	
	int i = 0;

	for (;;)
	{
		bResult = ClearCommError(hComm, &dwError, &comstat);
		//使用Readfile函数进行读操作时，应使用ClearCommError函数清楚错误

		if (comstat.cbInQue == 0)// COMSTAT结构返回串口信息
								//此处只用到了cbInQue变量，该成员变量代表了输入缓存区的字节数
		{
			continue;
		}
		
		
		
		if (bRead)
		{
			bRead = ReadFile
			(	hComm,		//Hande to COMM 串口的句柄
				&RXBuff,	//RX Buffer Pointer
							//读入数据的地址，即读入的数据将存储在
							//以该指针的值为首的一片内存区
				1,			//Read one Byte 要读入的字节数
				&BytesRead, // Stores number of bytes read, 指向一个DWORD
							//数值，该数值返回读操作实际读入的字节数
				&m_ov		//pointer to m_ov structer

			);				//重叠操作时，该参数指向一个OVERLAPPED结构，同步操作时，该参数为BULL
			//cout << RXBuff;   //测试语句
					
			if(RXBuff != '\n' || i>100)
			{
				sentenses[i] = RXBuff;
				i++;
				if (!bResult)	//当ReadFile和WriteFile返回FALSE时，不一定就是操作失败
								//线程应该调用GetLastError函数分析返回结果
				{
					switch (dwError = GetLastError())
					{
					case ERROR_IO_PENDING:
					{
						bRead = FALSE;
						break;
					}
					default:
						break;
					}
				}
				else
				{
					bRead = TRUE;
				}  //close if(bRead)
				if (!bRead)
				{
					bRead = TRUE;
					bResult = GetOverlappedResult(hComm, &m_ov, &BytesRead, TRUE);
				}
			}
			else
			{
				_case = false;
			}		

			
		}
		if (_case==false)
		{
			break;
		}
	}
	/*int len = strlen(sentenses);      //测试语句
	int len_ = sizeof(sentenses);*/

	for (int j = 0; j < i; j++)
	{
		cout << sentenses[j];
	}
	string msg = "";
	for (int j = 0; j < i; j++)
	{
		msg += sentenses[j];
	}
}
bool WriteChar(const BYTE * m_szWriteBuffer, DWORD m_nToSend)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	HANDLE m_hWriteEven = 0;
	ResetEvent(m_hWriteEven);
	if (bWrite)
	{
		m_ov.Offset = 0;
		m_ov.OffsetHigh = 0;
	}
	// Clear buffer
	bResult = WriteFile
	(	hComm,				// Handle to COMM Port, 串口的句柄

		m_szWriteBuffer,	// Pointer to message buffer in calling finction
							// 即以该指针的值为首地址的nNumberOfBytesToWrite
							// 个字节的数据将要写入串口的发送数据缓冲区

		m_nToSend,	// Length of message to send, 要写入的数据的字节数

		&BytesSent,	 // Where to store the number of bytes sent
					 // 指向指向一个DWORD数值，该数值返回实际写入的字节数

		&m_ov);	    // Overlapped structure
					// 重叠操作时，该参数指向一个OVERLAPPED结构，
					// 同步操作时，该参数为NULL

	if (!bResult)	// 当ReadFile和WriteFile返回FALSE时，不一定就是操作失
					//败，线程应该调用GetLastError函数分析返回的结果
	{
		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
		{
			BytesSent = 0;
			bWrite = FALSE;
			break;	
		}
		default:
			break;
		}
	}
	if (!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult
		(
			hComm,
			&m_ov,
			&BytesSent,
			TRUE
		);
		//deal with the error code
		if (!bResult)
		{
			cout << "GetOverlappedResults() in WriteFile()";
		}
		if (BytesSent != m_nToSend)
		{
			cout << "WARNING: WriteFile() error.. Bytes Sent: " << BytesSent
				<< "; Message Length: " << strlen((char*)m_szWriteBuffer) << endl;
		}
		return TRUE;
	}

}