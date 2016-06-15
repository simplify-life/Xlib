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
#include <string>
#include <exception>
#include <iosfwd>
#include <iostream>
#include <sstream>
XLIB_BEGAIN
namespace net{

    
    /**
     @brief a socket describe struct
     @param protocolFamily
     example:AF_INET/AF_INET6/AF_LOCAL(AF_UNIX)/AF_ROUTE
     @param socketType
     example:TCP->SOCK_STREAM/UDP->SOCK_DGRAM/SOCK_RAW/SOCK_PACKET/SOCK_SEQPACKET
     @param protocol
     example:IPPROTO_TCP/IPPROTO_UDP/IPPROTO_STCP/IPPROTO_TIPC
     */
    struct _socket
    {
        int protocolFamily;
        int socketType;
        int protocol;
    };
    
    struct _server
    {
        int port;
        const char* ip;
        _server(int port,const std::string& ip)
        {
            this->port = port;
            this->ip = ip.c_str();
        }
    };
    
    
    class SocketException : public std::exception
    {
    private:
        std::string _error;
        
    public:
        SocketException(const std::string&);
        ~SocketException() throw();
        
        virtual const char* what() const throw();
        friend std::ostream& operator<< (std::ostream &out, SocketException &e);
    };
    
    
    
    class XSocket
    {
    public:
        XSocket();
        virtual ~XSocket();
        
        void IPV4(ipv4* addrV4,short protocolFamily,uint port,const std::string& addr);
        void IPV6(ipv6* addrV6,short protocolFamily,uint port,const in6_addr& addr);

        SOCKET Socket(const _socket& aSocket);
        
        int32 Connect(SOCKET sd,const struct sockaddr* addr,int32 addrLen);
        
        int32 Send(const char* buffer,size_t len);
        std::string Receive();
        int32 Close();
        int32 Clean();
        virtual void handlerMessage(const std::string& message)=0;

    protected:
        void loop();
    protected:
        SOCKET mSocket;
        bool isBlock;
        bool _running;
        bool _threadEnd;
    };
    
    
    
    class XSocketTCP:public XSocket
    {
    private:
        std::thread _thread;
    public:
        XSocketTCP();
        virtual ~XSocketTCP();
        
        bool startClient(const _server& aServer,bool isSync=false);
        bool startServer();
        
        void handlerMessage(const std::string& message) override;
        
    protected:
        bool checkHost(const std::string& host);
        void stop();
    private:
        DISALLOW_COPY_AND_ASSIGN(XSocketTCP);
        
        
    };
    
}
XLIB_END

#endif /* XSocket_h */
