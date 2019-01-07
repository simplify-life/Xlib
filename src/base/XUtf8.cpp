//----------------------------------------------//
//  XUtf8.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/7.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "base/XUtf8.h"
#include "macro/XBinary.h"
#include "macro/XDefine.h"
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>

//#include <cassert>
XLIB_BEGAIN

using namespace std;

string XUtf8::utf8ToUnicode(const string& src)
{
	queue<string> q;
	for (string::size_type sss = 0; sss < src.length(); sss++)
	q.emplace(binary_impl<>(static_cast<uint8_t>(src[sss])).type);

	string s,st;
	while (!q.empty())
	{
		auto tmp = q.front();
		if (tmp.length() != 8)
		{
			int i = 8 - (int)tmp.length();
			if(i<0)
			break;
			else
			{
				string ttt = "";
				while (i)
				{
					ttt.append("0");
					i--;
				}
				tmp = ttt.append(tmp);
			}
		}			
		if (tmp[0] == '0')
		{
			char buf[32];
			snprintf(buf, sizeof buf, "\\u%04x", static_cast<uint>(binary_impl<>(tmp).value));
			s.append(buf);
		}
		else
		{
				if (tmp[1] == '1')
				{
					if (st != "")
					{
						char buf[32];
						snprintf(buf, sizeof buf, "\\u%04x", static_cast<uint>(binary_impl<>(st).value));
						s.append(buf);
					}
					st = "";
					int c1 = 2;
					while (c1<6)
					{
						if (tmp[c1] == '0') c1++;
						else break;
					}
					st = tmp.substr(c1+1);
				}
				else
				{
					st.append(tmp.substr(2));
				}
		}
		q.pop();
	}
	if (st != "")
	{
		char buf[32];
		snprintf(buf, sizeof buf, "\\u%04x",  static_cast<uint>(binary_impl<>(st).value));
		s.append(buf);
	}
	return s;
}

string XUtf8::unicodeToUtf8(const char* src)
{
    
    char *tmp;
    int place = 0;
    auto utf8code = std::strtol(src, &tmp , 16);
    if(utf8code<0x80)
    {
        place = 1;//0xxxxxxx
        char utf[2] = {static_cast<char>(utf8code),0};
        return string((char*)utf,1);
    }
    else if (utf8code<0x800)
    {
        place = 2;//110xxxxx 10xxxxxx
        char utf[3]={static_cast<char>(0xc0|(utf8code>>6)),static_cast<char>(0x80|(utf8code&0x3f)),0};
        return string((char*)utf,2);
    }
    else if (utf8code<0x10000)
    {
        place = 3;//1110xxxx 10xxxxxx 10xxxxxx
        char utf[4]={static_cast<char>(0xe0|(utf8code>>12)),static_cast<char>(0xc0|(utf8code>>6)),static_cast<char>(0x80|(utf8code&0x3f)),0};
        return string((char*)utf,3);
    }else if (utf8code<0x200000)
    {
        place = 4;//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        char utf[5]={static_cast<char>(0xf0|(utf8code>>18)), static_cast<char>(0xe0|(utf8code>>12)),static_cast<char>(0xc0|(utf8code>>6)),static_cast<char>(0x80|(utf8code&0x3f)),0};
        return string((char*)utf,4);
    }
    return "";
}

unsigned int XUtf8::getlen(const char* utf8)
{
    unsigned int c=0,len = 0;
    while(utf8[c])
    {
        if((utf8[c]&0xc0)!=0x80) len++;
        c++;
    }
        return len;
}

unsigned int XUtf8::getlen(const string & s_utf8)
{
    return getlen(s_utf8.c_str());
}


string XUtf8::getWord(unsigned int idx, const string &src)
{
    const char* src_ = src.c_str();
    unsigned int c=0,len = 0 ;
    string::size_type start=string::npos,end = string::npos;
    while (src_[c])
    {
        if((src_[c]&0xc0)!=0x80)
        {
            if(idx==len&&start==string::npos)
            {
                start = c;
            }
            len++;
        }
        c++;
        if(idx+2==len&&end==string::npos)
        {
            end = c-1;
            break;
        }
    }
    if((start==end)&&start==string::npos) return ""; //out of range
    return src.substr(start,end-start);
}

