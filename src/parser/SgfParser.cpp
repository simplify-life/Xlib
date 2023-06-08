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

    
        Parser::Parser() noexcept :_currentNode(nullptr),_parse(false),_parseStr(std::string()){};
            
        Parser::~Parser(){
            backToRoot();
            safeDeleteNode(_currentNode);
        };

        void Parser::parseProperty(){
            if(_parseStr.empty()){
                return;
            }
            std::cout<<"start parse: "<<_parseStr<<std::endl;
            std::size_t pos = _parseStr.find('[');
            std::string currentKey;
            std::string currentValue;
            while (pos!=std::string::npos) {
                if(currentKey.empty()){
                    currentKey = _parseStr.substr(0,pos);
                    std::size_t s = currentKey.find_first_not_of(" ");
                    if(std::string::npos == s){
                        s = 0;
                    }
                    std::size_t e = currentKey.find_last_not_of(" ");
                    if(std::string::npos==e){
                        e = currentKey.size();
                    }
                    currentKey = currentKey.substr(s,e+1);
                }
                std::string line = std::string(_parseStr);
                _parseStr = _parseStr.substr(pos+1);
                std::size_t end = _parseStr.find(']');
                std::size_t next = _parseStr.find('[');
                if(end!=std::string::npos){
                    std::string value = _parseStr.substr(0,end);
                    _parseStr = _parseStr.substr(end+1);
                    pos = _parseStr.find('[');
                    std::size_t start_branch = _parseStr.find_first_of("(;");
                    std::size_t end_branch = _parseStr.find_first_of(')');
                    std::size_t next_node = _parseStr.find_first_of(';');
                    //判断一下是否解析结束
                    bool parseEnd = pos> start_branch || pos > end_branch || pos> next_node;
                    if(parseEnd) pos = std::string::npos;
                    currentValue = currentValue.empty() ? value: currentValue.append(",").append(value);
                    if(currentKey=="B" || currentKey=="W"){
                        _currentNode->color = currentKey.back();
                        _currentNode->coord = currentValue;
                        std::cout<<"move:"<<currentKey<<"->"<<currentValue<<std::endl;
                        currentKey.clear();
                        currentValue.clear();
                        continue;
                    }
                    bool isEnd = next==std::string::npos || (next != end+1);
                    if(isEnd){
                        _currentNode->properties.insert(std::pair<std::string, std::string>(currentKey,currentValue));
                        std::cout<<currentKey<<"->"<<currentValue<<std::endl;
                        currentKey.clear();
                        currentValue.clear();
                        continue;
                    }
                    
                }else{
                    std::cout<<"stop parse :"<< _parseStr <<std::endl;
                    _parse = true;
                    _parseStr = line;
                    return;
                }
            }
            std::string line = std::string(_parseStr);
            _parseStr.clear();
            _parse = false;
            bool first = false;
            parseLine(line, first);
        }
    
        void Parser::parseLine(std::string &line, bool& first){
            if( _currentNode==nullptr || line.empty()){
                return;
            }
            if(first){
                _parseStr.clear();
                _parse = false;
                std::size_t start = line.find_first_of("(;");
                if(std::string::npos==start) return;
                first = false;
                line = line.substr(start+2);
                std::size_t pos = line.find('['); //pos 必须在一行
                _parseStr.append(line);
                if(std::string::npos==pos){
                    return;
                }else{
                    parseProperty();
                    return;
                }
            }
            if(_parse){
                //继续解析
                _parseStr.append(line);
                parseProperty();
                return;
            }
            std::size_t start_branch = line.find_first_of("(;");
            std::size_t end_branch = line.find_first_of(')');
            std::size_t next_node = line.find_first_of(';');
            if(end_branch<start_branch && end_branch<next_node){
                //end_branch
                line = line.substr(end_branch+1);
                if(_currentNode->parent==nullptr){
                    //search parent node
                    while(_currentNode->front!=nullptr){
                        _currentNode = _currentNode->front;
                        if(_currentNode->parent!=nullptr){
                            _currentNode = _currentNode->parent;
                            return parseLine(line, first);
                        }
                    }
                    return;
                }
                _currentNode = _currentNode->parent;
                return parseLine(line, first);
            }
            if(start_branch<next_node){
                //新分支
                Node* child = new Node();
                _currentNode->children.push_back(child);
                child->parent = _currentNode;
                _currentNode = child;
                
                line = line.substr(start_branch+2);
                std::size_t pos = line.find('['); //pos 必须在一行
                _parseStr.append(line);
                if(std::string::npos==pos){
                    return;
                }else{
                    parseProperty();
                    return;
                }
            }
            
            if(std::string::npos!=next_node){
                Node* next = new Node();
                next->front = _currentNode;
                _currentNode->next = next;
                _currentNode = next;
                line = line.substr(start_branch+1);
                std::size_t pos = line.find('['); //pos 必须在一行
                _parseStr.append(line);
                if(std::string::npos==pos){
                    return;
                }else{
                    parseProperty();
                    return;
                }
            }
        }
    
        Node* Parser::parseSgf(const std::string& sgfStr){
            std::istringstream stream(sgfStr);
            std::string line;
            if(_currentNode != nullptr){
                safeDeleteNode(_currentNode);
            }
            _currentNode = new Node();
            _currentNode->parent = nullptr;
            bool first = true;
            while (std::getline(stream, line)) {
                if (line.empty()) {
                    continue;
                }
                parseLine(line,first);
            }
            return _currentNode;
        }
    
        void Parser::backToRoot(){
            if(_currentNode->front!=nullptr){
                _currentNode = _currentNode->front;
                backToRoot();
            }
            if(_currentNode->parent!=nullptr){
                _currentNode = _currentNode->parent;
                backToRoot();
            }
        }
            
        void traverseNodeMove(Node* node,std::vector<std::string>& result,std::string mStr){
            if(!node->coord.empty()){
                mStr.append(1,';');
                mStr.append(1,node->color);
                mStr.append(1,'[');
                mStr.append(node->coord);
                mStr.append(1,']');
                auto it = node->properties.find("C");
                if(it!=node->properties.end()){
                    mStr.append(1,'C');
                    mStr.append(1,'[');
                    mStr.append(it->second);
                    mStr.append(1,']');
                }
            }
            bool end = true;
            if(node->next){
                end = false;
                traverseNodeMove(node->next,result,mStr);
            }
            for(auto n: node->children){
                end = false;
                traverseNodeMove(n,result,mStr);
            }
            if(end){
                if(!mStr.empty()) result.push_back(mStr);
            }
        }
    
        std::vector<std::string> Parser::getmoveList(){
            std::vector<std::string> v;
            //深度搜索
            std::string str;
            traverseNodeMove(_currentNode,v,str);
            return v;
        }
    }
}
