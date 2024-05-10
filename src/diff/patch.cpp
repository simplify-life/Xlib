//
//  patch.cpp
//  com.xm.xlib
//
//  Created by apple on 2024/5/10.
//

#include "diff/patch.h"
#include <iostream>
#include <fstream>
#include <vector>
namespace xlib {
    namespace patch {
    
        std::vector<Difference> binary_difference(const std::string& old_file, const std::string& new_file) {
            std::ifstream f1(old_file, std::ios::binary);
            std::ifstream f2(new_file, std::ios::binary);

            std::vector<Difference> differences;

            if (!f1.is_open() || !f2.is_open()) {
                std::cerr << "Error opening files!" << std::endl;
                return differences;
            }

             size_t index = 0;
             uint8 byte1, byte2;
             while (f1.read(reinterpret_cast<char*>(&byte1), 1) && f2.read(reinterpret_cast<char*>(&byte2), 1)) {
                 if (byte1 != byte2) {
                     differences.push_back({index, false, byte2});
                 }
                 index++;
             }

             // Check if file1 has extra bytes
             while (f1.read(reinterpret_cast<char*>(&byte1), 1)) {
                 differences.push_back({index, true, 0}); // Mark as deleted
                 return differences;
             }

             // Check if file2 has extra bytes
             while (f2.read(reinterpret_cast<char*>(&byte2), 1)) {
                 differences.push_back({index, false, byte2});
                 index++;
             }

             return differences;
        }
    
        void create_patch(const std::string& patch_file, const std::vector<Difference>& differences) {
            std::ofstream out(patch_file, std::ios::binary);
            if (!out.is_open()) {
                std::cerr << "Error opening patch file!" << std::endl;
                return;
            }

            for (const auto& diff : differences) {
                out.write(reinterpret_cast<const char*>(&diff.position), sizeof(diff.position));
                out.write(reinterpret_cast<const char*>(&diff.is_deleted), sizeof(diff.is_deleted));
                if (!diff.is_deleted) {
                    out.write(reinterpret_cast<const char*>(&diff.modified), sizeof(diff.modified));
                }
            }
        }
    
        void apply_patch(const std::string& original_file, const std::string& patch_file, const std::string& output_file) {
            std::ifstream original(original_file, std::ios::binary);
            std::ifstream patch(patch_file, std::ios::binary);
            std::ofstream output(output_file, std::ios::binary);

            if (!original.is_open() || !patch.is_open() || !output.is_open()) {
                std::cerr << "Error opening files for patching!"<< std::endl;
                return;
            }

            std::vector<uint8> file_contents((std::istreambuf_iterator<char>(original)), std::istreambuf_iterator<char>());
            size_t position;
            bool is_deleted;
            uint8 modified_byte;
            size_t fileSize = 0;
            std::cout << "Applying patch..."<< std::endl;
            while (patch.read(reinterpret_cast<char*>(&position), sizeof(position)) &&
                       patch.read(reinterpret_cast<char*>(&is_deleted), sizeof(is_deleted))) {
                std::cout << "Position: "<< position << ", is_deleted: "<< is_deleted<< std::endl;

                if (is_deleted) {
                    if (position< file_contents.size()) {
                        file_contents.erase(file_contents.begin() + position);
                        fileSize = position -1;
                        break;
                    }
                } else {
                    patch.read(reinterpret_cast<char*>(&modified_byte), sizeof(modified_byte));
                    if (position >= file_contents.size()) {
                        file_contents.resize(position + 1);
                    }
                    file_contents[position] = modified_byte;
                    fileSize = position + 1;
                }
                // 跳过已经读取的字节
//                patch.ignore(sizeof(position) + sizeof(modified_byte));
            }

            std::cout << "Patch applied. Writing output file..."<< std::endl;
            auto data = file_contents.data();
            output.write(reinterpret_cast<const char*>(data), fileSize);
        }
    }
}
