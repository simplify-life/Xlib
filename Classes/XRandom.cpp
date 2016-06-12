//----------------------------------------------//
//  XRandom.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/12.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "XRandom.h"
#include "XTime.h"
#include<random>

XLIB_BEGAIN

using namespace std;

vector<uint> getRandomV(uint size)
{
    vector<uint> v;
    uint cursor = 0;
    while (cursor<size)
    {
        v.emplace_back(cursor++);
    }
    shuffle(v.begin(),v.end(),default_random_engine((uint)XTime::getTimestamp_milliseconds()));
    return v;
}

std::string getRandomString(uint length)
{
    static const char * const chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 62 chars
    static const uint size = 62; // = strlen(chars);
    uniform_int_distribution<uint> distribution(0, size - 1);
    auto engine = default_random_engine((uint)XTime::getTimestamp_milliseconds());
    string s(length, '\0');
    for (uint i = 0; i < length; ++i)
    {
        s[i] = chars[distribution(engine)];
    }
    return s;
}


XLIB_END