//----------------------------------------------//
//  XRegex.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/1.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "base/XRegex.h"
#ifdef __clang__
#include <regex>
#else
    #if defined(__GNUC__) && defined(__GNUC_MINOR__ )
        #if(__GNUC__>4)||(__GNUC__==4)&&(__GNUC_MINOR__>=9)
            #include <regex>
            #else
            //#include <tr1::regex>
            //using namespace std::tr1;
            #define NO_USE_REGEX
        #endif
    #endif
    #ifdef _MSC_VER
        #if _MSC_VER>=1800
            #include <regex>
            #else
            #include <tr1::regex>
            using namespace std::tr1;
        #endif
    #endif
#endif
XLIB_BEGAIN

/**
 <regex> is not supported by some stl version,so what a pity，Only temporarily canceled
 */
#ifndef NO_USE_REGEX
#define NO_USE_REGEX
#endif

using namespace std;

vector<string> XRegex::getMatch(const string &src, const string &pattern)
{
    vector<string> result;
#ifndef NO_USE_REGEX
    const sregex_token_iterator end;
    for (sregex_token_iterator i(src.begin(),src.end(),regex(pattern.c_str())); i!=end;i++)
    {
        result.emplace_back(*i);
    }
#endif
    return result;
}

string XRegex::replace(const string &src, const string &pattern, const string &replace_str)
{
#ifndef NO_USE_REGEX
    return regex_replace(src,regex(pattern),replace_str);
#endif
    return "";
}

XLIB_END
