//----------------------------------------------//
//  http.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/24.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//


//http Protocol as follow
/**
 request message:
 
 request line + header + blank line + request data
 
 1.   [Method][blank]   [url][blank]  [Protocol] CRLF
 2.   Accept-Charset: utf-8
 Content-Type: text/html
 ...
 3.
 4.   xx=oo&ss=bb&...
 */

/**
 response message(the same as request message):
 
 response line + header +blank line + html body
 
 1. [Protocol]  [status code]   [description]
 2.  ... : ...
 ...
 3.
 4.  <?xml version="1.0" encoding="iso-8859-1"?>
 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 <html xmlns="http://www.w3.org/1999/xhtml">
 <head>
 <body> ... </body>
 </html>
 */





#ifndef http_h
#define http_h
#include    "netconfig.h"
#include    "macro/XBase.h"
#include    <string.h>
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
            
            enum class StatusCode
            {
                Continue =                      100,
                Switch_Protocols =              101,
                
                OK =                            200,
                Created =                       201,
                Accepted =                      202,
                Non_Authoritative_Information = 203,
                No_Content =                    204,
                Reset_Content =                 205,
                Partial_Content =               206,
                
                Multiple_Choices =              300,
                Move_Permanently =              301,
                Found =                         302,
                See_Other =                     303,
                Not_Modifed =                   304,
                Use_Proxy =                     305,
                Temporary_Redirect =            307,
                
                //client error
                Bad_Req =                       400,
                Unautorized =                   401,
                Payment_Req =                   402,
                Forbidden =                     403,
                Not_Found =                     404,
                Method_Not_Allowed =            405,
                NotAcceptable =                 406,
                Proxy_Authentication_Req =      407,
                Req_TimeOut =                   408,
                Conflict =                      409,
                Gone =                          410,
                Length_Req =                    411,
                Precondition_Failed =           412,
                Req_Ebtity_TooLarge =           413,
                Req_Uri_TooLong =               414,
                Unsupported_Media_Type =        415,
                Req_Range_Not_Satisfiable =     416,
                Expectation_Failed =            417,
                
                //service error
                Internal_Server_Error =         500,
                Not_Implemented =               501,
                Bad_Gatway =                    502,
                Service_Unavailable =           503,
                GatWay_TimeOut =                504,
                Http_Version_Not_Supported =    505
            };
            
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
                ;
                if(strlen(type))
                    data<<type<<"="<<value;
                else
                    data<<"";
                return data.str();
            }
            
            /**
             request : this just a example
             */
            string getRequest(const Method& method=Method::GET,const char* url="/History.html",const char* type = "",const char* value="")
            {
                stringstream request;
                
                request<<getLine(method,url,Protocol::HTTP_11)
                
                       <<getHeader(Header::Accept,"*/*")
                       <<getHeader(Header::Host,"www.w3.org")
                
                       <<getBlank()
                
                       <<getData(type,value);
                return request.str();
            }
            
            
        }//end http
        
    }//end net
    
}//end xlib

#endif /* http_h */
