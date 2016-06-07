//----------------------------------------------//
//  XUtf8.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/7.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XUtf8_h
#define XUtf8_h
#include <string>
#include <unordered_map>
#include "XBase.h"

XLIB_BEGAIN

/***************************************************/
// Class XUtf8 handler std::string which is unicode
/***************************************************/
class XUtf8
{
public:
    static XUtf8* getInstance();
public:
    unsigned int getlen(const char*);
    unsigned int getlen(const std::string&);
    /**
     get the idx words in utf8 string,not char!
     */
    std::string getWord(unsigned int idx,const std::string& src);
    
    /**
     get unicode message ,key is (words) idx,value is (words) length.
     */
    std::unordered_map<unsigned int,unsigned int> getMsg(const std::string&);
private:
    static XUtf8* mUtf8_;
};

XLIB_END

#endif /* XUtf8_h */
