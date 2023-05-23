//
//  YamlParser.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/5/18.
//

#ifndef YamlParser_h
#define YamlParser_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

namespace xlib {
    class YamlParser {
    public:
        YamlParser(std::string filename);
        std::string getValue(const std::string& key) const;
        void print();
        static YamlParser fromString(const std::string& yamlString);
    private:
        
        YamlParser() noexcept;
        
        void parseLine(std::string line, int indent);

        class YamlValue {
        public:
            void pushValue(std::string value);

            void pushArray(std::string value);

            void pushMap(std::string key, std::string value);

            bool isMap() const;

            bool isArray() const;

            std::vector<std::string> getArray() const;

            std::map<std::string, std::string> getMap() const;

            std::string toString() const;

            operator std::string() const;

        public:
            std::vector<std::string> values;
            std::vector<std::string> arrayValues;
            std::map<std::string, std::string> mapValues;
        };
        std::map<std::string, YamlValue> data;
        std::string currentKey;
    };
}

#endif /* YamlParser_h */
