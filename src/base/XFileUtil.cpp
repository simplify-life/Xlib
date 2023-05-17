//----------------------------------------------//
//  XFileUtil.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/3.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "base/XFileUtil.h"
#include "macro/xplatform.h"
#include "crypto/md5.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#if(X_PLATFORM==X_P_WIN32)
#include<direct.h>
#define getCWD(buf__,size__) _getcwd(buf__,size__)
#define mkDIR(dir) _mkdir(dir)
#define pathPrefix "/\\"
#else
#include <unistd.h>
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

std::string XFileUtil::getFileNameWithOutPathAndExt(const std::string& fileName)
{
    string::size_type pos_pre;
    pos_pre = fileName.find_last_of(pathPrefix);
    string fName = getFileNameWithOutPath(fileName);
    string::size_type pos = fName.find_last_of(".");
    if(pos!=string::npos)
    fName = fileName.substr(pos_pre+1,pos);
    else fName="";
    return fName;
}

int64 XFileUtil::getLastModifiTime(const std::string& fileName)
{
    struct stat statBuf;
    ifstream file(fileName.c_str(),ios::in);
    if(file.is_open())
    {
        stat(fileName.c_str(), &statBuf);
        file.close();
#if(X_PLATFORM!=X_P_MAC)
        return statBuf.st_mtime;
#else
        return statBuf.st_mtimespec.tv_sec;
#endif
    }
    return 0;
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
    string result = "";
    if(inFile.is_open())
    {
        while (!inFile.eof())
        {
            char tmp[MAX_READ_STRING_SIZE];
            inFile>>tmp;
            result.append(string(tmp));
        }
        inFile.close();
    }
    return result;
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

bool XFileUtil::copyFile(const std::string &from, const std::string& to)
{
    bool res = true;
    std::ifstream in(from.c_str(), std::ios::binary);
    std::ofstream out(to.c_str(), std::ios::binary);
    if(in.is_open()&&out.is_open()){
        out << in.rdbuf();
    }else{
        res  = false;
    }
    out.close();
    in.close();
    return true;
}

uint64 XFileUtil::getFileBytesLength(const std::string &file){
    std::ifstream in(file.c_str(), std::ios::binary);
    uint64 len = 0;
    if(in.is_open()){
        in.seekg(0,std::ios::end);
        len = in.tellg();
        in.close();
    }
    return len;
}

bool XFileUtil::encryptoFile(const std::string &from, const std::string &to, const std::string &key){
    uint64 lenFrom = getFileBytesLength(from);
    uint64 start = 0;
    char read_ch = 0;
    char write_ch = 0;
    bool res = true;
    const byte* keys = crypto::md5Digest(key);
    byte idx = 0;
    std::ifstream in(from.c_str(), std::ios::binary);
    std::ofstream out(to.c_str(), std::ios::binary);
    if (in.is_open())
    {
        while (true) {
            if(lenFrom==start)
                break;
            if (in.eof())
                break;
            in.read(&read_ch, 1);
            write_ch = ~read_ch^(keys[idx&0xF]);
            idx=(idx&0xF)+1;
            out.write(&write_ch, 1);
            start++;
            if (std::ios_base::goodbit != out.rdstate()) {
                out.close();
                in.close();
                return false;
            }
        }
    }else{
        res = false;
    }
    out.close();
    in.close();
    return true;
}

bool XFileUtil::decryptoFile(const std::string &from, const std::string &to, const std::string &key){
    return encryptoFile(from, to, key);
}

bool XFileUtil::allSameFile(const std::string &from, const std::string &to){
    uint64 lenFrom = getFileBytesLength(from);
    uint64 lenTo = getFileBytesLength(to);
    if(lenTo != lenFrom) return false;
    char chFrom = 0;
    char chTo = 0;
    bool res = true;
    std::ifstream inFrom(from.c_str(), std::ios::binary);
    std::ifstream inTo(to.c_str(), std::ios::binary);
    if(inFrom.is_open()&&inTo.is_open()){
        while (true)
        {
            if (inFrom.eof()||inTo.eof()){
                break;
            }
            inFrom.read(&chFrom, 1);
            inTo.read(&chTo, 1);
            if(chFrom!=chTo){
                inFrom.close();
                inTo.close();
                return false;
            }
        }
    }else{
        res = false;
    }
    inFrom.close();
    inTo.close();
    return res;
}

string XFileUtil::md5(const std::string file){
    std::ifstream ifs(file);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>() ));
    return crypto::MD5(content);
}

XLIB_END
