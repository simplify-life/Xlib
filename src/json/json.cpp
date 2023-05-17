#include "json/json.h"
#include <sstream>

namespace xlib{


    JSON::JSON() noexcept : type_(Null) {}

    JSON::JSON(std::nullptr_t) : type_(Null) {}

    JSON::JSON(bool value) : type_(Boolean) {
        value_.boolean_ = value;
    }

    JSON::JSON(int value) : type_(Number) {
        value_.int_ = value;
    }

    JSON::JSON(double value) : type_(Number) {
        value_.double_ = value;
    }

    JSON::JSON(const std::string& value) : type_(String) {
        value_.string_ = new std::string(value);
    }

    JSON::JSON(const char* value) : type_(String) {
        value_.string_ = new std::string(value);
    }

    JSON::JSON(const std::vector<JSON>& value) : type_(Array) {
        value_.array_ = new std::vector<JSON>(value);
    }

    JSON::JSON(const std::map<std::string, JSON>& value) : type_(Object) {
        value_.object_ = new std::map<std::string, JSON>(value);
    }

    JSON::Type JSON::getType() const {
        return type_;
    }

    bool JSON::isNull() const {
        return type_ == Null;
    }

    bool JSON::isBoolean() const {
        return type_ == Boolean;
    }

    bool JSON::isNumber() const {
        return type_ == Number;
    }

    bool JSON::isString() const {
        return type_ == String;
    }

    bool JSON::isArray() const {
        return type_ == Array;
    }

    bool JSON::isObject() const {
        return type_ == Object;
    }

    bool JSON::asBoolean() const {
        if (type_ != Boolean) {
            throw std::runtime_error("JSON object is not a bool");
        }
        return value_.boolean_;
    }

    int JSON::asInt() const {
        if (type_ != Number) {
            throw std::runtime_error("JSON object is not a number");
        }
        return value_.int_;
    }

    double JSON::asDouble() const {
        if (type_ != Number) {
            throw std::runtime_error("JSON object is not a number");
        }
        return value_.double_;
    }

    std::string JSON::asString() const {
        if (type_ != String) {
            throw std::runtime_error("JSON object is not a string");
        }
        return *value_.string_;
    }

    std::vector<JSON> JSON::asArray() const{
        if (type_ != Array) {
            throw std::runtime_error("JSON object is not an array");
        }
        return *value_.array_;
    }

    std::map<std::string, JSON> JSON::asObject() const {
        if (!isObject()) {
            throw std::runtime_error("JSON object expected");
        }
        std::map<std::string, JSON> result;
        for (const auto& pair : *value_.object_) {
            result.insert({pair.first, pair.second});
        }
        return result;
    }

    JSON& JSON::operator[](int index) {
      if (type_ != Type::Array) {
        throw std::runtime_error("JSON object is not an array");
      }
      if (index < 0 || index >= value_.array_->size()) {
        throw std::out_of_range("JSON array index out of range");
      }
      return (*value_.array_)[index];
    }

    const JSON& JSON::operator[](int index) const {
      if (type_ != Type::Array) {
        throw std::runtime_error("JSON object is not an array");
      }
      if (index < 0 || index >= value_.array_->size()) {
        throw std::out_of_range("JSON array index out of range");
      }
      return (*value_.array_)[index];
    }

    const JSON& JSON::operator[](const std::string& key) const {
        static const JSON nullJSON;
        if (type_ != Object) {
            return nullJSON;
        }
        const std::map<std::string, JSON>& obj = asObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            const std::string& k = it->first;
            // 在这里使用key和value进行操作
            if(k==key){
                return it->second;
            }
        }
        return nullJSON;
    }

    std::string JSON::toString() const {
      std::ostringstream oss;
      switch (type_) {
        case Null:
          oss << "null";
          break;
        case Boolean:
          oss << (value_.boolean_ ? "true" : "false");
          break;
        case Number:
          oss << value_.double_;
          break;
        case String:
          oss << "\"" << *value_.string_ << "\"";
          break;
        case Array:
          oss << "[";
          for (size_t i = 0; i < value_.array_->size(); ++i) {
            if (i > 0) oss << ",";
            oss << (*value_.array_)[i].toString();
          }
          oss << "]";
          break;
        case Object:
          oss << "{";
          for (auto it = value_.object_->begin(); it != value_.object_->end(); ++it) {
            if (it != value_.object_->begin()) oss << ",";
            oss << "\"" << it->first << "\":" << it->second.toString();
          }
          oss << "}";
          break;
      }
      return oss.str();
    }

    JSON JSON::parse(const std::string& jsonStr) {
        JSON result;
        std::istringstream iss(jsonStr);
        parseValue(result, iss);
        return result;
    }

    void JSON::parseValue(JSON& json, std::istringstream& iss) {
        char c;
        if (!(iss >> c)) {
            return;
        }
        if (c == 'n') {
            char nullBuffer[4];
            iss.read(nullBuffer, 4);
            json = nullptr;
        } else if (c == 't') {
            char trueBuffer[4];
            iss.read(trueBuffer, 4);
            json = true;
        } else if (c == 'f') {
            char falseBuffer[5];
            iss.read(falseBuffer, 5);
            json = false;
        } else if (c == '"') {
            std::string str;
            std::getline(iss, str, '"');
            json = str;
        } else if (c == '[') {
            std::vector<JSON> vec;
            char next = ' ';
            while (iss >> next && next != ']') {
            if (next == ',') {
                continue;
            }
            iss.putback(next);
            JSON value;
            parseValue(value, iss);
            vec.push_back(value);
            }
            json = vec;
        } else if (c == '{') {
            std::map<std::string, JSON> map;
            char next = ' ';
            while (iss >> next && next != '}') {
                if (next == ',') {
                    continue;
                }
                iss.putback(next);
                std::string key;
                std::getline(iss >> std::ws, key, ':');
                JSON value;
                parseValue(value, iss);
                map[key] = value;
            }
            json = map;
        } else {
            iss.putback(c);
            double num;
            iss >> num;
            if (num == (int)num) {
                json = (int)num;
            } else {
                json = num;
            }
        }
    }
}
