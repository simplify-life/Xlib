//----------------------------------------------//
//  XString.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "XString.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

string XString::format(const char * fmt, ...)
{
#define X_MAX_STRING_LENGTH (1024*100)
    
    string ret;
    	
    va_list ap;
    va_start(ap, fmt);
    
    char* buf = (char*)malloc(X_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, X_MAX_STRING_LENGTH, fmt, ap);
        ret = buf;
        free(buf);
    }
    va_end(ap);
    return ret;
}

string XString::toUpper(const string & str)
{
    string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), [](int c) ->int{return toupper(c);});
    return ret;
}

string XString::toLower(const string &str)
{
    string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), [](int c) ->int{return tolower(c);});
    return ret;
}

string XString::trim(const char key,const string& str)
{
    string ret = str;
    ret.erase(remove(ret.begin(),ret.end(),key),ret.end());
    return ret;
}

vector<string> XString::split(const std::string& key, const std::string &str)
{
    vector<string> ret;
    string::size_type start=0,end=0,increment=key.size();
    if(0==increment) return ret;
    string temp = str;
    temp.append(key);//append to get tail data
    while ((end=temp.find(key,start))!=string::npos)
    {
        string s = temp.substr(start,temp.find(key,start)-start);
        if(!s.empty()) /*Remove empty data*/ ret.emplace_back(s);
        start = end+increment;
    }
    return ret;
}

int XString::count(const std::string & key, const std::string &str)
{
   
    if(key.empty()||str.empty()) return 0;
    int t=0;
    string::size_type start=0,end=0,increment=key.size();
    while ((end=str.find(key,start))!=string::npos)
    {
        start = end+increment;
        t++;
    }
    return t;
}

bool XString::isInt(const std::string &str,bool ignoreSymbols)
{
    if(!str.empty())
    {
        if(ignoreSymbols)
        {
            for(auto &i:str)
            {
                if(i<'0'||i>'9') return false;
            }
            return true;
        }
        else
        {
            auto first = str.begin();
            if((*first!='-')&&(*first < '0' || *first > '9')) return false;
            for (auto i = str.begin()+1; i != str.end(); i++)
            {
                if (*i < '0' || *i > '9') return false;
            }
            return true;
        }

    }
    return false;
}