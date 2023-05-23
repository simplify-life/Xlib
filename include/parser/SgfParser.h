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
            std::string origin;
            std::map<std::string,std::string> properties;
            std::vector<Node*> children;
            std::string comment;
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
                void parseNodeByOrigin(Node* node);
                void parseLine(std::string &line, bool& first);
                void backToRoot();
                Node* currentNode;
        };
    }
}

#endif /* SgfParser_h */
