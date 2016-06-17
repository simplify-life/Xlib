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
    
    
    
    SocketException::SocketException(const std::string &message)
    {
        this->_error = message;
    }
    
    SocketException::~SocketException() throw()
    {
    }
    
    const char* SocketException::what() const throw()
    {
        return this->_error.c_str();
    }
    
    std::ostream& operator<< (std::ostream &out, SocketException &e)
    {
        out << e.what();
        return out;
    }
    
    
    ////////////////////XSOCKET//////////////////////
    XSocket::XSocket():mSocket(INVALID_SOCKET),isBlock(false),_running(false),_threadEnd(false)
    {
        
    }
    
    XSocket::~XSocket()
    {
        Clean();
    }
    
    pid_t XSocket::Fork()
    {
        pid_t pid = fork();
        if(pid<0) return -1;//throw SocketException("fork failed!");
        return pid;
    }
    
    void XSocket::IPV4(ipv4* addrV4, short protocolFamily, uint port, const std::string &addr)
    {
        bzero(addrV4, sizeof(addrV4));
        addrV4->sin_family = protocolFamily;
        addrV4->sin_port = htons(port);
        addrV4->sin_addr.s_addr=inet_addr(addr.c_str());
    }
    
    void XSocket::IPV6(ipv6* addrV6, short protocolFamily, uint port, const in6_addr &addr)
    {
        bzero(addrV6, sizeof(addrV6));
        addrV6->sin6_family = protocolFamily;
        addrV6->sin6_port = htons(port);
        addrV6->sin6_addr= addr;
    }
    
    SOCKET XSocket::Socket(const _socket& aSocket)
    {
        mSocket = socket(aSocket.protocolFamily,aSocket.socketType,aSocket.protocol);
        if(mSocket==INVALID_SOCKET)
        {
            // add handler error code here
            throw SocketException("init socket failed!");
        }
        return mSocket;
    }
    
    
    int32 XSocket::Connect(SOCKET sd, const struct sockaddr *addr, int32 addrLen)
    {
        int32 code = connect(sd,addr,addrLen);
        if(code==SOCKET_ERROR)
        {
            // add handler error code here
            throw SocketException("connect failed!");
        }
        return code;
    }
    
    
    int32 XSocket::Close()
    {
        return close(mSocket);
    }
    
    
    int32 XSocket::Clean()
    {
        _running = false;
        return clean();
    }
    
    ///////////////////////TCP//////////////////////
    XSocketTCP::XSocketTCP()
    {
        while (!_msgQueue.empty())
        {
            _msgQueue.pop();
        }
    }
    
    XSocketTCP::~XSocketTCP()
    {
        stop();
    }
    
    void XSocketTCP::stop()
    {
        if( _running ) {
            _threadEnd = true;
            if (_thread.joinable())
            {
                _thread.join();
            }
        }
    }
    
    bool XSocketTCP::startClient(const _server& aServer,bool isSync)
    {
        if(!checkHost(aServer.ip)) throw SocketException("error ip!");
        _socket tcpSocket;
        tcpSocket.protocolFamily = AF_INET;
        tcpSocket.socketType = SOCK_STREAM;
        tcpSocket.protocol = IPPROTO_TCP;
        mSocket = Socket(tcpSocket);
        ipv4 addr;
        IPV4((ipv4 *)&addr, AF_INET, aServer.port, std::string(aServer.ip));
        Connect(mSocket, (sockaddr*)&addr, sizeof(addr));
        _running = true;
        isBlock = !isSync;
        if(!isBlock)
        {
            _thread = std::thread( std::bind( &XSocketTCP::loop, this) );
        }
        return true;
    }
    
    bool XSocketTCP::startServer(int port)
    {
        Listen(port);
        while(1)
        {
            SOCKET client = accept(mSocket, nullptr, nullptr);
            if(client!=INVALID_SOCKET)
            {
                //_clients.emplace_back(client);
                //if(Fork()==0)
                handlerClient(client);
                
            }
            else
                CloseClient(client);
        }
        return true;
    }
    
    void XSocketTCP::CloseClient(const SOCKET s)
    {
        close(s);
    }
    
    void XSocketTCP::handlerClient(const SOCKET client)
    {
        if(client==INVALID_SOCKET) return;
        fd_set _read_set;
        struct timeval timeout;
        FD_ZERO(&_read_set);
        FD_SET(client,&_read_set);
        _running = true;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;
        
        while (1)
        {
            int s=0;
            if((s = select(0,&_read_set,NULL,NULL,&timeout))==-1)
            {
                /* error */
                if(errno != EINTR)
                    throw SocketException("Abnormal error in select()\n");
                continue;
            }
            else if( s == 0 )
            {
                /* timeout. do something ? */
                //break;
            }
            if(FD_ISSET(client, &_read_set))
            {
                _msg msg;
                Receive(client,msg.str_msg,1024);
                std::cout<<msg.str_msg<<std::endl;
                std::string ss = "I received msg:";
                ss.append(msg.str_msg);
                Send(client,ss.c_str(),ss.length());
            }
        }
        /**
         You'd better add a heartbeat packets to determine whether to close the client.
         */
        CloseClient(client);
    }
    
    
    void XSocketTCP::loop()
    {
        fd_set _read_set;
        struct timeval timeout;
        FD_ZERO(&_read_set);
        FD_SET(mSocket,&_read_set);
        _running = true;
        timeout.tv_sec = 0;
        
        /*
         if your engine is cocos2dx
         0.016 seconds. Wake up once per frame at 60PFS.
         else
         And you can define your timeout depending on your needs  */
        timeout.tv_usec = 16000;
        
        while (!_threadEnd)
        {
            int s=0;
            if((s = select(0,&_read_set,NULL,NULL,&timeout))==-1)
            {
                /* error */
                if(errno != EINTR)
                    throw SocketException("Abnormal error in select()\n");
                continue;
            }
            else if( s == 0 )
            {
                /* timeout. do something ? */
            }
            if(FD_ISSET(mSocket, &_read_set))
            {
                _msg msg;
                Receive(msg.str_msg,1024);
                _msgQueue.emplace(msg);
                handlerMessage();
            }
        }
    }
    
    
    
    
    std::string XSocketTCP::Receive()
    {
        std::string ret="";
        char buf[SOCKET_MAX_BUFFER_LEN];
        while(ret.find("\n")==std::string::npos)
        {
            ssize_t s=0;
            if((s=recv(mSocket, buf, 1024, 0))==-1)
            {
                if(errno == EINTR) continue;
                throw SocketException("receive failed!");
            }
            else if(s==0) throw SocketException("the socket is closed!");
            ret.append(buf,s);
        }
        return ret;
    }
    
    
    void XSocketTCP::handlerMessage()
    {
        //add your handler code here
        while(!_msgQueue.empty())
        {
            std::cout<<_msgQueue.front().str_msg<<std::endl;
            _msgQueue.pop();
        }
    }
    
    bool XSocketTCP::checkHost(const std::string &host)
    {
        const char *pChar;
        bool rv = true;
        int tmp1, tmp2, tmp3, tmp4, i;
        while( 1 )
        {
            i = sscanf(host.c_str(), "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);
            
            if( i != 4 )
            {
                rv = false;
                break;
            }
            if( (tmp1 > 255) || (tmp2 > 255) || (tmp3 > 255) || (tmp4 > 255) )
            {
                rv = false;
                break;
            }
            for( pChar = host.c_str(); *pChar != 0; pChar++ )
            {
                if( (*pChar != '.')
                   && ((*pChar < '0') || (*pChar > '9')) )
                {
                    rv = false;
                    break;
                }
            }
            break;
        }
        
        return rv;
    }
    
    
    
    int32 XSocketTCP::Listen(int port)
    {
        _socket tcpSocket;
        tcpSocket.protocolFamily = AF_INET;
        tcpSocket.socketType = SOCK_STREAM;
        tcpSocket.protocol = IPPROTO_TCP;
        mSocket = Socket(tcpSocket);
        IPV4(&_serverAddr, AF_INET, port, "0.0.0.0");
        int32 ret = 0;
        if((ret=bind(mSocket, (const struct sockaddr*)&_serverAddr,sizeof(struct sockaddr)))==-1)
        {
            if(errno == EINTR) return Listen(port);
            throw SocketException("");
        }
        if(listen(mSocket, 5)!=0)
        {
            std::stringstream error;
            error << "[listen_on_port] with [port=" << port << "] [listeners=" << 5 << "] Cannot bind socket";
            throw SocketException(error.str());
        }
        return ret;
    }
}
XLIB_END
