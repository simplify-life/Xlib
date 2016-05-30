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
#include <string>
#include <vector>
class XString
{
public:
    static std::string format(const char*,...);
    static std::string toUpper(const std::string&);
    static std::string toLower(const std::string&);
    static std::string trim(const char,const std::string&);
    static std::vector<std::string> split(const std::string&,const std::string&);
    static int count(const std::string&,const std::string&);
    static bool isInt(const std::string&,bool ignoreSymbols=true);
    
};

#endif /* XString_h */
