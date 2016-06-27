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
    XSocketTCP::XSocketTCP():_type(SOCKET_TYPE::TCP)
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
    
    bool XSocketTCP::startClient(const _server& aServer,const SOCKET_TYPE& type,bool isSync)
    {
        _type = type;
        if(!checkHost(aServer.ip)) throw SocketException("error ip!");
        _socket tcpSocket;
        tcpSocket.protocolFamily = AF_INET;
        tcpSocket.socketType = SOCK_STREAM;
        if(_type == SOCKET_TYPE::TCP)
            tcpSocket.protocol = IPPROTO_TCP;
        else if(_type == SOCKET_TYPE::HTTP)
            tcpSocket.protocol = 0;
        else
            tcpSocket.protocol = IPPROTO_TCP;
	INIT_SOCKET();
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
    
    bool XSocketTCP::startServer(int port,const SOCKET_TYPE& type)
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
    
    
    bool XSocketTCP::startHttpClient(const char* httpHost)
    {
        struct hostent *hostinfo = nullptr;
        hostinfo = gethostbyname(httpHost);
        if(!hostinfo) return false;
        _socket tcpSocket;
        tcpSocket.protocolFamily = AF_INET;
        tcpSocket.socketType = SOCK_STREAM;
        tcpSocket.protocol = IPPROTO_TCP;

        INIT_SOCKET();
        mSocket = Socket(tcpSocket);
        ipv4 addr;
        addr.sin_addr = *(in_addr*) hostinfo->h_addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        Connect(mSocket, (sockaddr*)&addr, sizeof(addr));
        _running = true;
        _thread = std::thread( std::bind( &XSocketTCP::loop, this) );
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
            if((s = select(client+1,&_read_set,NULL,NULL,&timeout))==-1)
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
                net::Receive(client,msg.str_msg,1024);
                std::cout<<msg.str_msg<<std::endl;
                std::string ss = "I received msg:";
                ss.append(msg.str_msg);
                net::Send(client,ss.c_str(),ss.length());
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
		ssize_t n = net::Receive(mSocket,msg.str_msg,1024);
                if(n==0)
                {
                    break;
                }
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
    
    /**
        UDP
     */
    XSocketUDP::XSocketUDP(){}
    XSocketUDP::~XSocketUDP(){}
    
    bool XSocketUDP::startServer(int port)
    {
        
        std::cout<<"start UDP server..."<<std::endl;
        
        SOCKET sock;
        struct sockaddr_in server_socket;
        struct sockaddr_in client_socket;
        
        if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        {
            std::cout<<"create UDP server Socket error!"<<std::endl;
            return false;
        }
        
#ifndef SO_REUSEPORT
#define SO_REUSEPORT SO_REUSEADDR
        std::cout<<"start UDP server SO_REUSEPORT NOT defined!"<<std::endl;
        return false;
#endif
        int one = 1;
        if(setsockopt(sock,SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one))==-1)
        {
            std::cout<<"start UDP server setsockopt error!"<<std::endl;
            return false;
        }

        bzero(&server_socket, sizeof(server_socket));
        server_socket.sin_family = AF_INET;
        server_socket.sin_port = htons(port);
        server_socket.sin_addr.s_addr=htonl(INADDR_ANY);
        if (bind(sock, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0)
        {
            std::cout<<"start UDP server bind error!"<<std::endl;
            return false;
        }
        
        char buffer[1024];
        ssize_t received;
        while (1)
        {
            socklen_t client_len = sizeof(client_socket);
            if ((received = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *) &client_socket, &client_len)) < 0)
            {
                std::cout<<"start UDP server recvfrom error!"<<std::endl;
                exit(0);
            }
            buffer[received] = '\0';
            std::cout << "Client connected: "<< inet_ntoa(client_socket.sin_addr)<<"\t"<<ntohs(client_socket.sin_port)<<std::endl;
            std::cout << buffer<<std::endl;
        }
        return true;
    }
    
    bool XSocketUDP::startClient(const char* ip,int port)
    {
        std::cout<<"start UDP client..."<<std::endl;
        
        
        SOCKET sock;
        struct sockaddr_in server_socket;
        struct sockaddr_in client_socket;

        if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        {
            std::cout<<"create UDP client Socket error!"<<std::endl;
            return false;
        }
#ifdef SO_BROADCAST
        int one = 1;
        if(setsockopt(sock,SOL_SOCKET, SO_BROADCAST, &one, sizeof(one))==-1)
        {
            std::cout<<"start UDP client setsockopt error!"<<std::endl;
            return false;
        }
#endif
        bzero(&server_socket, sizeof(server_socket));
        server_socket.sin_family = AF_INET;
        server_socket.sin_port = htons(port);
        server_socket.sin_addr.s_addr= inet_addr(ip);
        
        char buffer[]="this is a UDP test message from client!";
        
        while(1)
        {
            if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0)
            {
                std::cout<<"start UDP client sendto error!"<<std::endl;
                exit(0);
            }
            break;
        }
        
        return true;
    }
    
}
XLIB_END
