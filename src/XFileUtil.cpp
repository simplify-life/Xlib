//----------------------------------------------//
//  XFileUtil.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/3.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "XFileUtil.h"
#include "xplatform.h"
#include <fstream>
#include <iostream>
#if(X_PLATFORM==X_P_WIN32)
#include<direct.h>
#define getCWD(buf__,size__) _getcwd(buf__,size__)
#define mkDIR(dir) _mkdir(dir)
#define pathPrefix "/\\"
#else
#include <unistd.h>
#include <sys/stat.h>
#define getCWD(buf__,size__) getcwd(buf__,size__)
#define mkDIR(dir) mkdir((dir),0755)
#define pathPrefix "/"
#endif


XLIB_BEGAIN

using namespace std;

bool XFileUtil::isFileExist(const string &fileName)
{
    fstream file(fileName.c_str(),ios::in);
    if(file.is_open())
    {
        file.close();
        return true;
    }
    return false;
}

bool XFileUtil::isPathExist(const string &path)
{
    return isFileExist(path);
}


string XFileUtil::getCurrentPath()
{
    char buf[1024*4];
    getCWD(buf,1024*4);
    return string(buf);
}

string XFileUtil::getCurrentPathWithPrefix()
{
    string path = getCurrentPath();
    return path.append(pathPrefix);
}

string XFileUtil::getFileNameWithOutPath(const string &fileName)
{
    string::size_type pos;
    pos = fileName.find_last_of(pathPrefix);
    if(pos!=string::npos)
        return fileName.substr(pos+1);
    return fileName;
}

string XFileUtil::getFileExt(const string &fileName)
{
    string ext = getFileNameWithOutPath(fileName);
    string::size_type pos = ext.find_last_of(".");
    if(pos!=string::npos)
    ext = ext.substr(pos+1);
    else ext="";
    return ext;
}

string XFileUtil::getParentPath(const string &path)
{
    if(!path.empty())
    {
        auto end = path.find_last_of(pathPrefix);
        if(end==string::npos) return "";
        auto tmp = path.substr(0,end);
        if(end==path.length()-1)
        {
            tmp = tmp.substr(0,end-1);
            end = tmp.find_last_of(pathPrefix);
            tmp = tmp.substr(0,end);
        }
        return tmp;
    }
    return "";
}

bool XFileUtil::createDirectory(const string &path)
{
    auto tmp = getParentPath(path);
    if(isPathExist(tmp))
    {
        if(mkDIR(path.c_str())==0)
        return true;
        return false;
    }
    else
    {
        if(createDirectory(tmp)) return createDirectory(path);
        else return false;
    }
}

void XFileUtil::writeBufferToNewBinFile(const vector<char> *buffer, const string &fileFullName)
{
    fstream fs(fileFullName,ios::out|ios::binary|ios::trunc);
    if(fs.is_open())
    {
        string tmp = string(buffer->begin(), buffer->end());
        fs.write(tmp.c_str(), tmp.size());
    }
    fs.close();
}

void XFileUtil::writeBufferToExistBinFile(const vector<char> *buffer, const string &fileFullName)
{
    fstream fs(fileFullName,ios::out|ios::binary|ios::app);
    if(fs.is_open())
    {
        string tmp = string(buffer->begin(), buffer->end());
        fs.write(tmp.c_str(), tmp.size());
    }
    fs.close();
}


void XFileUtil::writeTxtLineToFile(const string &line, const string &fileFullName)
{
    ofstream outFile(fileFullName,ios::out|ios::app);
    if(outFile.is_open())
    {
        outFile<<line<<endl;
        outFile.close();
    }
    else
        writeTxtLineToNewFile(line, fileFullName);
}

void XFileUtil::writeTxtLineToNewFile(const string &line, const string &fileFullName)
{
    ofstream outFile(fileFullName,ios::out|ios::trunc);
    if(outFile.is_open())
    {
        outFile<<line<<endl;
        outFile.close();
    }
}

void XFileUtil::writeTxtLineToExistFile(const string &line, const string &fileFullName)
{
    ofstream outFile(fileFullName,ios::out|ios::app);
    if(outFile.is_open())
    {
        outFile<<line<<endl;
        outFile.close();
    }
}

string XFileUtil::readStringFromFile(const string &fileFullName)
{
    ifstream inFile(fileFullName,ios::in);
    if(inFile.is_open())
    {
        char tmp[MAX_READ_STRING_SIZE];
        while (!inFile.eof())
        {
            inFile>>tmp;
        }
        inFile.close();
        return string(tmp);
    }
    return "";
}

vector<string> XFileUtil::readStringByLine(const string &fileFullName)
{
    vector<string> result;
    ifstream inFile(fileFullName,ios::in);
    if(inFile.is_open())
    {
        string tmp = "";
        while (!inFile.eof())
        {
            while(getline(inFile,tmp))
            {
                result.push_back(tmp);
                tmp = "";
            }
        }
        inFile.close();
    }
    return result;
}


XLIB_END
