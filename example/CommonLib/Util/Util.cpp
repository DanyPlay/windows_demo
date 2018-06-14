#include "stdafx.h"
#include "Util.h"
using namespace std;
#include <chrono>
#include <vector>
#include <Nb30.h>
#include <assert.h>

#pragma warning(disable: 4996)
#pragma warning(disable: 4819)

#if _MSC_VER < 1900
#define snprintf _snprintf
#endif

#pragma comment( lib, "Netapi32.lib" )
#pragma comment(lib, "Version")
#pragma comment(lib, "shlwapi")

char g_strTempBuf[1024];

time_t Util::Time::CurrentTime()
{
    using namespace std::chrono;

    auto tp = system_clock::now();
    time_t now = system_clock::to_time_t(tp);

    return now;
}

string Util::Time::CurrentTimeString()
{
    using namespace std::chrono;

    struct tm  tstruct;
    char       buf[80];

    auto tp = system_clock::now();
    auto now = system_clock::to_time_t(tp);
    int err = localtime_s(&tstruct, &now);

    size_t written = strftime(buf, sizeof(buf), "%X", &tstruct);
    if (ratio_less<system_clock::period, seconds::period>::value &&
        written && (sizeof(buf) - written) > 5) {
        auto tp_secs =
            time_point_cast<seconds>(tp);
        auto millis =
            duration_cast<milliseconds>(tp - tp_secs).count();

        snprintf(buf + written, sizeof(buf) - written, ".%03u",
            static_cast<unsigned>(millis));
    }

    return buf;
}

string Util::Time::CurrentDateTimeString()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d, %X", &tstruct);
    return buf;
}


string Util::Time::GenerateTimeDateFilename(const char *extension, bool noSpace)
{
    time_t    now = time(0);
    char      file[256] = {0};
    struct tm *cur_time;

    cur_time = localtime(&now);
    if (cur_time) {
        snprintf(file, sizeof(file), "%d-%02d-%02d%c%02d-%02d-%02d.%s",
            cur_time->tm_year + 1900,
            cur_time->tm_mon + 1,
            cur_time->tm_mday,
            noSpace ? '_' : ' ',
            cur_time->tm_hour,
            cur_time->tm_min,
            cur_time->tm_sec,
            extension);
    }

    return string(file);
}

string Util::Time::GenerateSpecifiedFilename(const char *extension, bool noSpace, const char *format)
{
    time_t now = time(0);
    struct tm *cur_time;
    cur_time = localtime(&now);

    const size_t spec_count = 23;
    const char *spec[][2] = {
        { "%CCYY", "%Y" },
        { "%YY", "%y" },
        { "%MM", "%m" },
        { "%DD", "%d" },
        { "%hh", "%H" },
        { "%mm", "%M" },
        { "%ss", "%S" },
        { "%%", "%%" },

        { "%a", "" },
        { "%A", "" },
        { "%b", "" },
        { "%B", "" },
        { "%d", "" },
        { "%H", "" },
        { "%I", "" },
        { "%m", "" },
        { "%M", "" },
        { "%p", "" },
        { "%S", "" },
        { "%y", "" },
        { "%Y", "" },
        { "%z", "" },
        { "%Z", "" },
    };

    char convert[128] = {};
    string sf = format;
    string c;
    size_t pos = 0, len;

    while (pos < sf.length()) {
        len = 0;
        for (size_t i = 0; i < spec_count && len == 0; i++) {

            if (sf.find(spec[i][0], pos) == pos) {
                if (strlen(spec[i][1]))
                    strftime(convert, sizeof(convert),
                    spec[i][1], cur_time);
                else
                    strftime(convert, sizeof(convert),
                    spec[i][0], cur_time);

                len = strlen(spec[i][0]);

                c = convert;
                if (c.length() && c.find_first_not_of(' ') !=
                    std::string::npos)
                    sf.replace(pos, len, convert);
            }
        }

        if (len)
            pos += strlen(convert);
        else if (!len && sf.at(pos) == '%')
            sf.erase(pos, 1);
        else
            pos++;
    }

    if (noSpace)
        replace(sf.begin(), sf.end(), ' ', '_');

    sf += '.';
    sf += extension;

    return (sf.length() < 256) ? sf : sf.substr(0, 255);
}

