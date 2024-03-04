//----------------------------------------------//
//  XString.cpp
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "base/XString.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <algorithm>
#include <map>

using namespace std;

XLIB_BEGAIN

string XString::format(const char *fmt, ...)
{
#define X_MAX_STRING_LENGTH (1024 * 100)

    string ret;

    va_list ap;
    va_start(ap, fmt);

    char *buf = (char *)malloc(X_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, X_MAX_STRING_LENGTH, fmt, ap);
        ret = buf;
        free(buf);
    }
    va_end(ap);
    return ret;
}

string XString::toUpper(const string &str)
{
    string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), [](int c) -> int
                   { return toupper(c); });
    return ret;
}

string XString::toLower(const string &str)
{
    string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), [](int c) -> int
                   { return tolower(c); });
    return ret;
}

string XString::trim(const string &str,const string &key)
{
    return replace(str, key, "");
}

vector<string> XString::split(const string &str,const string &key)
{
    vector<string> ret;
    string::size_type start = 0, end = 0, increment = key.size();
    if (0 == increment)
        return ret;
    string temp = str;
    temp.append(key); // append to get tail data
    while ((end = temp.find(key, start)) != string::npos)
    {
        string s = temp.substr(start, temp.find(key, start) - start);
        if (!s.empty()) /*Remove empty data*/
            ret.emplace_back(s);
        start = end + increment;
    }
    return ret;
}

std::string XString::replace(const std::string &src, const std::string &sfind, const std::string &sreplace)
{
    ostringstream os;
    string::size_type cursor = 0;
    string::size_type len = src.length();
    string::size_type flen = sfind.length();
    while (cursor < len)
    {
        auto pos = src.find(sfind, cursor);
        if (string::npos != pos)
        {
            os << src.substr(cursor, pos - cursor);
            cursor = pos;
            os << sreplace;
            cursor += flen;
        }
        else
            break;
    }
    return os.str();
}

std::size_t XString::findNthOccurrence(const std::string &str, char c, int n)
{
    std::size_t pos = std::string::npos;
    for (int i = 0; i < n; i++)
    {
        if (pos == std::string::npos)
        {
            pos = str.find(c, 0);
        }
        else
        {
            pos = str.find(c, pos + 1);
        }
        if (pos == std::string::npos)
        {
            return pos;
        }
    }
    return pos;
}

int XString::count(const string &key, const string &str)
{

    if (key.empty() || str.empty())
        return 0;
    int t = 0;
    string::size_type start = 0, end = 0, increment = key.size();
    while ((end = str.find(key, start)) != string::npos)
    {
        start = end + increment;
        t++;
    }
    return t;
}

bool XString::isInt(const string &str, bool ignoreSymbols)
{
    if (!str.empty())
    {
        if (ignoreSymbols)
        {
            for (auto &i : str)
            {
                if (i < '0' || i > '9')
                    return false;
            }
            return true;
        }
        else
        {
            auto first = str.begin();
            if ((*first != '-') && (*first < '0' || *first > '9'))
                return false;
            for (auto i = str.begin() + 1; i != str.end(); i++)
            {
                if (*i < '0' || *i > '9')
                    return false;
            }
            return true;
        }
    }
    return false;
}

int XString::compareVersion(const string &sv1, const string &sv2, const string &key)
{
    if (sv1.empty() || sv2.empty() || key.empty())
        return 3;
    auto v1 = split(key, sv1);
    auto v2 = split(key, sv2);
    string::size_type count = v1.size() > v2.size() ? v2.size() : v1.size();
    for (string::size_type i = 0; i < count; i++)
    {
        if (!isInt(v1.at(i)) || !isInt(v2.at(i)))
            return 3;
        int v1_int = atoi(v1.at(i).c_str());
        int v2_int = atoi(v2.at(i).c_str());
        if (v1_int > v2_int)
            return 1;
        if (v1_int < v2_int)
            return -1;
    }
    if (v1.size() > v2.size())
        return 1;
    if (v1.size() < v2.size())
        return -1;
    return 0;
}

string XString::formatTime(const tm *ttime, const xlib::TIME_F &fmt)
{
#define TIME_STR_SIZE 80
    char t[TIME_STR_SIZE];
    string sfmt = "";
    switch (fmt)
    {
    case xlib::TIME_F::H12_M_S:
        sfmt = "%I:%M:%S";
        break;
    case xlib::TIME_F::H24_M_S:
        sfmt = "%H:%M:%S";
        break;
    case xlib::TIME_F::Y_M_D:
        sfmt = "%Y-%m-%d";
        break;
    case xlib::TIME_F::W_D:
        sfmt = "%A";
        break;
    case xlib::TIME_F::LOG_TIME:
        sfmt = "%Y-%m-%d %H:%M:%S";
        break;
    case xlib::TIME_F::T_DEFAULT:
        sfmt = "%Y-%m-%d %A %H:%M:%S";
    default:
        break;
    }
    strftime(t, TIME_STR_SIZE, sfmt.c_str(), ttime);
    return string(t);
}

