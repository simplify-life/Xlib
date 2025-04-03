//
//  md5.hpp
//  xlib
//
//  Created by Ximena on 2018/12/5.
//  Copyright © 2018 Ximena. All rights reserved.
//

#ifndef md5_h
#define md5_h
#include "macro/XBase.h"
#include <string>
namespace xlib {
    namespace crypto{
        const std::string MD5(const std::string& message);
        /* Generate md5 digest. */
        const xbyte* md5Digest(const std::string& message);
    }//! end crypto
    
}//!end xlib
#endif /* md5_h */
