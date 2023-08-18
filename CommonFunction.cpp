#include"���ú���.hpp"

namespace CMath {


    //��ȡ�����細�ڴ���ʧ�ܴ���
    //Ҫʹ��LocalFree(errorMsg)�����ڴ�
    void GetSystemLastError(DWORD* ErrorCode, CHAR** ErrorMsg)
    {
        *ErrorCode = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(ErrorMsg), 0, nullptr);
    }
    //��ȡ�����細�ڴ���ʧ�ܴ���
    //Ҫʹ��LocalFree(errorMsg)�����ڴ�
    void GetSystemLastError(DWORD* ErrorCode, WCHAR** ErrorMsg)
    {
        *ErrorCode = GetLastError();
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(ErrorMsg), 0, nullptr);
    }
    LOGFONT ChooseLogFont_g(HWND hWnd)
    {
        // ��ʾ����ѡ��Ի���
        CHOOSEFONT chooseFont;
        LOGFONT logFont;

        ZeroMemory(&chooseFont, sizeof(CHOOSEFONT));
        ZeroMemory(&logFont, sizeof(LOGFONT));

        chooseFont.lStructSize = sizeof(CHOOSEFONT);
        chooseFont.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
        chooseFont.hwndOwner = hWnd;
        chooseFont.lpLogFont = &logFont;
        chooseFont.nFontType = SCREEN_FONTTYPE;
        ChooseFontW(&chooseFont);
        return logFont;
    }
    COLORREF ChooseCustomColor(HWND hwnd)
    {
        CHOOSECOLOR cc;                 // ��ɫѡ��Ի���ṹ
        static COLORREF customColors[16];  // �Զ�����ɫ����

        // ��ʼ��CHOOSECOLOR�ṹ
        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner = hwnd;
        cc.lpCustColors = (LPDWORD)customColors;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

        // ��ʾ��ɫѡ��Ի���
        if (ChooseColor(&cc))
        {
            return cc.rgbResult;  // ����ѡ�����ɫֵ
        }

        return RGB(0, 0, 0);   // ���ȡ��ѡ�񣬷��غ�ɫ
    }
}