string XString::formatTime(const XTime::tm_million *t_m, const xlib::TIME_F &fmt)
{
#define TIME_STR_SIZE 80
    char t[TIME_STR_SIZE];
    string sfmt = "";
    switch (fmt)
    {
    case xlib::TIME_F::H12_M_S:
        sfmt = "%I:%M:%S";
        break;
    case xlib::TIME_F::H24_M_S:
        sfmt = "%H:%M:%S";
        break;
    case xlib::TIME_F::Y_M_D:
        sfmt = "%Y-%m-%d";
        break;
    case xlib::TIME_F::W_D:
        sfmt = "%A";
        break;
    case xlib::TIME_F::LOG_TIME:
        sfmt = "%Y-%m-%d %H:%M:%S";
        break;
    case xlib::TIME_F::T_DEFAULT:
        sfmt = "%Y-%m-%d %A %H:%M:%S";
    default:
        break;
    }
    strftime(t, TIME_STR_SIZE, sfmt.c_str(), t_m->tm);
    string million = XString::format(".%03d", t_m->million);
    free((void *)t_m);
    return string(t).append(million.c_str());
}

bool XString::isCharInString(const char &c, const string &src)
{
    return src.find(XString::convert<string>(c)) != string::npos;
}

std::vector<std::string> XString::lettersShape(const std::string &letters)
{
    std::map<char, std::vector<std::string>>  lettersPair = {
        {'A',
         {"  __  ",
          " /  \\ ",
          "|    |",
          "|====|",
          "|    |"}},
        {'B',
         {"|===\\ ",
          "|    |",
          "|===/ ",
          "|    |",
          "|___/ "}},
        {'C',
         {
             "  ____ ",
             " /     ",
             "|      ",
             "|      ",
             " \\___  ",
         }},
        {'D',
         {
             "|====\\ ",
             "|     |",
             "|     |",
             "|     |",
             "|____/ ",
         }},
        {'E',
         {"|====|",
          "|     ",
          "|===  ",
          "|     ",
          "|____|"}},
        {'F',
         {"|====|",
          "|     ",
          "|===  ",
          "|     ",
          "|     "}},
        {'G',
         {
             "  ___  ",
             " /     ",
             "|   ___",
             "|    | ",
             " \\___/ ",
         }},
        {'H',
         {"|    | ",
          "|    | ",
          "|====| ",
          "|    | ",
          "|    | "}},
        {'I',
         {" ====  ",
          "  ||   ",
          "  ||   ",
          "  ||   ",
          "______ "}},
        {'J',
         {"     | ",
          "     | ",
          "     | ",
          "|    | ",
          " \\___/ "}},
        {'K',
         {"|    /",
          "|   / ",
          "|__/  ",
          "|  \\  ",
          "|   \\ "}},
        {'L',
         {"===     ",
          " |      ",
          " |      ",
          " |      ",
          "_|____/ "}},
        {'M',
         {"|     | ",
          "|\\   /| ",
          "| \\ / | ",
          "|     | ",
          "|     | "}},
        {'N',
         {"|    | ",
          "|\\   | ",
          "| \\  | ",
          "|  \\ | ",
          "|   \\| "}},
        {'O',
         {" ____  ",
          "/    \\ ",
          "|    | ",
          "|    | ",
          "\\____/ "}},
        {'P',
         {"|===\\  ",
          "|    | ",
          "|===/  ",
          "|      ",
          "|      "}},
        {'Q',
         {" ___   ",
          "/   \\  ",
          "|    | ",
          "|  \\ | ",
          "\\___\\  "}},
        {'R',
         {"|====\\ ",
          "|    | ",
          "|===/  ",
          "| \\    ",
          "|  \\   "}},
        {'S',
         {" ___  ",
          "/ __) ",
          "\\__\\  ",
          "(   / ",
          "\\___) "}},
        {'T',
         {"______ ",
          "------ ",
          "  ||   ",
          "  ||   ",
          "  ||   "}},
        {'U',
         {"|    | ",
          "|    | ",
          "|    | ",
          "|    | ",
          "\\___/  "}},
        {'V',
         {"         ",
          "\\      / ",
          " \\    /  ",
          "  \\  /   ",
          "   \\/    "}},
        {'W',
         {
             "|    | ",
             "|    | ",
             "|    | ",
             "| /\\ | ",
             "|/  \\| ",
         }},
        {'X',
         {"\\   / ",
          " \\ /  ",
          "  X   ",
          " / \\  ",
          "/   \\ "}},
        {'Y',
         {" \\   /",
          "  \\ / ",
          "   |  ",
          "   |  ",
          "   |  "}},
        {'Z',
         {"____ ",
          "   / ",
          "  /  ",
          " /   ",
          "/___ "}},
        {'-',
         {"      ",
          "      ",
          " ==== ",
          "      ",
          "      "}},
    };
    std::string str0;
    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;
    for (const char& letter : letters) {
         char uppercaseLetter = std::toupper(letter);
         if (lettersPair.find(uppercaseLetter) != lettersPair.end()) {
            str0.append(lettersPair[uppercaseLetter][0]);
            str1.append(lettersPair[uppercaseLetter][1]);
            str2.append(lettersPair[uppercaseLetter][2]);
            str3.append(lettersPair[uppercaseLetter][3]);
            str4.append(lettersPair[uppercaseLetter][4]);
         }
    }
    std::vector<std::string> result = {str0,str1,str2,str3,str4};
    return result;
}

    std::size_t XString::findFirstCharNotEscapedBefore(const std::string &str, char ch){
        bool isEscaped = false;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '\\' && !isEscaped) {
                isEscaped = true;
            } else {
                if (str[i] == ch && !isEscaped) {
                    return i;
                }
                isEscaped = false;
            }
        }
        return std::string::npos; 
    }

XLIB_END
