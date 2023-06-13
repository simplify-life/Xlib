//
//  xml.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/13.
//

#ifndef xml_h
#define xml_h
#include <string>
#include <vector>

namespace xlib {
    struct XMLNode {
        std::string name;
        std::string value;
        std::vector<std::pair<std::string, std::string>> attributes;
        std::vector<XMLNode*> children;
    };
    class XMLParser {
        public:
            XMLParser();
            ~XMLParser();
            XMLNode* parse(const std::string& xmlStr);
        private:
            void trim(std::string& str);
            XMLNode* parseNode(const std::string& line);
            void deleteNode(XMLNode* node);
            XMLNode* rootNode;
    };
    
    std::string nodeToString(xlib::XMLNode* node);
}

#endif /* xml_h */
