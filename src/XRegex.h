//----------------------------------------------//
//  XRegex.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/1.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XRegex_h
#define XRegex_h
#include "XBase.h"
#include <vector>
#include <string>

XLIB_BEGAIN

namespace XRegex
{
    std::vector<std::string> getMatch(const std::string& src,const std::string& pattern);
    std::string replace(const std::string& src,const std::string& pattern,const std::string& replace_str);
};

XLIB_END

#endif /* XRegex_h */
