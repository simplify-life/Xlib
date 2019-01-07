//
//  url.h
//  Xlib
//
//  Created by Ximena on 2018/12/28.
//  Copyright © 2018 Ximena. All rights reserved.
//

#ifndef url_h
#define url_h
#include <string>
namespace xlib {
    namespace crypto{
        std::string urlencode(const std::string&,bool encodeKeepUri=true, bool encodeUnKeepUri=true);
        std::string urldecode(const std::string&);
    }
}

#endif /* url_h */
