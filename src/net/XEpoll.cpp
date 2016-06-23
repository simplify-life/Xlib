#include "XEpoll.h"

#if EPOLL_SUPPORT
#include <signal.h>
#include <iostream>

XLIB_BEGAIN
namespace net{
	namespace epoll{


		 void IPV4(ipv4* addrV4, short protocolFamily, uint port, const std::string &addr)
   		 {
    		    bzero(addrV4, sizeof(addrV4));
    		    addrV4->sin_family = protocolFamily;
    		    addrV4->sin_port = htons(port);
    		    addrV4->sin_addr.s_addr=inet_addr(addr.c_str());
   		 }
	

		SOCKET epoll_socket(const _socket& data)
		{
			SOCKET sfd = socket(data.protocolFamily,data.socketType,data.protocol),flags=1;
			if(sfd==-1)	return -1;
			if (ioctl(sfd, FIONBIO, &flags) &&((flags = fcntl(sfd, F_GETFL, 0)) < 0 || fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0))
			{
				close(sfd);
				return -1;
			}
			return sfd;
		}
		
		void setNoBlock(SOCKET sock)
		{
			int opts;
			opts = fcntl(sock, F_GETFL);

			if(opts < 0)
			{
				perror("fcntl(sock, GETFL)");
				exit(1);
			}

			opts = opts | O_NONBLOCK;

			if(fcntl(sock, F_SETFL, opts) < 0)
			{
				perror("fcntl(sock, SETFL, opts)");
				exit(1);
			}
		}
	
	
		int epoll_Create()
		{
			int ret=epoll_create(MAX_DEFAULT_FDS);
			if(ret<0)
			{
				//epoll_create error
				return -1;
			} 
			return ret;	
		}
		

		/////////////class Epoll///////////////
	
		Epoll::Epoll():mSocket(INVALID_SOCKET),ep_fd(-1)
		{

		}
		
		Epoll::~Epoll()
		{
			close(mSocket);
			close(ep_fd);
		}
		
	        void Epoll::init(int port )
	        {
        	    if(-1==ep_fd) ep_fd = epoll_Create();
       		    if(-1==mSocket)
        	    {
                	_socket tcpSocket;
                	tcpSocket.protocolFamily = AF_INET;
                	tcpSocket.socketType = SOCK_STREAM;
                	tcpSocket.protocol = IPPROTO_TCP;
                
                	mSocket = epoll_socket(tcpSocket);
		    }
            	    IPV4(&_serverAddr, AF_INET, port, "0.0.0.0");
		    bzero(&ev,sizeof(ev));
           	    ev.data.fd = mSocket;
            	    ev.events = EPOLLIN | EPOLLET;
            	    epoll_ctl(ep_fd, EPOLL_CTL_ADD,mSocket, &ev);
            	    bind(mSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
            	    listen(mSocket, port);
        	}

		
		int Epoll::startServer(int port)
		{
			
			init(port);

			struct sockaddr_in clientaddr;socklen_t clilen = sizeof(clientaddr);
			SOCKET client_fd=-1,socket_fd=-1;
			int ndfs=-1;
			ssize_t n=0;
			char line[1024];
			while(1)
			{
				ndfs = epoll_wait(ep_fd,events,MAX_DEFAULT_FDS,1000);
				for(int i=0;i<ndfs;i++)
				{
					if(events[i].data.fd == mSocket)
					{
						client_fd = accept(mSocket, (struct sockaddr *)&clientaddr, &clilen);
						if(client_fd<0)
						{
							std::cout<<"accept error!"<<std::endl;
							exit(1);
						}
						setNoBlock(client_fd);
						char *str = inet_ntoa(clientaddr.sin_addr);
						printf("connect from %s\n", str);
						
						ev.data.fd = client_fd;
						ev.events = EPOLLIN | EPOLLET;
						epoll_ctl(ep_fd, EPOLL_CTL_ADD, client_fd, &ev);
					}			

					else if(events[i].events&EPOLLIN)
					{
						if((socket_fd=events[i].data.fd)<0) continue;
						if((n = net::Receive(socket_fd, line, 1024)) < 0)
						{
							if(errno == ECONNRESET)
							{
								close(socket_fd);
								events[i].data.fd = -1;
							}
							else
							{
								printf("readline error");
							}
						} else if(n == 0)
						{
							close(socket_fd);
							events[i].data.fd = -1;
						}
				
						printf("received data: %s\n", line);

						ev.data.fd = socket_fd;
						ev.events = EPOLLOUT | EPOLLET;
						epoll_ctl(ep_fd, EPOLL_CTL_MOD, socket_fd, &ev);
					}
					else if(events[i].events & EPOLLOUT)
		 			{
						socket_fd = events[i].data.fd;

						net::Send(socket_fd,line,sizeof(line));
						printf("written data: %s\n", line);

						ev.data.fd = socket_fd;
						ev.events = EPOLLIN | EPOLLET;
						epoll_ctl(ep_fd, EPOLL_CTL_MOD, socket_fd, &ev);
					}	
				}


			}
			return 0;
		}		
	}	//end epoll
}	//end net
XLIB_END

#endif

