//----------------------------------------------//
//  XBase.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/30.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XBase_h
#define XBase_h

#include <stdint.h>

//define cross-platform base data
using uint=unsigned int;

#ifdef _MSC_VER
using int8=__int8;
using int16=__int16;
using int32=__int32;
using int64=__int64;

using uint8=unsigned __int8;
using uint16=unsigned __int16;
using uint32=unsigned __int32;
using uint64=unsigned __int64;
#else

using int8=int8_t;
using int16=int16_t;
using int32=int32_t;
using int64=int64_t;

using uint8=uint8_t;
using uint16=uint16_t;
using uint32=uint32_t;
using uint64=uint64_t;
#endif

//about array

#define arraysize(array) (int)(sizeof(array)/sizeof((array)[0]))
template<class T,int N>
static int getValueIndex(T value,const T (&arr)[N])
{
    for (int i=0; i<arraysize(arr); i++)
    {
        if(value==arr[i]) return i;
    }
    return -1;
}
//template<>
//defined flags
#define DEFINE_flag(type, name, deflt, desc) \
namespace xlib { type FLAGS_##name = deflt; }

#define DECLARE_flag(type, name) \
namespace xlib { extern type FLAGS_##name; }

#define DEFINE_bool(name, deflt, desc) DEFINE_flag(bool, name, deflt, desc)
#define DEFINE_int32(name, deflt, desc) DEFINE_flag(int32, name, deflt, desc)
#define DEFINE_string(name, deflt, desc) DEFINE_flag(string, name, deflt, desc)

#define DECLARE_bool(name) DECLARE_flag(bool, name)
#define DECLARE_int32(name) DECLARE_flag(int32, name)
#define DECLARE_string(name) DECLARE_flag(string, name)


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&)=delete;                 \
TypeName& operator=(const TypeName&)=delete;


//define namespace

#define XLIB_BEGAIN namespace xlib{
#define XLIB_END }
#define US_NS_X using namespace xlib



#endif /* XBase_h */
