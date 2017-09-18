//----------------------------------------------//
//  XFileUtil.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/3.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XFileUtil_h
#define XFileUtil_h
#include <string>
#include <vector>
#include <map>
#include "XBase.h"


XLIB_BEGAIN

static const int64 MAX_READ_STRING_SIZE = 1024*16;
static const int64 MAX_LINE_STRING_SIZE = 1024*2;
namespace XFileUtil
{
    bool isFileExist(const std::string& fileName);
    /**
     get current path
     */
    std::string getCurrentPath();
    std::string getCurrentPathWithPrefix();
    std::string getParentPath(const std::string& path);
    //now temporarily invalid
    DEPRECATED_ATTRIBUTE bool isPathExist(const std::string& path);
    std::string getFileNameWithOutPath(const std::string& fileName);
    std::string getFileExt(const std::string& fileName);
    //now temporarily invalid MAC is OK
    DEPRECATED_ATTRIBUTE bool createDirectory(const std::string& path);
    /**
        write
     */
    void writeBufferToNewBinFile(const std::vector<char>* buffer,const std::string& fileFullName);
    void writeBufferToExistBinFile(const std::vector<char>* buffer,const std::string& fileFullName);
    void writeTxtLineToFile(const std::string& line,const std::string& fileFullName);
    void writeTxtLineToNewFile(const std::string& line,const std::string& fileFullName);
    void writeTxtLineToExistFile(const std::string& line,const std::string& fileFullName);
    
    /**
        read
     */
    std::string readStringFromFile(const std::string& fileFullName);
    std::vector<std::string> readStringByLine(const std::string& fileFullName);
    
    /**
        copy
     */
    bool copyFile(const std::string& from,const std::string& to);
};


XLIB_END

#endif /* XFileUtil_h */