unordered_map<unsigned int,unsigned int> XUtf8::getMsg(const string &src)
{
    unordered_map<unsigned int,unsigned int> map_msg;
    const char* src_ = src.c_str();
    unsigned int c=0,len =0;
    while (src_[c])
    {
        if((src_[c]&0xc0)!=0x80)
        {
            map_msg.emplace(len,0);
            len++;
        }
        map_msg.find(len-1)->second++;
        c++;
    }
    return map_msg;
}

string XUtf8::substr(const string &src, unsigned int startPos, unsigned int len)
{
    auto msg = getMsg(src);
    if(startPos+len<=msg.size())
    {
        string::size_type start=0,end = 0;
        for(auto e:msg)
        {
            if(e.first<startPos) start+=e.second;
            else if(e.first<startPos+len)
            {
                end+=e.second;
            }
        }
        return src.substr(start,end);
    }
    //assert(0&&"out of range");
    return ""; //out of range
}

uint XUtf8::getUtf8ByteLen(byte b)
{
    if(b<0x80){
        return 1;
    }
    if(b>=0xc0&&b<=0xdf){
        return 2;
    }
    if(b>=0xe0&&b<=0xef){
        return 3;
    }
    if(b>=0xf0&&b<=0xf7){
        return 4;
    }
    return 0;
}

bool XUtf8::isValidUtf8ByteExcept1st(byte b){
    return (b&0x80)==0x80 && (b|0x80)==b;
}

bool XUtf8::isValidUtf8Buffer(byte* buffer, uint size){
    for(uint i = 0 ; i < size ;){
        auto len = getUtf8ByteLen(buffer[i]);
        if(0 == len) return false;
        for(uint j = 1 ; j < len ; j++){
            if(i+j<len){
                bool res = isValidUtf8ByteExcept1st(buffer[i+j]);
                if(res) continue;
                else return false;
            }else return false;
        }
        i += len;
    }
    return true;
}

//string XUtf8::findPrefixUtf8(const std::string& src){
//    string res="";
//    const char* src_ = src.c_str();
//    uint32 c=0;
//    while ('\0'!=src[c])
//    {
//        unsigned int bb;
//        std::stringstream ss;
//        ss << src_[c];
//        ss >> std::hex >> bb;
//        byte b = (byte)src_[c] ;
//        uint bl = getUtf8ByteLen(b);
//        switch (bl) {
//            case 0:
//                return res;
//            break;
//            case 1:
//                res += src_[c];
//            c++;
//                break;
//            case 2:
//            if('\0'!=src_[c+1]){
//                byte d = (byte)src_[c+1] ;
//                if(d<=191){
//                    res += src_[c]+src_[c+1];
//                    c+=2;
//                }
//            }else{
//                return res;
//            }
//            break;
//            case 3:
//            if('\0'!=src_[c+1]&&'\0'!=src_[c+2]){
//                byte d = (byte)src_[c+1] ;
//                if(d<=191){
//                    byte dd = (byte)src_[c+2] ;
//                    if(dd<=191){
//                        res += src_[c]+src_[c+1]+src_[c+2];
//                        c+=3;
//                    }
//                }
//            }else{
//                return res;
//            }
//            break;
//            case 4:
//            if('\0'!=src_[c+1]&&'\0'!=src_[c+2]){
//                byte d = (byte)src_[c+1] ;
//                if(d<=191){
//                    byte dd = (byte)src_[c+2] ;
//                    if(dd<=191){
//                        byte ddd = (byte)src_[c+3] ;
//                        if(ddd<=191){
//                            res += src_[c]+src_[c+1]+src_[c+2]+src_[3];
//                            c+=4;
//                        }
//                    }
//                }
//            }else{
//                return res;
//            }
//            break;
//            default:
//            return res;
//        }
//    }
//    return res;
//}

XLIB_END
