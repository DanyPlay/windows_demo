#ifndef _CRASH_H_20180509_
#define _CRASH_H_20180509_

#pragma once

class CCrash
{
private:
    CCrash() {};
    ~CCrash() {};

public:
    static void Init();

public:
    static inline void WriteDump(PEXCEPTION_POINTERS exception);
private:
    static std::string GetFilePath();
};

#endif