std::string Util::Time::format_hhmmss(int time)
{
    int seconds = time % 60;
    int totalMinutes = time / 60;
    int minutes = totalMinutes % 60;
    int hours = totalMinutes / 60;

    char ret[256] = {};
    snprintf(ret, sizeof(ret), "%02u:%02u:%02u", hours, minutes, seconds);

    return std::string(ret);
}

time_t Util::Time::StringToDatetime(const char *str)
{
    tm tm_;
    int year, month, day, hour, minute, second;
    sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    tm_.tm_year = year - 1900;
    tm_.tm_mon = month - 1;
    tm_.tm_mday = day;
    tm_.tm_hour = hour;
    tm_.tm_min = minute;
    tm_.tm_sec = second;
    tm_.tm_isdst = 0;

    time_t t_ = mktime(&tm_); //已经减了8个时区
    return t_; //秒时间
}

time_t Util::Time::GetCurrentTime()
{
    time_t t;  //秒时间

    t = time(NULL); //获取目前秒时间

    return t;
}

/*****************************************************************************/

std::string Util::Encode::TransWStringToUTF8(const wchar_t* apFrom)
{
    if (!apFrom || !apFrom[0])
        return "";

    int liOutNeedLen = WideCharToMultiByte(CP_UTF8, 0, apFrom, -1, NULL, NULL, NULL, NULL);
    if (liOutNeedLen > 0)
    {
        std::vector<char> lvcWideChar(liOutNeedLen);
        char * lpBuff = &lvcWideChar.front();

        liOutNeedLen = WideCharToMultiByte(CP_UTF8, 0, apFrom, -1, lpBuff, liOutNeedLen, NULL, NULL);
        if (liOutNeedLen > 0)
        {
            return std::string(lpBuff);
        }
    }
    return "";
}

std::wstring Util::Encode::TransUTF8ToWString(const char * apFrom)
{
    if (!apFrom || !apFrom[0])
        return L"";

    int liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apFrom, -1, NULL, 0);
    if (liOutNeedLen > 0)
    {
        std::vector<wchar_t> lvcWideChar(liOutNeedLen);
        wchar_t * lpBuff = &lvcWideChar.front();

        liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apFrom, -1, lpBuff, liOutNeedLen);
        if (liOutNeedLen > 0)
        {
            return std::wstring(lpBuff);
        }
    }
    return L"";
}

std::string Util::Encode::UTF82ASCII(std::string& strUtf8Code)
{
    std::string strRet("");
    //先把 utf8 转为 unicode
    std::wstring wstr = TransUTF8ToWString(strUtf8Code.c_str());
    //最后把 unicode 转为 ascii
    strRet = WideToASCII(wstr.c_str());
    return strRet;
}

std::wstring Util::Encode::Utf82Unicode(const std::string& utf8string)
{
    int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)
    {
        throw std::exception("Invalid UTF-8 sequence.");
    }
    if (widesize == 0)
    {
        throw std::exception("Error in conversion.");
    }

    wchar_t *resultstring = new wchar_t[widesize];

    int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, resultstring, widesize);
    resultstring[convresult - 1] = 0;
    if (convresult != widesize)
    {
        throw std::exception("La falla!");
    }

    std::wstring ret(resultstring);
    delete[] resultstring;

    return ret;
}

std::string Util::Encode::ASCII2UTF8(std::string& ascii)
{
    std::wstring wstr = ASCIIToWide(ascii);
    return TransWStringToUTF8(wstr.c_str());
}

std::string Util::Encode::WideToASCII(const std::wstring& wide)
{
    //return std::string(wide.begin(), wide.end());
    if (wide.length() == 0)
        return "";

    std::string result;

    int nLen = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
    if (nLen <= 0)
    {
        return std::string("");
    }
    char *presult = new char[nLen];
    if (NULL == presult)
    {
        return std::string("");
    }
    WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, presult, nLen, NULL, NULL);
    presult[nLen - 1] = 0;
    result = presult;
    delete[] presult;

    return result;
}

