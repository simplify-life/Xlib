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

XLIB_BEGAIN

using namespace std;
XUtf8* XUtf8::mUtf8_ = nullptr;


XUtf8* XUtf8::getInstance()
{
    if(!mUtf8_) mUtf8_ = new XUtf8;
    return mUtf8_;
}


int XUtf8::getlen(const char* unicode)
{
    int c=0,len = 0;
    while(unicode[c])
    {
        if((unicode[c]&0xc0)!=0x80) len++;
        c++;
    }
        return len;
}

int XUtf8::getlen(const string & s_utf8)
{
    return getlen(s_utf8.c_str());
}


string XUtf8::getWord(int idx, const string &src)
{
    const char* src_ = src.c_str();
    int c=0,len = 0 ;
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

unordered_map<int,int> XUtf8::getMsg(const string &src)
{
    unordered_map<int,int> map_msg;
    const char* src_ = src.c_str();
    int c=0,len =0;
    while (src_[c])
    {
        if((src_[c]&0xc0)!=0x80)
        {
            map_msg.insert(unordered_map<int, int>::value_type(len,0));
            len++;
        }
        map_msg.find(len-1)->second++;
        c++;
    }
    return map_msg;
}

XLIB_END