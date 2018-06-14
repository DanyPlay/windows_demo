
#pragma once
#ifndef TSTRING_H
#define TSTRING_H

#ifdef UNICODE
#define tstring  std::wstring
#else
#define tstring  std::string
#endif // !UNICODE


#endif  //TSTRING_H

