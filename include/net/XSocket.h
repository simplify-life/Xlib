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
#include "macro/XBase.h"
#include <string>
#include <exception>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <queue>
XLIB_BEGAIN
namespace net{
    
    struct _msg
    {
        char str_msg[1024];
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
    
    
    
    //////////////XSocket-----socket wrapper//////////////////
    
    
    
    class XSocket
    {
    public:
        XSocket();
        virtual ~XSocket();
        
        pid_t Fork();
        
        void IPV4(ipv4* addrV4,short protocolFamily,uint port,const std::string& addr);
        void IPV6(ipv6* addrV6,short protocolFamily,uint port,const in6_addr& addr);
        
        SOCKET Socket(const _socket& aSocket);
        
        int32 Connect(SOCKET sd,const struct sockaddr* addr,int32 addrLen);
        
        virtual int32 Listen(int port)=0;
        
        int32 Close();
        int32 Clean();
        
        virtual void handlerMessage()=0;
        
    protected:
        virtual void loop()=0;
    protected:
        SOCKET mSocket;
        bool isBlock;
        bool _running;
        bool _threadEnd;
    };
    
    
    
    ///////////////////////////TCP//////////////////////
    
    class XSocketTCP:public XSocket
    {
    private:
        std::thread _thread;
        std::queue<_msg> _msgQueue;
    public:
        XSocketTCP();
        virtual ~XSocketTCP();
        /**
         start service
         */
        bool startClient(const _server& aServer,const SOCKET_TYPE& type=SOCKET_TYPE::TCP, bool isSync=false);
        bool startServer(int port,const SOCKET_TYPE& type=SOCKET_TYPE::TCP);
        bool startHttpClient(const char* httpHost);
        
        
        std::string Receive();
        
        void handlerMessage() override;
        int32 Listen(int port) override;
        
        template <class T> ssize_t Send(const T* t, size_t len){return net::Send(mSocket,t, len);};
        template <class T> ssize_t Receive(T* t, size_t len){return net::Receive(mSocket,t, len);};
        
    protected:
        
        virtual void loop() override;
        bool checkHost(const std::string& host);
        void stop();
        
        /**
         @brief option about client
         */
        void CloseClient(const SOCKET s=-1);
        void handlerClient(const SOCKET client=-1);
        
    private:
        DISALLOW_COPY_AND_ASSIGN(XSocketTCP);
        ipv4 _serverAddr;
        SOCKET_TYPE _type;
        std::vector<SOCKET> _clients;
        
    };
    
    
    ////////////////////////UDP/////////////////////////
    
    class XSocketUDP
    {
    public:
        
        XSocketUDP();
        virtual ~XSocketUDP();
    public:
        
        bool startServer(int port);
        bool startClient(const char* ip,int port);
    };
    
    
    
}
XLIB_END

#endif /* XSocket_h */
