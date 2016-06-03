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

XLIB_BEGAIN

using namespace std;
XFileUtil* XFileUtil::instance_ = nullptr;


XFileUtil::XFileUtil()
{
    pathType_=X_PLATFORM==X_P_WIN32?PATH_TYPE::WIN:PATH_TYPE::POSIX;
}

XFileUtil::~XFileUtil()
{
    
}

XFileUtil* XFileUtil::getInstance()
{
    if(nullptr==instance_) instance_ = new XFileUtil;
    return instance_;
}

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
    return false;
}

string XFileUtil::getFileNameWithOutPath(const string &fileName)
{
    string::size_type pos;
    if(pathType_==PATH_TYPE::POSIX)
    pos = fileName.find_last_of("/");
    else
    pos = fileName.find_last_of("/\\");
    if(pos!=string::npos)
        return fileName.substr(pos+1);
    return fileName;
}

string XFileUtil::getFileExt(const string &fileName)
{
    string ext = getFileNameWithOutPath(fileName);
    string::size_type pos = ext.find_first_of(".");
    if(pos!=string::npos)
    ext = ext.substr(pos+1);
    else ext="";
    return ext;
}

bool XFileUtil::createDirectory(const string &path)
{
    if(pathType_==PATH_TYPE::POSIX)
    {
        return 0;
    }
    else
    {
        return 0;
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