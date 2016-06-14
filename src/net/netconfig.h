//----------------------------------------------//
//  netconfig.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/13.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef netconfig_h
#define netconfig_h


#include <thread>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include    <BaseTsd.h>
#include    <io.h>
#include    <WS2tcpip.h>
#include    <Winsock2.h>
#ifndef __SSIZE_T
#define __SSIZE_T
typedef SSIZE_T ssize_t;
#define	bzero(ptr,n)		memset(ptr, 0, n)
#endif // __SSIZE_T

#else

#include	<errno.h>
#include    <netdb.h>
#include    <unistd.h>
#include	<poll.h>
#include    <arpa/inet.h>
#include    <netinet/in.h>
#include	<sys/types.h>
#include    <sys/select.h>
#include    <sys/socket.h>
#include    <sys/un.h>
#include    <sys/ioctl.h>
#include	<sys/time.h>
#include	<sys/stat.h>
#include	<sys/uio.h>
#endif

/* OSF/1 actually disables recv() and send() in <sys/socket.h> */
#ifdef	__osf__
#undef	recv
#undef	send
#define	recv(a,b,c,d)	recvfrom(a,b,c,d,0,0)
#define	send(a,b,c,d)	sendto(a,b,c,d,0,0)
#endif

#endif /* netconfig_h */
