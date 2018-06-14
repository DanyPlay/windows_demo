#ifndef __UI_H__
#define __UI_H__

#pragma once

#ifdef DEBUG
#pragma comment(lib, "UI/Lib/DuiLib_d.lib")
#else
#pragma comment(lib, "UI/Lib/DuiLib.lib")
#endif


#include "Inlcude/UIlib.h"

#include "Core/Control/UIWindow.h"

#include "Core/WindowImpl.h"
#include "Core/DialogImpl.h"
#include "Core/ChildWindowImpl.h"

#include "Core/MessageBox.h"

#endif
