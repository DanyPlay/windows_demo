#include "stdafx.h"
#include "Crash.h"
#include <time.h>
#include <dbghelp.h>

typedef BOOL(WINAPI *PMiniDumpWriteDump)(HANDLE hProcess, DWORD ProcessId,
    HANDLE hFile, MINIDUMP_TYPE DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

static LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS exception)
{
    LOGE(TAG_COMMON, _T("%s catch a crash"), __FUNCTIONW__);

    /* don't use if a debugger is present */
    if (IsDebuggerPresent())
        return EXCEPTION_CONTINUE_SEARCH;

    static bool inside_handler = false;

    if (inside_handler)
        return EXCEPTION_CONTINUE_SEARCH;

    inside_handler = true;

    MessageBox(::GetActiveWindow(), _T("±§Ç¸£¬360ÊÓÆµÔÆ±ÀÀ£ÁË£¡\t\t\t"), APP_TITLE, MB_OK);

    CCrash::WriteDump(exception);

    inside_handler = false;

    return EXCEPTION_EXECUTE_HANDLER;
}

void CCrash::Init()
{
    static bool bInit = false;

    if (!bInit)
    {
        SetUnhandledExceptionFilter(ExceptionHandler);
        bInit = true;
    }
}

void CCrash::WriteDump(PEXCEPTION_POINTERS exception)
{
    if (!exception)
        return;

    HMODULE hModule = NULL;
    do
    {
        hModule = LoadLibraryW(L"DbgHelp");
        if (!hModule)
        {
            break;
        }

        PMiniDumpWriteDump pMiniDumpWriteDump = (PMiniDumpWriteDump)GetProcAddress(hModule, "MiniDumpWriteDump");
        if (!pMiniDumpWriteDump)
        {
            break;
        }

        MINIDUMP_EXCEPTION_INFORMATION info;
        info.ThreadId = GetCurrentThreadId();
        info.ExceptionPointers = exception;
        info.ClientPointers = FALSE;

        HANDLE hFile = CreateFileA(GetFilePath().c_str(), GENERIC_READ | GENERIC_WRITE,
            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile && hFile != INVALID_HANDLE_VALUE)
        {
            pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                hFile, MiniDumpWithIndirectlyReferencedMemory, &info, NULL, NULL);
            CloseHandle(hFile);
        }
    } while (false);

    if (hModule)
    {
        FreeLibrary(hModule);
        hModule = NULL;
    }
}

std::string CCrash::GetFilePath()
{
    time_t now = time(0);
    struct tm *cur_time = localtime(&now);

    char szName[MAX_PATH] = { 0 };
    if (cur_time) {
        _snprintf_s(szName, _countof(szName), _TRUNCATE, "%d-%02d-%02d%c%02d-%02d-%02d.dmp",
            cur_time->tm_year + 1900,
            cur_time->tm_mon + 1,
            cur_time->tm_mday,
            '_',
            cur_time->tm_hour,
            cur_time->tm_min,
            cur_time->tm_sec);
    }
    else {
        _snprintf_s(szName, _countof(szName), _TRUNCATE, "%d-%02d-%02d%c%02d-%02d-%02d.dmp",
            1900,
            1,
            1,
            '_',
            0,
            0,
            0);
    }


    char szPath[MAX_PATH] = { 0 };
    ::SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, TRUE);
    ::PathAddBackslashA(szPath);
    ::PathAppendA(szPath, DIR_CRASH);
    ::PathAddBackslashA(szPath);
    ::PathAppendA(szPath, szName);

    Util::File::ForceCreateDir(STR_A2T(szPath).c_str());

    return szPath;
}
