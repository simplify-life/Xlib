//----------------------------------------------//
//  XBase.h
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/30.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XBase_h
#define XBase_h

#include <stdint.h>
#include <cstdint>  //using eg.std::int8_t


#if __cplusplus < 201103L
//#error "should use c++11 implementation"//delete this error,  most compiler has not full support for C++11 
#endif // __cplusplus


//namespace xlib {

    
    
//define cross-platform base data
using uint=unsigned int;
using byte=std::uint8_t;
#ifdef byte_max
#undef byte_max
#endif
#define byte_max 0xff
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




#define SAFE_DELETE(ptr) delete ptr;ptr=nullptr
#define SAFE_DELET_ARR(arr_ptr) delete[] arr_ptr;arr_ptr=nullptr

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

//define namespace

#define XLIB_BEGAIN namespace xlib{
#define XLIB_END }
#define US_NS_X using namespace xlib

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


/** @def DISALLOW_COPY_AND_ASSIGN(TypeName)
 * A macro to disallow the copy constructor and operator= functions.
 * This should be used in the private: declarations for a class
 */
#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
|| (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName &) = delete; \
TypeName &operator =(const TypeName &) = delete;
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName &); \
TypeName &operator =(const TypeName &);
#endif

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
#define DEPRECATED_ATTRIBUTE
#endif

/** @def DEPRECATED(...)
 * Macro to mark things deprecated as of a particular version
 * can be used with arbitrary parameters which are thrown away.
 * e.g. DEPRECATED(4.0) or DEPRECATED(4.0, "not going to need this anymore") etc.
 */
#define DEPRECATED(...) DEPRECATED_ATTRIBUTE

#define X_VECTOR(T,NAME,...) \
std::vector<T> NAME{__VAR_ARGS__}
    
/**
 get arguments number
 */
#define Y_TUPLE_SIZE_II(__args) Y_TUPLE_SIZE_I __args
#define Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0

#define Y_TUPLE_SIZE_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n

#define MPL_ARGS_SIZE(...) Y_TUPLE_SIZE_II((Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))


//} //end namespace xlib


#endif /* XBase_h */
