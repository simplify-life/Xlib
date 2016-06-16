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
//    XUtf8 handler std::string which is utf8 code
/***************************************************/
namespace XUtf8
{
    /**
     * 	code the utf8 string  to unicode .
     * 	for eg. utf8ToUnicode("这是一个字符串") = "\u8fd9\u662f\u4e00\u4e2a\u5b57\u7b26\u4e32";
    */
    std::string utf8ToUnicode(const std::string& src);

    unsigned int getlen(const char*);
    /**
     get utf8 string's words count
     eg. "a" len is 1,"a中" len is 4 in MacOS platform
     */
    unsigned int getlen(const std::string&);
    /**
     get the words in utf8 string,not char!
     utf8 word's posIdx,start from 0,such as:in MacOS platform in "I'm中国人",'I' pos is 0,'中' is 3,'国' is 4
     */
    std::string getWord(unsigned int posIdx,const std::string& src);
    
    /**
     @brief. get unicode message ,key is (word's) idx,value is (word's) length.
     @eg. getMsg("I'm中国人") = {(0,1),(1,1),(2,1),(3,3),(4,3),(5,3)}
     */
    std::unordered_map<unsigned int,unsigned int> getMsg(const std::string&);
    /**
     you can use this just lick std::string->substr();
     @param startPos     utf8 word's pos,such as:in MacOS platform in "I'm中国人",'I' pos is 0,'中' is 3,'国' is 4
     @param len  eg. in MacOS platform  auto str = substr("I'm中国人",4,2) ,the str is "国人"
     */
    std::string substr(const std::string& src,unsigned int startPos,unsigned int len);
};

XLIB_END

#endif /* XUtf8_h */
