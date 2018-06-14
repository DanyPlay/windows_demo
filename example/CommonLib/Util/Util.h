#pragma once
#include <shlobj.h>
#include <map>
#include "../string/tstring.h"

#ifdef UNICODE
#define STR2T(str)      Util::Encode::Utf82Unicode(str)
#define STR_U2T(str)    Util::Encode::Utf82Unicode(str)
#define STR_A2T(str)    Util::Encode::ASCIIToWide(str)
#define STR2A(str)      Util::Encode::WideToASCII(str)
#define STR_T2U(str)    Util::Encode::TransWStringToUTF8(str.c_str())
#else
#define STR2T(str)      str
#define STR_U2T(str)    str
#define STR_S2T(str)    str
#define STR2A(str)      str
#define STR_T2U(str)      str
#endif // !UNICODE

namespace Util
{
    namespace Time
    {
        time_t CurrentTime();
        std::string CurrentTimeString();
        std::string CurrentDateTimeString();
        std::string GenerateTimeDateFilename(const char *extension, bool noSpace = false);
        std::string GenerateSpecifiedFilename(const char *extension, bool noSpace, const char *format);

        std::string format_hhmmss(int time);

        time_t StringToDatetime(const char *str);
        time_t GetCurrentTime();
    }

    namespace Encode
    {
        std::string TransWStringToUTF8(const wchar_t* apFrom);
        std::wstring TransUTF8ToWString(const char * apFrom);

        std::string UTF82ASCII(std::string& strUtf8Code);
        std::wstring Utf82Unicode(const std::string& utf8string);

        std::string ASCII2UTF8(std::string& ascii);

        std::string WideToASCII(const std::wstring& wide);
        std::wstring ASCIIToWide(const std::string& ascii);

        std::string URLDecode(const std::string& inTmp);
        std::string URLEncode(const std::string& inTmp);
        int URLEncode(const char* str, const int strSize, char* result, const int resultSize);
    }

    namespace String
    {
        int replace(std::wstring& str, const std::wstring& pattern, const std::wstring& newpat);
        std::string find(const std::string& str, const std::string& begin, const std::string& end);
        std::string StrFormat(const char * format, ...);

        std::string Int2Str(unsigned int value);
        std::string Double2Str(double value);

        std::string Map2Str(const std::map<std::string, std::string>& value);
    }

    namespace OS
    {
        int GetOSVersion();
        const char* GetTheFirstMac();
    }

    namespace Progress
    {
        BOOL CreateProcessEx(LPCTSTR lpszExe, LPCTSTR lpszParam, DWORD dwTimeout, DWORD* pdwReturn = NULL);
        std::wstring GetProcessName(DWORD pid);
    }

    namespace Display
    {
        void SetAeroEnabled(bool enable);

        void ForceShowWindow(HWND hWNd);
    }

    namespace File
    {
        std::wstring BrowseFolder(HWND hWnd, LPCTSTR lpszTitle, LPCTSTR lpszFolder, UINT ulFlags = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS);

        // 强制创建目录
        BOOL ForceCreateDir(LPCTSTR pszFullPathFileName);
        // 创建目录,存在和创建成功返回true
        BOOL CreateMyDir(LPTSTR pszDir);

        BOOL GetFileVersion(LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen);
        BOOL GetFileVersion(LPCTSTR strFile, VS_FIXEDFILEINFO** pInfo);
        tstring GetFileVersion(LPCTSTR strFile);
        tstring GetFileVersion(HMODULE hModule);
    }

};