//
//  SgfParser.h
//  xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/5/19.
//

#ifndef SgfParser_h
#define SgfParser_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace xlib {
    namespace sgf{
    
        struct Node {
            std::map<std::string,std::string> properties;
            std::vector<Node*> children;
            char color;
            std::string coord;
            Node* parent;
            Node* front=nullptr;
            Node* next=nullptr;
            bool deleted = false;
        };
        
        class Parser{
            public:
                Parser() noexcept;
                ~Parser();
                Node* parseSgf(const std::string& sgfStr);
                std::vector<std::string> getmoveList();
            private:
                void parseProperty();
                void parseLine(std::string &line, bool& first);
                void backToRoot();
                std::string _parseStr;
                bool _parse;
                Node* _currentNode;
        };
    }
}

#endif /* SgfParser_h */
