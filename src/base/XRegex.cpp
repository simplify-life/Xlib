//----------------------------------------------//
//  XRegex.cpp
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/1.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "base/XRegex.h"
#include <regex>
#include <string>

XLIB_BEGAIN


using namespace std;


vector<XRegex::MatchInfo> XRegex::getMatch(const string &src, const string &pattern, bool onlyFirst)
{
    vector<XRegex::MatchInfo> result;
    regex re(pattern);
    smatch sm;
    auto iter = src.cbegin();
    long start = 0;
    while(regex_search(iter,src.cend(), sm, re)) {
        std::string str = sm.str();
//        copy(sm.begin(), sm.end(), std::back_inserter(result));
        XRegex::MatchInfo info;
        info.str = str;
        info.pos = sm.position()+start;
        result.push_back(info);
        start += sm.position() + str.length();
        iter = sm[0].second;
        if(onlyFirst) break;
        
    }
    return result;
}

string XRegex::replace(const string &src, const string &pattern, const string &replace_str, int idx)
{
    
    if(0==idx){
        regex re(pattern);
        return regex_replace(src,re,replace_str);
    }
    vector<XRegex::MatchInfo> result = getMatch(src, pattern,false);
    size_t total = result.size();
    if(total==0){
        return src;
    }
    if(idx<0){
        idx = total + idx;
    }else{
        idx -=1;
    }
    if(idx>=total){
        return src;
    }
    auto info = result.at(idx);
    std::string r = std::string(src);
    return r.replace(info.pos,info.str.length(),replace_str);
}

XLIB_END
