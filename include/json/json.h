#ifndef XJSON_H
#define XJSON_H

#include <string>
#include <vector>
#include <map>
namespace xlib{


    class JSON {
    public:
        enum Type {
            Null,
            Boolean,
            Number,
            String,
            Array,
            Object
        };
        static JSON parse(const std::string& jsonStr);
        JSON() noexcept;
        JSON(std::nullptr_t);
        JSON(bool value);
        JSON(int value);
        JSON(double value);
        JSON(const std::string& value);
        JSON(const char* value);
        JSON(const std::vector<JSON>& value);
        JSON(const std::map<std::string, JSON>& value);

        Type getType() const;

        bool isNull() const;
        bool isBoolean() const;
        bool isNumber() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        bool asBoolean() const;
        int asInt() const;
        double asDouble() const;
        std::string asString() const;
        std::vector<JSON> asArray() const;
        std::map<std::string, JSON> asObject() const;

        JSON& operator[](int index);
        const JSON& operator[](int index) const;
        const JSON& operator[](const std::string& key) const;

        std::string toString() const;

    private:
        
        static void parseValue(JSON& json, std::istringstream& iss);
        
        Type type_;
        union {
            bool boolean_;
            int int_;
            double double_;
            std::string* string_;
            std::vector<JSON>* array_;
            std::map<std::string, JSON>* object_;
        } value_;
    };
}
#endif
