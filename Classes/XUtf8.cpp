//----------------------------------------------//
//  XUtf8.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/7.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "XUtf8.h"
//#include <cassert>
XLIB_BEGAIN

using namespace std;
XUtf8* XUtf8::mUtf8_ = nullptr;


XUtf8* XUtf8::getInstance()
{
    if(!mUtf8_) mUtf8_ = new XUtf8;
    return mUtf8_;
}


unsigned int XUtf8::getlen(const char* unicode)
{
    unsigned int c=0,len = 0;
    while(unicode[c])
    {
        if((unicode[c]&0xc0)!=0x80) len++;
        c++;
    }
        return len;
}

unsigned int XUtf8::getlen(const string & s_utf8)
{
    return getlen(s_utf8.c_str());
}


string XUtf8::getWord(unsigned int idx, const string &src)
{
    const char* src_ = src.c_str();
    unsigned int c=0,len = 0 ;
    string::size_type start=string::npos,end = string::npos;
    while (src_[c])
    {
        if((src_[c]&0xc0)!=0x80)
        {
            if(idx==len&&start==string::npos)
            {
                start = c;
            }
            len++;
        }
        c++;
        if(idx+2==len&&end==string::npos)
        {
            end = c-1;
            break;
        }
    }
    if((start==end)&&start==string::npos) return ""; //out of range
    return src.substr(start,end-start);
}

unordered_map<unsigned int,unsigned int> XUtf8::getMsg(const string &src)
{
    unordered_map<unsigned int,unsigned int> map_msg;
    const char* src_ = src.c_str();
    unsigned int c=0,len =0;
    while (src_[c])
    {
        if((src_[c]&0xc0)!=0x80)
        {
            map_msg.emplace(len,0);
            len++;
        }
        map_msg.find(len-1)->second++;
        c++;
    }
    return map_msg;
}

string XUtf8::substr(const string &src, unsigned int startPos, unsigned int len)
{
    auto msg = getMsg(src);
    if(startPos+len<=msg.size())
    {
        string::size_type start=0,end = 0;
        for(auto e:msg)
        {
            if(e.first<startPos) start+=e.second;
            else if(e.first<startPos+len)
            {
                end+=e.second;
            }
        }
        return src.substr(start,end);
    }
    //assert(0&&"out of range");
    return ""; //out of range
}

XLIB_END