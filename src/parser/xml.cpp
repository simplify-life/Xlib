//
//  xml.cpp
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/13.
//

#include "parser/xml.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

namespace xlib {
    
    XMLParser::XMLParser():rootNode(nullptr) {}
    XMLParser::~XMLParser() {
        if (rootNode != nullptr) {
            deleteNode(rootNode);
        }
    }

    XMLNode* XMLParser::parse(const std::string& xmlStr) {
        std::istringstream stream(xmlStr);
        std::stack<XMLNode*> nodeStack;
        if (rootNode != nullptr) {
            deleteNode(rootNode);
        }
        rootNode = nullptr;
        std::string line;
        while (getline(stream, line)) {
            trim(line);
            if (line.empty()) {
                continue;
            }
            
            if (line[0] == '<') {
                if (line[1] == '/') {
                    nodeStack.pop();
                } else {
                    XMLNode* node = parseNode(line);
                    if (nodeStack.empty()) {
                        rootNode = node;
                    } else {
                        nodeStack.top()->children.push_back(node);
                    }
                    nodeStack.push(node);
                }
            } else {
                if (!nodeStack.empty()) {
                    nodeStack.top()->value = line;
                }
            }
        }
        return rootNode;
    }

    void XMLParser::trim(std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            str = str.substr(start);
        }
        size_t end = str.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            str = str.substr(0, end + 1);
        }
    }

    XMLNode* XMLParser::parseNode(const std::string& line) {
        XMLNode* node = new XMLNode();
        size_t pos = line.find_first_of(" \t\r\n>");
        if (pos == std::string::npos) {
            std::cerr << "Invalid node: " << line << std::endl;
            delete node;
            return nullptr;
        }
        node->name = line.substr(1, pos - 1);
        
        // Get node value
        size_t valueStart = line.find_first_of(">");
        if (valueStart != std::string::npos) {
            size_t valueEnd = line.find_last_of("<");
            if (valueEnd != std::string::npos) {
                node->value = line.substr(valueStart + 1, valueEnd - valueStart - 1);
            }
        }

        size_t len = line.length();
        while (pos < len) {
            size_t keyStart = line.find_first_not_of(" \t\r\n", pos);
            if (keyStart == std::string::npos) {
                break;
            }
            if (line[keyStart] == '/') {
                break;
            }
            size_t keyEnd = line.find_first_of(" \t\r\n=", keyStart);
            if (keyEnd == std::string::npos) {
                break;
            }
            size_t valueStart = line.find_first_of("=", keyEnd);
            if (valueStart == std::string::npos) {
                break;
            }
            valueStart = line.find_first_not_of(" \t\r\n", valueStart + 1);
            if (valueStart == std::string::npos) {
                break;
            }
            char quote = line[valueStart];
            if (quote != '\'' && quote != '\"') {
                break;
            }
            size_t valueEnd = line.find(quote, valueStart + 1);
            if (valueEnd == std::string::npos) {
                break;
            }
            node->attributes.push_back(make_pair(line.substr(keyStart, keyEnd - keyStart), line.substr(valueStart + 1, valueEnd - valueStart - 1)));
            pos = valueEnd + 1;
        }
        return node;
    }

    std::string nodeToString(xlib::XMLNode* node) {
        std::stringstream ss;
        ss<<node->name<<"="<<node->value<<"\n";
        for (auto& attribute : node->attributes) {
            ss << node->name << "." <<attribute.first << " = " << attribute.second << "\n";
        }
        for (auto& child : node->children) {
            ss << nodeToString(child);
        }
        return ss.str();
    }

    void XMLParser::deleteNode(XMLNode* node) {
        for (auto& child : node->children) {
            deleteNode(child);
        }
        delete node;
    }
}
