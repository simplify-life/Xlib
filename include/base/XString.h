//----------------------------------------------//
//  XString.h
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XString_h
#define XString_h
#include "macro/XBase.h"
#include "XTime.h"
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

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
namespace XString
{
    std::string format(const char*,...);
    std::string formatTime(const std::tm*,const TIME_F&);
    std::string formatTime(const XTime::tm_million*,const TIME_F&);
    std::string toUpper(const std::string&);
    std::string toLower(const std::string&);
    /**
     @eg.   trim("Hello,,my name,,is",",,")="Hellomy nameis"
     */
    std::string trim(const std::string&,const std::string&);
    /**
     @eg.   split("this,,is,,my,,name",",,")=vector<std::string>{"this","is","my","name"}
     */
    std::vector<std::string> split(const std::string&,const std::string&);
    /**
     @eg.   replace("this,,is,,my,,name,,",",,","||")="this||is||my||name||"
     */
    std::string replace(const std::string& src,const std::string& sfind,const std::string& sreplace);

    std::size_t findNthOccurrence(const std::string& str, char c, int n) ;
    
    int count(const std::string&,const std::string&);
    bool isInt(const std::string&,bool ignoreSymbols=true);
    int compareVersion(const std::string&,const std::string&,const std::string& key=".");
    
    bool isCharInString(const char&,const std::string&);
    
    std::vector<std::string> lettersShape(const std::string &letters);

    std::size_t findFirstCharNotEscapedBefore(const std::string& str, char ch);

    template<class T_out,class T_in>
    T_out convert(const T_in& in)
    {
        std::stringstream stream;
        stream<<in;
        T_out out;
        stream>>out;
        stream.clear();
        return out;
    }

    template<typename T>
    std::string toString(const std::vector<std::vector<T>>& matrix){
        std::stringstream ss;
        for (const auto& row : matrix) {
            for (const auto& elem : row) {
                ss << elem << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }
    
    template<typename T>
    std::string toString(const std::vector<T>& v){
        std::stringstream ss;
        for (const auto& elem : v) {
            ss << elem << " ";
        }
        return ss.str();
    }

    template<typename T>
    std::string toStringAddEnter(const std::vector<T>& v){
        std::stringstream ss;
        for (const auto& elem : v) {
            ss << elem << "\n";
        }
        return ss.str();
    }
};
XLIB_END
#endif /* XString_h */
