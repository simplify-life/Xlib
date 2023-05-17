#ifndef Serializer_h
#define Serializer_h

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

namespace xlib {

    class Serializer {
    public:
        template<typename T>
        static void serialize(const T& obj, const std::string& filename) {
            std::ofstream ofs(filename, std::ios::binary);
            if (!ofs) {
                throw std::runtime_error("Failed to open file for writing");
            }
            serialize(obj, ofs);
        }

        template<typename T>
        static std::vector<char> serialize(const T& obj) {
            std::vector<char> buffer(sizeof(obj));
            serialize(obj, buffer.data());
            return buffer;
        }

        template<typename T>
        static void deserialize(T& obj, const std::string& filename) {
            std::ifstream ifs(filename, std::ios::binary);
            if (!ifs) {
                throw std::runtime_error("Failed to open file for reading");
            }
            deserialize(obj, ifs);
        }

        template<typename T>
        static T deserialize(const std::vector<char>& data) {
            T obj;
            deserialize(obj, data);
            return obj;
        }

    private:
        template<typename T>
        static void serialize(const T& obj, std::ostream& os) {
            os.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
        }

        template<typename T>
        static void serialize(const T& obj, char* buffer) {
            memcpy(buffer, &obj, sizeof(obj));
        }

        template<typename T>
        static void deserialize(T& obj, std::istream& is) {
            is.read(reinterpret_cast<char*>(&obj), sizeof(obj));
        }

        template<typename T>
        static void deserialize(T& obj, const std::vector<char>& data) {
            memcpy(&obj, data.data(), sizeof(obj));
        }
    };

}

#endif
