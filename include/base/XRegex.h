//----------------------------------------------//
//  XRegex.h
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/1.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XRegex_h
#define XRegex_h
#include "macro/XBase.h"
#include <vector>
#include <string>

XLIB_BEGAIN

namespace XRegex
{
    struct MatchInfo{
        std::string str;
        long pos;
    };
    std::vector<MatchInfo> getMatch(const std::string& src,const std::string& pattern, bool onlyFirst=true);
    /**
     * @brief replace("hello123world456","\\d+","H",-1) => hello123worldH
     *        replace("hello123world456","\\d+","H",0) => helloHworldH
     *        replace("hello123world456","\\d+","H",1) => helloHworld456
     *        replace("hello123world456","\\d+","H",2) => hello123worldH
     * @param idx 0 reolace all match, 1 replace first match, -1 replace last match
     */
    std::string replace(const std::string& src,const std::string& pattern,const std::string& replace_str, int idx=0);
};

XLIB_END

#endif /* XRegex_h */
