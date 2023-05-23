//
//  SgfParser.cpp
//  xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/5/19.
//

#include "parser/SgfParser.h"
#include "base/XString.h"
#include <iostream>
#include <fstream>
#include <sstream>


namespace xlib {
    
    namespace sgf{
    
        void safeDeleteNode(Node*& node) {
            if (node == nullptr) return;
            if (node->deleted){
                return;
            }
            Node* next = node->next;
            while (next != nullptr) {
                next->front = nullptr;
                Node* tmp = next;
                safeDeleteNode(tmp);
                next = next->next;
            }
            for (auto n : node->children) {
                safeDeleteNode(n);
            }
            node->deleted = true;
            delete node;
            node = nullptr;
        }

    
        Parser::Parser() noexcept :currentNode(nullptr){};
            
        Parser::~Parser(){
//            backToRoot();
            safeDeleteNode(currentNode);
        };
    
        void Parser::parseNodeByOrigin(Node* node){
            std::string str = node->origin;
            if(str.empty()){
                return;
            }
            std::cout<<"start parse"<<std::endl;
            std::size_t pos = str.find('[');
            std::string currentKey;
            std::string currentValue;
            while (pos!=std::string::npos) {
                if(currentKey.empty()){
                    currentKey = str.substr(0,pos);
                    std::size_t s = currentKey.find_first_not_of(" ");
                    std::size_t e = currentKey.find_last_not_of(" ");
                    currentKey = currentKey.substr(s,e+1);
                }
                str = str.substr(pos+1);
                std::size_t end = str.find(']');
                std::size_t next = str.find('[');
                if(end!=std::string::npos){
                    std::string value = str.substr(0,end);
                    str = str.substr(end+1);
                    pos = str.find('[');
                    currentValue = currentValue.empty() ? value: currentValue.append(",").append(value);
                    if(currentKey=="B" || currentKey=="W"){
                        node->color = currentKey.back();
                        node->coord = currentValue;
                        std::cout<<"move:"<<currentKey<<"->"<<currentValue<<std::endl;
                        currentKey.clear();
                        currentValue.clear();
                        continue;
                    }
                    bool isEnd = next==std::string::npos || (next != end+1);
                    if(isEnd){
                        std::size_t s = currentValue.find_first_not_of(" ");
                        std::size_t e = currentValue.find_last_not_of(" ");
                        currentValue = currentValue.substr(s,e+1);
                        node->properties.insert(std::pair<std::string, std::string>(currentKey,currentValue));
                        std::cout<<currentKey<<"->"<<currentValue<<std::endl;
                        currentKey.clear();
                        currentValue.clear();
                        continue;
                    }
                    
                }else{
                    std::cout<<"stop parse error:"<< str <<std::endl;
                    return;
                }
            }
        }
    
        void Parser::parseLine(std::string &line, bool& first){
            if( currentNode==nullptr || line.empty()){
                return;
            }
            std::size_t start = line.find('(');
            std::size_t end = line.find(')');
            std::size_t nextPos = line.find(';');
            if(start==std::string::npos && end==std::string::npos){
                
                //no new branch and no end branch
                // move node calculate
                if(nextPos==std::string::npos){
                    currentNode->origin.append(line);
                    return;
                }else{
                    goto create_next_node;
                }
            }
            if(start!=std::string::npos && (end==std::string::npos || start < end)){
                
                if(nextPos==std::string::npos || start< nextPos){
                    //start new branch
                    
                    //1. old branch append
                    std::string old = line.substr(0,start);
                    currentNode->origin.append(old);
                    
                    //2. start new branch
                    // if is the first, don't creat new node
                    if(first){
                        first = false;
                        line = line.substr(start+1);
                        return parseLine(line ,first);
                    }else{
                        Node* child = new Node();
                        currentNode->children.push_back(child);
                        child->parent = currentNode;
                        currentNode = child;
                        line = line.substr(start+1);
                        return parseLine(line ,first);
                    }
                }else{
                    goto create_next_node;
                }
            }
            if(end!=std::string::npos &&(start==std::string::npos || start > end)){
                if(nextPos==std::string::npos || end< nextPos){
                    // end current branch
                    
                    //1. old branch append
                    std::string old = line.substr(0,end);
                    currentNode->origin.append(old);
    //                std::cout<<"branch :"<< currentNode->origin.c_str()<<std::endl;
                    
                    //2. parse branch
                    parseNodeByOrigin(currentNode);
                    
                    //3. back branch
                    line = line.substr(end+1);
                    if(currentNode->parent==nullptr){
                        //search parent node
                        while(currentNode->front!=nullptr){
                            currentNode = currentNode->front;
                            if(currentNode->parent!=nullptr){
                                currentNode = currentNode->parent;
                                return parseLine(line, first);
                            }
                        }
                        return;
                    }
                    currentNode = currentNode->parent;
                    return parseLine(line, first);
                }else{
                    goto create_next_node;
                }
            }
            
            create_next_node:
            {
                //1. parse old node
                std::string old = line.substr(0,nextPos);
                currentNode->origin.append(old);
                parseNodeByOrigin(currentNode);
                
                //2. create new node;
                Node* next = new Node();
                next->front = currentNode;
                currentNode->next = next;
                currentNode = next;
                line = line.substr(nextPos+1);
                return parseLine(line ,first);
            };
        }
    
        Node* Parser::parseSgf(const std::string& sgfStr){
            std::istringstream stream(sgfStr);
            std::string line;
            if(currentNode != nullptr){
//                backToRoot();
                safeDeleteNode(currentNode);
            }
            currentNode = new Node();
            currentNode->parent = nullptr;
            bool first = true;
            while (std::getline(stream, line)) {
                if (line.empty()) {
                    continue;
                }
                parseLine(line,first);
            }
            backToRoot();
            return currentNode;
        }
    
        void Parser::backToRoot(){
            if(currentNode->front!=nullptr){
                currentNode = currentNode->front;
                backToRoot();
            }
            if(currentNode->parent!=nullptr){
                currentNode = currentNode->parent;
                backToRoot();
            }
        }
    
        void nextToString(std::vector<std::string>& v,Node*& n, std::string origin, bool branch){
            if(nullptr==n){
                return;
            }
            if(!n->coord.empty()){
                //走子
                if(!origin.empty()){
                    origin.append(";");
                }
                origin.push_back(n->color);
                origin.append("[");
                origin.append(n->coord);
                origin.append("]");
                v.at(v.size()-1) = origin;
            }else{
                if(branch){
                    v.push_back(origin);
                }
            }
            std::string nextOri;
            if(v.size()>0){
                nextOri = v.back();
            }
            nextToString(v,n->next,nextOri,false);
            for(auto c:n->children){
                nextToString(v,c,nextOri,true);
            }
        }
    
        std::vector<std::string> Parser::getmoveList(){
            std::vector<std::string> v;
            nextToString(v,currentNode,"",false);
            return v;
        }
    }
}
