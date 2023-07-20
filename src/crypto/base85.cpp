//----------------------------------------------//
//  base85.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "crypto/base85.h"
#include "base/XString.h"
#include <string>

namespace xlib
{
    namespace crypto
    {
        const char en85[] = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
            'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
            'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
            'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
            'u', 'v', 'w', 'x', 'y', 'z',
            '!', '#', '$', '%', '&', '(', ')', '*', '+', '-',
            ';', '<', '=', '>', '?', '@', '^', '_', '`', '{',
            '|', '}', '~'};
        const unsigned char const de85[] = {
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            62,
            85,
            63,
            64,
            65,
            66,
            85,
            67,
            68,
            69,
            70,
            85,
            71,
            85,
            85,
            0,
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9,
            85,
            72,
            73,
            74,
            75,
            76,
            77,
            10,
            11,
            12,
            13,
            14,
            15,
            16,
            17,
            18,
            19,
            20,
            21,
            22,
            23,
            24,
            25,
            26,
            27,
            28,
            29,
            30,
            31,
            32,
            33,
            34,
            35,
            85,
            85,
            85,
            78,
            79,
            80,
            36,
            37,
            38,
            39,
            40,
            41,
            42,
            43,
            44,
            45,
            46,
            47,
            48,
            49,
            50,
            51,
            52,
            53,
            54,
            55,
            56,
            57,
            58,
            59,
            60,
            61,
            81,
            82,
            83,
            84,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
            85,
        };

        void i4toa5(unsigned long acc, std::string &out)
        {

            std::string temp;
            for (int i = 0; i < 5; i++)
            {
                temp.insert(temp.begin(), en85[acc % 85]);
                acc /= 85;
                if(acc==0) break;
            }
            out.append(temp);
        }

        void a5toi4(unsigned long acc, std::string &out)
        {
            std::string temp;
            for (int i = 0; i < 4; i++)
            {
                char c = acc % 256;
                temp.push_back(c);
                acc >>= 8;
                if(acc==0) break;
            }
            out.append(temp.rbegin(), temp.rend());
        }

        std::string encode85(const std::string &in)
        {
            std::string out;
            unsigned long acc = 0;
            int count = 0;
            for (auto c : in)
            {
                // 1次1个字节
                acc = (acc << 8) | (unsigned char)c;
                if (++count == 4)
                {
                    // 满4字节后，4转5
                    i4toa5(acc, out);
                    acc = 0;
                    count = 0;
                }
            }
            if (count > 0) // 处理剩余的字符
            {
                // for (int i = count; i < 4; i++)
                // {
                //    acc <<= 8; // 用0填充，填满4个字节
                // }
                // 4转5
                i4toa5(acc, out);
            }
            // 添加原始数据的长度
            // out.push_back(':');
            // out.push_back((4-count) + '0');
            return out;
        }

        std::string decode85(const std::string &in)
        {
            std::string out;
            unsigned long acc = 0;
            int count = 0;
            // 获取原始数据的长度
            // auto arr = XString::split(":",in);
            // int subLen = std::stoi(arr.at(1));
            for (auto c : in)
            {
                // if (c == ':') // 遇到特殊字符，停止解码
                // {
                //     break;
                // }
                acc = acc * 85 + de85[c];
                if (++count == 5)
                {
                    a5toi4(acc, out);
                    acc = 0;
                    count = 0;
                }
            }
            if (count > 0) // 处理剩余的字符
            {
                // for (int i = count; i < 5; i++)
                // {
                //     acc *= 85; // 用0填充
                // }
                a5toi4(acc, out);
            }
            // 去除额外的0
            // out = out.substr(0, out.length()-subLen);
            return out;
        }
    } // end crypto
} // end xlib
