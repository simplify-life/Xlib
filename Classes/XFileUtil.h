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

enum class PATH_TYPE
{
    WIN = 0,
    POSIX = 1
};
static const int64 MAX_READ_STRING_SIZE = 1024*16;
static const int64 MAX_LINE_STRING_SIZE = 1024*2;
class XFileUtil
{
public:
    //constructors
    XFileUtil();
    ~XFileUtil();
    static XFileUtil* getInstance();
public:
    bool isFileExist(const std::string& fileName);
    /**
     get current path
     */
    std::string getCurrentPath();
    std::string getParentPath(const std::string& path);
    //now temporarily invalid
    DEPRECATED_ATTRIBUTE bool isPathExist(const std::string& path);
    std::string getFileNameWithOutPath(const std::string& fileName);
    std::string getFileExt(const std::string& fileName);
public:
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
private:
    static XFileUtil* instance_;
    PATH_TYPE pathType_;
};


XLIB_END

#endif /* XFileUtil_h */
