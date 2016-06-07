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
     get unicode message ,key is (word's) idx,value is (word's) length.
     */
    std::unordered_map<unsigned int,unsigned int> getMsg(const std::string&);
    /**
     you can use this just lick std::string->substr();
     @param startPos     utf8 word's pos,such as:in MacOS platform in "I'm中国人",'I' pos is 0,'中' is 3,'国' is 4
     @param len  eg. in MacOS platform  auto str = substr("I'm中国人",4,2) ,the str is "国人"
     */
    std::string substr(const std::string& src,unsigned int startPos,unsigned int len);
private:
    static XUtf8* mUtf8_;
};

XLIB_END

#endif /* XUtf8_h */
