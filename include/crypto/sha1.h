//
//  sha1.h
//  Xlib
//
//  Created by Ximena on 2018/12/20.
//  Copyright © 2018 Ximena. All rights reserved.
//

#ifndef sha1_h
#define sha1_h
#include <string>
#include "macro/XBase.h"
namespace xlib {
    namespace crypto{
        class SHA1{
        public:
            SHA1();
            void update(const std::string &s);
            void update(std::istream &is);
            std::string final();
            static std::string from_file(const std::string &filename);
        private:
            uint32 digest[5];
            std::string buffer;
            size_t transforms;
        };
    }//! end crypto
}//!end xlib
#endif /* sha1_h */
