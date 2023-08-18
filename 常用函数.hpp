#pragma once
#include<Windows.h>

namespace CMath {

	//��ȡ�����細�ڴ���ʧ�ܴ���
	//Ҫʹ��LocalFree(errorMsg)�����ڴ�
	void GetSystemLastError(DWORD* ErrorCode, CHAR** ErrorMsg);
	//��ȡ�����細�ڴ���ʧ�ܴ���
	class GetSystemLastErrorClass
	{
		DWORD m_ErrorCode;
		char* m_ErrorMsg;
	public:
		GetSystemLastErrorClass()
		{
			m_ErrorCode = GetLastError();
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, m_ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&m_ErrorMsg), 0, nullptr);
		}
		GetSystemLastErrorClass(DWORD* ErrorCode, CHAR** ErrorMsg)
		{
			*ErrorCode = GetLastError();
			m_ErrorCode = *ErrorCode;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(ErrorMsg), 0, nullptr);
			m_ErrorMsg = *ErrorMsg;
		}
		DWORD GetErrorCode()
		{
			return m_ErrorCode;
		}
		char* GetErrorMsg()
		{
			return m_ErrorMsg;
		}
		~GetSystemLastErrorClass()
		{
			LocalFree(m_ErrorMsg);
		}
	};
	//��ȡ�����細�ڴ���ʧ�ܴ���
	//Ҫʹ��LocalFree(errorMsg)�����ڴ�
	void GetSystemLastError(DWORD* ErrorCode, WCHAR** ErrorMsg);
	//��ȡ�����細�ڴ���ʧ�ܴ���
	class GetSystemLastErrorClassW
	{
		DWORD m_ErrorCode;
		WCHAR* m_ErrorMsg;
	public:
		GetSystemLastErrorClassW()
		{
			m_ErrorCode = GetLastError();
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, m_ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&m_ErrorMsg), 0, nullptr);
		}
		GetSystemLastErrorClassW(DWORD* ErrorCode, WCHAR** ErrorMsg)
		{
			*ErrorCode = GetLastError();
			m_ErrorCode = *ErrorCode;
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(ErrorMsg), 0, nullptr);
			m_ErrorMsg = *ErrorMsg;
		}
		DWORD GetErrorCode()
		{
			return m_ErrorCode;
		}
		wchar_t* GetErrorMsg()
		{
			return m_ErrorMsg;
		}
		~GetSystemLastErrorClassW()
		{
			LocalFree(m_ErrorMsg);
		}

	};
	LOGFONT ChooseLogFont_g(HWND hWnd);
	COLORREF ChooseCustomColor(HWND hwnd);
}