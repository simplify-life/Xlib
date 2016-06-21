//----------------------------------------------//
//  base64.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "base64.h"


namespace xlib {
    namespace crypt{
        
        
        void _3to4(unsigned char * a3, unsigned char * a4)
        {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = (a3[2] & 0x3f);
        }
        void _4to3(unsigned char * a3, unsigned char * a4)
        {
            a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
            a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
            a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
        }
        
        
        unsigned char get64Index(unsigned char c)
        {
            if(c >='A' && c <='Z') return c - 'A';
            if(c >='a' && c <='z') return c - 71;
            if(c >='0' && c <='9') return c + 4;
            if(c == '+') return 62;
            if(c == '/') return 63;
            return 255;
        }
        
        
        std::string encode64(const std::string& in)
        {
            std::string out;
            int i=0,j=0;
            size_t len_in = in.length(),len_out=0,len_out_target=(len_in+2 - ((len_in+2)% 3)) / 3 * 4;
            out.resize(len_out_target);
            unsigned char _in[3],_out[4];
            std::string::const_iterator input = in.begin();
            while(len_in--)
            {
                _in[i++]=*(input++);
                if(3==i)
                {
                    _3to4(_in, _out);
                    for(i = 0;i < 4; i++)
                    {
                        out[len_out++] =  kBase64Char[_out[i]];
                    }
                    i = 0;
                }
            }
            if(i)
            {
                for (j = i; j < 3; j++)
                {
                    _in[j] = '\0';
                }
                _3to4(_in, _out);
                for(j = 0;j < i+1; j++)
                {
                    out[len_out++] =  kBase64Char[_out[j]];
                }
                while(i++<3)
                {
                    out[len_out++] = '=';
                }
                
            }
            return out;
        }
        
        
        std::string decode64(const std::string& in)
        {
            std::string out;
            size_t numEq = 0,n = in.size(),len_in = in.length(),len_out=0,i=0,j=0;
            
            for (std::string::const_reverse_iterator it = in.rbegin(); *it == '='; ++it)
            {
                ++numEq;
            }
            size_t len_out_target = ((3 * n) / 4) - numEq;
            out.resize(len_out_target);
            
            unsigned char _in[4],_out[3];
            std::string::const_iterator input = in.begin();
            
            while(len_in--)
            {
                if (*input == '=')
                {
                    break;
                }
                _in[i++]=*(input++);
                if(i==4)
                {
                    for(i=0;i<4;i++)
                    {
                        _in[i]=get64Index(_in[i]);
                    }
                    
                    _4to3(_out, _in);
                    for(i=0;i<3;i++)
                    {
                        out[len_out++]=_out[i];
                    }
                    i=0;
                }
            }
            
            if(i)
            {
                for(j=i;j<4;j++)
                {
                    _in[j]='\0';
                }
                for (j = 0; j < 4; j++)
                {
                    _in[j] = get64Index(_in[j]);
                }
                _4to3(_out, _in);
                for(j=0;j<i-1;j++)
                {
                    out[len_out++] = _out[j];
                }
                
            }
            
            return out;
        }
        
        
        
        
    }//end crypt
}//end xlib