std::wstring Util::Encode::ASCIIToWide(const std::string& ascii)
{
    //return std::wstring(ascii.begin(), ascii.end());
    if (ascii.length() == 0)
        return L"";

    int nLen = ascii.length();
    int nSize = MultiByteToWideChar(CP_ACP, 0, ascii.c_str(), nLen, 0, 0);
    if (nSize <= 0)
    {
        return std::wstring(L"");
    }
    WCHAR *pDst = new WCHAR[nSize + 1];
    if (NULL == pDst)
    {
        return NULL;
    }
    MultiByteToWideChar(CP_ACP, 0, ascii.c_str(), nLen, pDst, nSize);
    pDst[nSize] = 0;
    if (0xFEFF == pDst[0])
    {
        for (int i = 0; i < nSize; i++)
        {
            pDst[i] = pDst[i + 1];
        }
    }
    std::wstring wcstr(pDst);
    //delete[] pDst;
    return wcstr;
}

inline BYTE fromHex(const BYTE &x)
{
    return x > 64 ? x - 55 : x - 48;
}

std::string Util::Encode::URLDecode(const std::string & inTmp)
{
    std::string strResult = "";
    int inlen = inTmp.length();
    for (int i = 0; i < inlen; i++)
    {
        if (inTmp.at(i) == '%')
        {
            if (i < inlen - 2)//保证不会越界访问
            {
                i++;
                char c = fromHex(inTmp.at(i++));
                c = c << 4;
                c += fromHex(inTmp.at(i));
                strResult += c;
            }
            else
            {
                break;//原始字符串有问题
            }
        }
        else if (inTmp.at(i) == '+')
            strResult += ' ';
        else
            strResult += inTmp.at(i);
    }
    return strResult;
}

std::string Util::Encode::URLEncode(const std::string& inTmp)
{
    if (inTmp.empty())
        return "";

    int length = inTmp.length();
    char* szBuf = new char[length * 3] {0};
    URLEncode(inTmp.c_str(), length, szBuf, length * 3);
    std::string ret = szBuf;
    delete szBuf;

    return ret;
}

int Util::Encode::URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
    int i = 0;
    int j = 0;
    char ch = 0;

    if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0))
        return   0;

    for (i = 0; (i < strSize) && (j < resultSize); i++)
    {
        ch = str[i];
        if ((ch >= 'A') && (ch <= 'Z'))	{
            result[j++] = ch;
        }
        else if ((ch >= 'a') && (ch <= 'z'))	{
            result[j++] = ch;
        }
        else if ((ch >= '0') && (ch <= '9')) {
            result[j++] = ch;
        }
        else if (ch == '   ') {
            result[j++] = '+';
        }
        else {
            if (j + 3 < resultSize)   {
                sprintf(result + j, "%%%02X", (unsigned char)ch);
                j += 3;
            }
            else
            {
                return   0;
            }
        }
    }

    result[j] = '\0';
    return j;
}

/*****************************************************************************/

int Util::String::replace(std::wstring& str, const std::wstring& pattern, const std::wstring& newpat)
{
    int count = 0;
    const size_t nsize = newpat.size();
    const size_t psize = pattern.size();

    for (size_t pos = str.find(pattern, 0);
        pos != std::string::npos;
        pos = str.find(pattern, pos + nsize))
    {
        str.replace(pos, psize, newpat);
        count++;
    }

    return count;
}

std::string Util::String::find(const std::string& str, const std::string& begin, const std::string& end)
{
    string result;

    string::size_type start_str_pos, end_str_pos, start_cut_pos;
    if ((start_str_pos = str.find(begin)) == string::npos)
        return result;
    else{
        start_cut_pos = start_str_pos + begin.length();
        if ((end_str_pos = str.find(end, start_cut_pos)) == string::npos)
            return result;
    }

    if (start_cut_pos == end_str_pos)
        return result;
    else
        result.assign(str.begin() + start_cut_pos, str.begin() + end_str_pos);

    return result;
}

std::string Util::String::StrFormat(const char * format, ...)
{
    char buf[1024];
    va_list arglist;
    va_start(arglist, format);
    _vsnprintf_s(buf, 1024, format, arglist);
    va_end(arglist);
    return std::string(buf);
}

