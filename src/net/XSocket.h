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
    

    class XSocket
    {
    public:
        XSocket();
        virtual ~XSocket();
        
        bool startClient();
        bool startServer();
        
        void stop();
    private:
        DISALLOW_COPY_AND_ASSIGN(XSocket);
        bool _running;
    };
    
}
XLIB_END

#endif /* XSocket_h */
