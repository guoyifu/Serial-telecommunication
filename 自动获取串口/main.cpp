
#include <iostream>
#include <Windows.h>
#include <afx.h>
using namespace std;
int main()
{
	int i = 0;
	CHAR Name[25];
	UCHAR szPortName[25];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName = sizeof(Name);
	DWORD dwSizeofPortName = sizeof(szPortName);
	DWORD Type;
	HKEY hKey;
	CString strSerialList[256]; // 临时定义 256 个字符串组，因为系统最多也就 256 个 
	LPCTSTR data_Set = "HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	//dwName = sizeof(Name);
	//dwSizeofPortName = sizeof(szPortName);
	//long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey)); 
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey); //打开一个制定的注册表键,成功返回ERROR_SUCCESS即“0”值
	if (ret0 == ERROR_SUCCESS)
	{
		do
		{

			Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName);//读取键值 
			if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
			{
				strSerialList[i] = CString(szPortName); // 串口字符串保存 
														//printf("serial:%s\n", strSerialList[i]);
				cout << "serial: " << strSerialList[i] << endl;
				i++;// 串口计数 
			}
			//每读取一次dwName和dwSizeofPortName都会被修改 
			//注意一定要重置,否则会出现很离奇的错误,本人就试过因没有重置,出现先插入串口号大的（如COM4）,再插入串口号小的（如COM3），此时虽能发现两个串口，但都是同一串口号（COM4）的问题，同时也读不了COM大于10以上的串口 
			dwName = sizeof(Name);
			dwSizeofPortName = sizeof(szPortName);
		} while ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA));

		RegCloseKey(hKey);
	}


	//HKEY hKey;

	//if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	//{
	//	TCHAR szPortName[256], szComName[256];
	//	DWORD dwLong, dwSize;
	//	int nCount  = 0;
	//	/*CComboBox* pCombo   = (CComboBox*)GetDlgItem(IDC_COMBO_COM);*/

	//	//pCombo->ResetContent();
	//	while (true)
	//	{
	//		dwLong = dwSize  = 256;
	//		if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
	//			break;

	//		pCombo->InsertString(nCount, szComName);
	//		nCount++;
	//	}
	//	RegCloseKey(hKey);
	//	pCombo->SetCurSel(0);
	//}

	system("pause");
}