std::string Util::String::Int2Str(unsigned int value)
{
    char ret[65] = { 0 };
    _itoa_s(value, ret, ARRAYSIZE(ret), 10);
    return ret;
}

std::string Util::String::Double2Str(double value)
{
    char ret[65] = { 0 };
    _snprintf_s(ret, ARRAYSIZE(ret), _TRUNCATE, "%f", value);
    return ret;
}

std::string Util::String::Map2Str(const std::map<std::string, std::string>& value)
{
    if (value.size() == 0)
        return "";

    std::string ret;
    for (std::map<std::string, std::string>::const_iterator itor = value.begin(); itor != value.end(); ++itor)
    {
        if (itor != value.begin())
            ret += "&";

        ret += itor->first + "=" + itor->second;
    }

    return ret;
}

/*****************************************************************************/

int Util::OS::GetOSVersion()
{
    OSVERSIONINFO osVersionInfo = { 0 };
    osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
    GetVersionEx(&osVersionInfo);

    if (osVersionInfo.dwMajorVersion > 6)
        return 8;

    if (osVersionInfo.dwMajorVersion == 6)
    {
        //Windows 8
        if (osVersionInfo.dwMinorVersion >= 2)
            return 8;

        //Windows 7
        if (osVersionInfo.dwMinorVersion == 1)
            return 7;

        //Vista
        if (osVersionInfo.dwMinorVersion == 0)
            return 6;
    }

    return 0;
}

const char* Util::OS::GetTheFirstMac()
{
    typedef struct _ASTAT_
    {
        ADAPTER_STATUS adapt;
        NAME_BUFFER    NameBuff[30];
    }ASTAT, *PASTAT;

    ASTAT Adapter;

    NCB ncb;
    UCHAR uRetCode;
    LANA_ENUM lana_enum_buf;

    memset(g_strTempBuf, 0, sizeof(g_strTempBuf));

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBENUM;

    ncb.ncb_buffer = (unsigned char *)&lana_enum_buf;
    ncb.ncb_length = sizeof(lana_enum_buf);

    // 向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡、每张网卡的编号等 
    uRetCode = Netbios(&ncb);
    //	printf( "The NCBENUM return		code is: 0x%x \n", uRetCode );
    if (uRetCode != 0)
    {
        return g_strTempBuf;
    }

    int nIndex = 0;
    //	for(int nIndex = 0; nIndex < lana_enum_buf.length; nIndex++)
    {
        int lana_num = lana_enum_buf.lana[nIndex];

        memset(&ncb, 0, sizeof(ncb));
        ncb.ncb_command = NCBRESET;
        ncb.ncb_lana_num = lana_num;
        // 指定网卡号

        // 首先对选定的网卡发送一个NCBRESET命令，以便进行初始化 
        uRetCode = Netbios(&ncb);
        //    printf( "The NCBRESET return code is:   0x%x \n", uRetCode );

        memset(&ncb, 0, sizeof(ncb));
        ncb.ncb_command = NCBASTAT;
        ncb.ncb_lana_num = lana_num;     // 指定网卡号

        memcpy(ncb.ncb_callname, "*               ", sizeof(ncb.ncb_callname));
        ncb.ncb_buffer = (unsigned char *)&Adapter;

        // 指定返回的信息存放的变量 
        ncb.ncb_length = sizeof(Adapter);

        // 接着，可以发送NCBASTAT命令以获取网卡的信息 
        uRetCode = Netbios(&ncb);
        //    printf( "The NCBASTAT   return code is: 0x%x \n", uRetCode );
        if (uRetCode == 0)
        {
            // 把网卡MAC地址格式化成常用的16进制形式，如0010A4E45802 
            char szTemp[33] = { 0 };
            _snprintf_s(szTemp, sizeof(szTemp), 32, "%02X%02X%02X%02X%02X%02X",
                Adapter.adapt.adapter_address[0],
                Adapter.adapt.adapter_address[1],
                Adapter.adapt.adapter_address[2],
                Adapter.adapt.adapter_address[3],
                Adapter.adapt.adapter_address[4],
                Adapter.adapt.adapter_address[5]);

            strcat_s(g_strTempBuf, sizeof(g_strTempBuf), szTemp);
        }
    }

    return g_strTempBuf;
}

