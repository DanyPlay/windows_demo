#ifndef _INTERACT_UI_CONSTANT_H_
#define _INTERACT_UI_CONSTANT_H_

#pragma once

#include <windows.h>

enum
{
    WM_PAGE_SELECT_CHANGED = WM_USER + 1,
    WM_CREATE_ROOM_SUCCESS,
    WM_BEGIN_JOIN_ROOM,

    WM_TOOLBAR_SHOW = (WM_USER + 0x200),
    WM_TOOLBAR_KICKOUT_GUEST,
    WM_TOOLBAR_MUTE_VIDEO,
    WM_TOOLBAR_MUTE_AUDIO,
    WM_TOOLBAR_MUTE_MICRO,

    WM_GUEST_ASK_JOIN,
};

enum MAIN_TAB
{
    MAIN_TAB_UNKOWN = -1,
    MAIN_TAB_HOME = 0,
    MAIN_TAB_ROOM,
    MAIN_TAB_CREATE_ROOM,
    MAIN_TAB_DEVICE,
    MAIN_TAB_SETTING,
    MAIN_TAB_MAX = MAIN_TAB_SETTING,
};

static LPCTSTR ROOM_MODES[] =
{
    _T("Ö÷²¥&Ö÷²¥"),
    _T("Ö÷²¥&¼Î±ö"),
    _T("ºäÅ¿")
};

static LPCTSTR USER_ANCHOR_NAME = _T("Ö÷²¥");
static LPCTSTR USER_GUEST_NAME = _T("¼Î±ö");
static LPCTSTR USER_AUDIENCE_NAME = _T("¹ÛÖÚ");

#endif
