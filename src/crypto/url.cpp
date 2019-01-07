//
//  url.cpp
//  Xlib
//
//  Created by Ximena on 2018/12/28.
//  Copyright © 2018 Ximena. All rights reserved.
//

#include "crypto/url.h"
#include "base/XString.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
namespace xlib {
    namespace crypto{
        //保留字符 RFC 3986
        const char kUriKeepChar[] = "!#$&'()*+,/:;=?@[]";
        //未保留字符 RFC 3986
        const char kUriUnKeepChar[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-_.~";
        unsigned char hexToChar(const string &str) {
            short c = 0;
            if(!str.empty()) {
                istringstream in(str);
                in >> hex >> c;
                if(in.fail()) {
                    throw runtime_error("stream decode failure");
                }
            }
            return static_cast<unsigned char>(c);
        }
        
        bool isInkUriKeepChar(const char& c){
            return XString::isCharInString(c, string(kUriKeepChar));
        }
        bool isInkUriUnKeepChar(const char& c){
            return XString::isCharInString(c, string(kUriUnKeepChar));
        }
        string urlencode(const string& src,bool encodeKeepUri, bool encodeUnKeepUri){
            ostringstream out;
            for(string::size_type i = 0, len = src.length(); i < len;i++) {
                char t = src.at(i);
                if(' '==t){
                    out<<'+';
                    continue;
                }
                if(false==encodeKeepUri){
                    if(isInkUriKeepChar(t)){
                        out << src.at(i);
                        continue;
                    }
                }
                if(false==encodeUnKeepUri){
                    if(isInkUriUnKeepChar(t)){
                        out << src.at(i);
                        continue;
                    }
                }
                out << '%' << std::hex << std::uppercase << (int)(unsigned char)src[i];
                
//                out<<'%';
//                const char lookup[]= "0123456789ABCDEF";
//                out<<lookup[(((int)(unsigned char)src[i]&0xF0)>>4)];
//                out<<lookup[((int)(unsigned char)src[i]&0x0F)];
            }
            return out.str();
        }
        string urldecode(const string&src){
            ostringstream out;
            for(string::size_type i = 0, len = src.length(); i < len; ++i) {
                 if(src.at(i) == '%') {
                    string str(src.substr(i+1, 2));
                    out << hexToChar(str);
                    i += 2;
                 } else if(src.at(i) == '+'){
                     out<<' ';
                 } else {
                    out << src.at(i);
                }
            }
            return out.str();
        }
    }
}