/*****************************************************************************/

BOOL Util::Progress::CreateProcessEx(LPCTSTR lpszExe, LPCTSTR lpszParam, DWORD dwTimeout, DWORD* pdwReturn /*= NULL*/)
{
    BOOL bRet = FALSE;
    if (!PathFileExists(lpszExe))
        return bRet;

    TCHAR szCmdLine[2048 * 2] = { 0 };
    if (lpszParam && lpszParam[0])
        _sntprintf(szCmdLine, sizeof(szCmdLine) - 1, _T("\"%s\" %s"), lpszExe, lpszParam);
    else
        _sntprintf(szCmdLine, sizeof(szCmdLine) - 1, _T("\"%s\""), lpszExe);

    TCHAR szCurDir[MAX_PATH] = { 0 };
    _sntprintf(szCurDir, sizeof(szCurDir) - 1, _T("%s"), lpszExe);
    PathRemoveFileSpec(szCurDir);

    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_FORCEOFFFEEDBACK;
    PROCESS_INFORMATION pi = { 0 };
    if (CreateProcess(lpszExe, szCmdLine, NULL, NULL, FALSE, 0, NULL, szCurDir, &si, &pi))
    {
        if (dwTimeout)
        {
            DWORD dwRet = WaitForSingleObject(pi.hProcess, dwTimeout);
            if (WAIT_OBJECT_0 == dwRet)
                bRet = TRUE;
        }
        else
            bRet = TRUE;
        if (pdwReturn)
        {
            GetExitCodeProcess(pi.hProcess, pdwReturn);
        }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    return bRet;
}

std::wstring Util::Progress::GetProcessName(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == INVALID_HANDLE_VALUE)
    {
        return L"";
    }

    TCHAR processName[MAX_PATH] = { 0 };
    DWORD dwSize = sizeof(processName);
    BOOL ret = QueryFullProcessImageName(hProcess, 0, processName, &dwSize);

    CloseHandle(hProcess);

    return ret ? PathFindFileName(processName) : L"";
}

/*****************************************************************************/

#include <dwmapi.h>

void Util::Display::SetAeroEnabled(bool enable)
{
    static HRESULT(WINAPI *func)(UINT) = nullptr;
    static bool failed = false;

    if (!func) {
        if (failed) {
            return;
        }

        HMODULE dwm = LoadLibraryW(L"dwmapi");
        if (!dwm) {
            failed = true;
            return;
        }

        func = reinterpret_cast<decltype(func)>(GetProcAddress(dwm, "DwmEnableComposition"));
        if (!func) {
            failed = true;
            return;
        }
    }

    func(enable ? DWM_EC_ENABLECOMPOSITION : DWM_EC_DISABLECOMPOSITION);
}

void Util::Display::ForceShowWindow(HWND hWNd)
{
    if (::IsIconic(hWNd))
        ::ShowWindow(hWNd, SW_RESTORE);
    ::ShowWindow(hWNd, SW_SHOWNA);

    HWND hForeWnd = ::GetForegroundWindow();
    DWORD dwForeID = ::GetWindowThreadProcessId(hForeWnd, NULL);
    DWORD dwCurID = ::GetCurrentThreadId();
    ::AttachThreadInput(dwCurID, dwForeID, TRUE);
    ::SetWindowPos(hWNd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    ::SetWindowPos(hWNd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    ::SetForegroundWindow(hWNd);
    ::SetActiveWindow(hWNd);
    ::AttachThreadInput(dwCurID, dwForeID, FALSE);
}

namespace Util
{
    namespace File
    {
        int CALLBACK BrowseForFolderCallbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
                case BFFM_INITIALIZED:
                {
                    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lParam);
                    ::SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, lParam);
                    break;
                }
                case BFFM_SELCHANGED:
                {
                    //TCHAR pszPath[MAX_PATH] = { 0 };
                    //SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);
                    //::SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)pszPath);
                    break;
                }
            }
            return 0;
        }
    }
}

