//----------------------------------------------//
//  http.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/24.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef http_h
#define http_h
#include    "netconfig.h"
#include    "../XBase.h"
#include    <string>
#include    <vector>
#include    <unordered_map>
namespace xlib{
    
    namespace net{
        
        namespace http{
            
            using namespace std;
            
            
            #define HTTP_METHOD_MAP(XX)     \
            /* Request Methods */           \
            XX(DELETE,      "DELETE")       \
            XX(GET,         "GET")          \
            XX(HEAD,        "HEAD")         \
            XX(POST,        "POST")         \
            XX(PUT,         "PUT")          \


            //const data
            const char CR = '\r';
            const char LF = '\n';
            const char*	CRLF = "\r\n";
            
           
            #define HTTP_METHOD(n,s) n,
            enum class Method
            {
                HTTP_METHOD_MAP(HTTP_METHOD)
            };
            #undef HTTP_METHOD
            
            
            
            enum class Protocol
            {
                HTTP_10, HTTP_11
            };
            
            enum class Header
            {
                Accept, Accept_Charset , Accept_Encoding, Accept_Language ,
                Authorization , Connection , Content_Type ,
                Content_Length , Cookie , From , Host
            };
            /**
             request message:  
	     1.request line + 2.header + 3. empty line + 4. request data :
             
             1.   [Method][blank]   [url][blank]  [Protoclo] CRLF
             2.   Accept-Charset: utf-8
                  Content-Type: text/html
                  ...
             3.
             4.   xx=oo&ss=bb&...
             */
            
            /**
              request line
             */
            string getLine(const Method& method,const char* url,const Protocol& protocol)
            {
                stringstream getline;
                unordered_map<int,string> s_method ( {{(int)Method::GET,"GET"},{(int)Method::HEAD,"HEAD"},{(int)Method::POST,"POST"}} );
                unordered_map<int,string> s_protocol ({{(int)Protocol::HTTP_10,"HTTP/1.0"},{(int)Protocol::HTTP_11,"HTTP/1.1"}});
                getline<<s_method[(int)method]<<" "<<url<<" "<<s_protocol[(int)protocol]<<CRLF;
                return getline.str();
            }
            
            /**
              header
             */
            string getHeader(const Header& type,const char* value)
            {
                stringstream header;
                unordered_map<int,string> s_head ({
                    {(int)Header::Accept,"Accept:"},
                    {(int)Header::Accept_Charset,"Accept-Charset:"},
                    {(int)Header::Content_Type,"Content-Type:"},
                    {(int)Header::Authorization,"Authorization:"},
                    {(int)Header::Content_Length,"Content-Length:"},
                    {(int)Header::Host,"Host:"},
                    {(int)Header::Connection,"Connection:"}
                });
                header<<s_head[(int)type]<<" "<<value<<CRLF;
                return header.str();
            }
            
            string getHeader(unordered_map<int,string> m_head)
            {
                stringstream header;
                for(auto &e:m_head)
                {
                    header<<getHeader((Header)e.first,(e.second).c_str());
                }
                return header.str();
            }
            /**
                blank
             */
            string getBlank()
            {
                stringstream blank;
                blank<<CRLF;
                return blank.str();
            }
            
            /**
                data
             */
            template<class T>
            string getData(const char* type,T&& value)
            {
                stringstream data;
		if(type!=""&&value!="")
                data<<type<<"="<<value;
		else
		data<<"";
                return data.str();
            }
            
            /**
             request
             */
            string getRequest(const Method& method=Method::GET,const char* url="/History.html",const char* type = "",const char* value="")
            {
                stringstream request;
                request<<getLine(method,url,Protocol::HTTP_11)
		<<getHeader(Header::Accept,"*/*")
		<<getHeader(Header::Host,"www.w3.org")	
		<<getBlank()<<getData(type,value);
                return request.str();
            }
            
            
        }//end http
    }//end net
    
}//end xlib

#endif /* http_h */
