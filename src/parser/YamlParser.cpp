//
//  YamlParser.cpp
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/5/18.
//

#include "parser/YamlParser.h"
#include "base/XString.h"
#include <iostream>
#include <fstream>

namespace xlib {

    YamlParser::YamlParser() noexcept {}
    
    YamlParser YamlParser::fromString(const std::string &yamlString){
        YamlParser parser;
        std::istringstream stream(yamlString);
        std::string line;
        int indent = 0;
        while (std::getline(stream, line)) {
            parser.parseLine(line, indent);
        }
        return parser;
    }

    YamlParser::YamlParser(std::string filename) {
        std::ifstream file(filename);
        std::string line;
        int indent = 0;
        while (std::getline(file, line)) {
            parseLine(line, indent);
        }
    }
    
    std::string YamlParser::getValue(const std::string& key) const {
        auto it = data.find(key);
        if (it == data.end()) {
            return "";
        }
        return it->second.toString();
    }

    void YamlParser::print() {
        for (auto& entry : data) {
            std::cout << entry.first << ": ";
            if (entry.second.isMap()) {
                std::cout << std::endl;
                for (auto& subentry : entry.second.getMap()) {
                    std::cout << "  " << subentry.first << ": " << subentry.second << std::endl;
                }
            } else if (entry.second.isArray()) {
                std::cout << std::endl;
                for (auto& subentry : entry.second.getArray()) {
                    std::cout << "  - " << subentry << std::endl;
                }
            } else {
                std::cout << entry.second.toString() << std::endl;
            }
        }
    }

    void YamlParser::parseLine(std::string line, int indent) {
        if (line.empty()) {
            return;
        }
        while (line[indent] == ' ') {
            indent++;
        }
        line = line.substr(indent);
        if (line.empty()) {
            return;
        }
        if (line[0] == '#') {
            return;
        }
        if (line[0] == '-') {
            data[currentKey].pushArray(line.substr(1));
        } else {
            std::size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                return;
            }
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            if(currentKey.empty()){
                currentKey = key;
            }else{
                int preIdx = indent/2;
                if(preIdx==0){
                    currentKey = key;
                }else{
                    std::size_t pos = XString::findNthOccurrence(currentKey, '.', preIdx);
                    if(pos!=std::string::npos){
                        std::string tmp = currentKey.substr(0,pos);
                        currentKey = tmp+"." + key;
                    }
                    else{
                        currentKey += "." + key;
                    }
                }
            }

            if (value.empty()) {
                // This is a nested map
                return;
            }
            value = value.substr(1);
            colonPos = value.find('#');
            if (colonPos != std::string::npos) {
                value.resize(colonPos+1);
            }
            // Add the key-value pair to the map
            std::string fullKey = currentKey;
            data[fullKey].pushValue(value);
        }
    }

    void YamlParser::YamlValue::pushValue(std::string value) {
        values.push_back(value);
    }

    void YamlParser::YamlValue::pushArray(std::string value) {
        arrayValues.push_back(value);
    }

    void YamlParser::YamlValue::pushMap(std::string key, std::string value) {
        mapValues[key] = value;
    }

    bool YamlParser::YamlValue::isMap() const {
        return !mapValues.empty();
    }

    bool YamlParser::YamlValue::isArray() const {
        return !arrayValues.empty();
    }

    std::vector<std::string> YamlParser::YamlValue::getArray() const {
        return arrayValues;
    }

    std::map<std::string, std::string> YamlParser::YamlValue::getMap() const {
        return mapValues;
    }

    std::string YamlParser::YamlValue::toString() const {
        if (values.empty()) {
            return "";
        }
        return values[0];
    }

    YamlParser::YamlValue::operator std::string() const {
        return toString();
    }

}
