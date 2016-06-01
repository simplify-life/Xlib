//----------------------------------------------//
//  XRegex.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/1.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "XRegex.h"
#include <regex>

XLIB_BEGAIN

using namespace std;

vector<string> XRegex::getMatch(const string &src, const string &pattern)
{
    vector<string> result;
    const sregex_token_iterator end;
    for (sregex_token_iterator i(src.begin(),src.end(),regex(pattern.c_str())); i!=end;i++)
    {
        result.emplace_back(*i);
    }
    return result;
}

string XRegex::replace(const string &src, const string &pattern, const string &replace_str)
{
    return regex_replace(src,regex(pattern),replace_str);
}

XLIB_END