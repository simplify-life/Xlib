//----------------------------------------------//
//  XString.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XString_h
#define XString_h
#include "XBase.h"
#include <string>
#include <vector>
#include <ctime>
XLIB_BEGAIN
enum class TIME_F : unsigned char
{
    H12_M_S,    //12 hour:minute:seconds
    H24_M_S,    //24 hour:minute:seconds
    Y_M_D,      //year-month-day
    W_D,        //weekday
    LOG_TIME,
    T_DEFAULT,  //default format
};
class XString
{
public:
    static std::string format(const char*,...);
    static std::string formatTime(const std::tm*,const TIME_F&);
    static std::string toUpper(const std::string&);
    static std::string toLower(const std::string&);
    /**
     @eg.   trim(",,","Hello,,my name,,is")="Hellomy nameis"
     */
    static std::string trim(const std::string&,const std::string&);
    /**
     @eg.   split(",,","this,,is,,my,,name")=vector<std::string>{"this","is","my","name"}
     */
    static std::vector<std::string> split(const std::string&,const std::string&);
    /**
     @eg.   replace("this,,is,,my,,name,,",",,","||")="this||is||my||name||"
     */
    static std::string replace(const std::string& src,const std::string& sfind,const std::string& sreplace);
    
    static int count(const std::string&,const std::string&);
    static bool isInt(const std::string&,bool ignoreSymbols=true);
    static int compareVersion(const std::string&,const std::string&,const std::string& key=".");

};
XLIB_END
#endif /* XString_h */
