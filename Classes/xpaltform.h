//----------------------------------------------//
//  xpaltform.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/27.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef xpaltform_h
#define xpaltform_h

// define supported target platform .
#define X_P_UNKNOWN            0
#define X_P_IOS                1
#define X_P_ANDROID            2
#define X_P_WIN32              3
#define X_P_MARMALADE          4
#define X_P_LINUX              5
#define X_P_BADA               6
#define X_P_BLACKBERRY         7
#define X_P_MAC                8
#define X_P_NACL               9
#define X_P_EMSCRIPTEN        10
#define X_P_TIZEN             11
#define X_P_QT5               12
#define X_P_WINRT             13

// Determine current platform by compile environment macro.
#define X_PLATFORM             X_P_UNKNOWN

// mac
#if defined(__APPLE__)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_MAC
#endif

// iphone
#if defined(CC_TARGET_OS_IPHONE)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_IOS
#endif

// android
#if defined(ANDROID)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_ANDROID
#endif

// win32
#if defined(_WIN32) || defined(_WINDOWS)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  X_PLATFORM
#define X_PLATFORM         X_P_BADA
#endif

// qnx
#if defined(__QNX__)
#undef  X_PLATFORM
#define X_PLATFORM     X_P_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#undef  X_PLATFORM
#define X_PLATFORM     X_P_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
#undef  X_PLATFORM
#define X_PLATFORM     X_P_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
#undef  X_PLATFORM
#define X_PLATFORM     X_P_TIZEN
#endif

// qt5
#if defined(CC_TARGET_QT5)
#undef  X_PLATFORM
#define X_PLATFORM     X_P_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#undef  X_PLATFORM
#define X_PLATFORM          X_P_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! X_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

/** 判断平台是否为小端字节序 */
inline bool isPlatformLittleEndian()
{
    int n = 1;
    return *((char*)&n) ? true : false;
}

#endif /* xpaltform_h */
