//----------------------------------------------//
//  base64.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef base64_h
#define base64_h
#include <string>

namespace xlib {
 
    namespace crypto{
        
        const char kBase64Char[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
        void _3to4(unsigned char * a3, unsigned char * a4);
        void _4to3(unsigned char * a3, unsigned char * a4);
        unsigned char get64Index(unsigned char c);
        std::string encode64(const std::string& in);
        std::string decode64(const std::string& in);
        
    }//! end crypto
    
}//!end xlib



#endif /* base64_h */
