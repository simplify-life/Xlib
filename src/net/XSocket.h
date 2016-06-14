//----------------------------------------------//
//  XSocket.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/14.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XSocket_h
#define XSocket_h
#include "netconfig.h"
#include "../XBase.h"

XLIB_BEGAIN
namespace net{

	using ipV4 = sockaddr_in;
  	using ipV6 = sockaddr_in6;

    class XSocketTCP
    {
    public:
        XSocketTCP();
        virtual ~XSocketTCP();
        
        bool startClient();
        bool startServer();
        
        void stop();
    private:
        DISALLOW_COPY_AND_ASSIGN(XSocketTCP);
        bool _running;
    };
    
}
XLIB_END

#endif /* XSocket_h */
