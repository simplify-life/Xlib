//----------------------------------------------//
//  XEpoll.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 2016/6/20.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XEpoll_h
#define XEpoll_h

#include	"netconfig.h"

#if	EPOLL_SUPPORT

#include	"../XBase.h"
//#include	<pcl.h>
#include	<string>

XLIB_BEGAIN

namespace net{

	namespace epoll{
		#define MAX_DEFAULT_FDS		20000
		#define STD_LISTEN_SIZE		2048
		
		void IPV4(ipv4* addrV4,short protocolFamily,uint port,const std::string& addr);
		SOCKET epoll_socket(const _socket& socketData); 
		void setNoBlock(SOCKET);
		int epoll_Create();
	
	class Epoll{
	
		public:	int startServer(int port);
			Epoll();		
			virtual ~Epoll();
		private:
			DISALLOW_COPY_AND_ASSIGN(Epoll);
			SOCKET mSocket;
			int ep_fd;
			ipv4 _serverAddr;
		};
	}


}


XLIB_END



#endif
#endif