std::wstring Util::File::BrowseFolder(HWND hWnd, LPCTSTR lpszTitle, LPCTSTR lpszFolder, UINT ulFlags /*= BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS*/)
{
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = hWnd;
    bi.lpszTitle = lpszTitle;
    bi.ulFlags = ulFlags;
    bi.lpfn = (BFFCALLBACK)BrowseForFolderCallbackProc;
    bi.lParam = (LPARAM)lpszFolder;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (!pidl)
    {
        return L"";
    }

    TCHAR* path = new TCHAR[MAX_PATH];
    SHGetPathFromIDList(pidl, path);

    return path;
}

BOOL Util::File::ForceCreateDir(LPCTSTR pszFullPathFileName)
{
    if (!pszFullPathFileName || !pszFullPathFileName[0])
        return FALSE;

    if (PathIsRoot(pszFullPathFileName))
        return TRUE;

    TCHAR Dir[MAX_PATH + 1] = {};
    int	 nNeed;
    LPCTSTR p, pLast;
    BOOL  Result;

    Result = FALSE;
    assert(pszFullPathFileName);
    pLast = pszFullPathFileName;
    if (_tcslen(pLast) > _MAX_PATH) return FALSE;
    while (NULL != *pLast)
    {
        p = _tcsstr(pLast, _T("\\"));
        if (NULL == p)
        {
            p = _tcsstr(pLast, _T("/"));

            if (NULL == p)
                return Result;
        }
        nNeed = p - pszFullPathFileName;
        if (nNeed > 0)
        {
            memset(Dir, 0, sizeof(Dir));
            _tcsncpy_s(Dir, pszFullPathFileName, p - pszFullPathFileName);
            Result = CreateMyDir(Dir);
        }
        p++;
        pLast = p;
    }
    return Result;
}

BOOL Util::File::CreateMyDir(LPTSTR pszDir)
{
    assert(pszDir);

    if (!pszDir || !pszDir[0])
        return FALSE;

    BOOL bRet;
    bRet = CreateDirectory(pszDir, NULL);
    if (FALSE == bRet)
    {
        if (ERROR_ALREADY_EXISTS == GetLastError())
            return TRUE;
    }
    return bRet;
}

BOOL Util::File::GetFileVersion(LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen)
{
    if (!strFile || !strFile[0] || !pszVersion || nVersionLen <= 0)
        return FALSE;

    TCHAR  szVersionBuffer[8192] = {};
    DWORD dwVerSize;
    DWORD dwHandle;

    dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
    if (dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
        return FALSE;

    if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
    {
        VS_FIXEDFILEINFO * pInfo;
        unsigned int nInfoLen;

        if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
        {
            _sntprintf_s(pszVersion, nVersionLen, _TRUNCATE, _T("%d.%d.%d.%d"),
                HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),
                HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
            pszVersion[nVersionLen - 1] = _T('\0');
            return TRUE;
        }
    }

    return FALSE;
}

BOOL Util::File::GetFileVersion(LPCTSTR strFile, VS_FIXEDFILEINFO** pInfo)
{
    if (!strFile || !strFile[0])
        return FALSE;

    TCHAR  szVersionBuffer[8192] = {};
    DWORD dwVerSize;
    DWORD dwHandle;

    dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
    if (dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
        return FALSE;

    if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))
    {
        unsigned int nInfoLen;
        if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)pInfo, &nInfoLen))
            return TRUE;
    }

    return FALSE;
}

tstring Util::File::GetFileVersion(LPCTSTR strFile)
{
    if (!strFile || !strFile[0])
        return _T("");
#ifndef SIZEOF
#define SIZEOF(A) (sizeof(A)/sizeof((A)[0]))
#endif
    TCHAR  szVersionBuffer[8192] = {};
    if (GetFileVersion(strFile, szVersionBuffer, SIZEOF(szVersionBuffer)))
    {
        return szVersionBuffer;
    }
    return _T("");
}

tstring Util::File::GetFileVersion(HMODULE hModule)
{
    if (!hModule) return _T("");

    TCHAR szFilePath[MAX_PATH] = {};
    GetModuleFileName(hModule, szFilePath, MAX_PATH);
    return GetFileVersion(szFilePath);
}


