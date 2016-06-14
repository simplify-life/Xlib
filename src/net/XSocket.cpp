//----------------------------------------------//
//  XSocket.cpp
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/14.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "XSocket.h"

XLIB_BEGAIN
namespace net {
#ifndef socklen_t
#define socklen_t uint32
#endif
    XSocket::XSocket()
    {
        
    }
    
    XSocket::~XSocket()
    {
        stop();
    }
    
    void XSocket::stop()
    {
        
    }
    
    bool XSocket::startClient()
    {
        return false;
    }
    
    bool XSocket::startServer()
    {
        return false;
    }
    
    
}
XLIB_END