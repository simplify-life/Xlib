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
#include <memory>

namespace xlib
{
    namespace sgf
    {

        enum class ParserState : int
        {
            SGF_START,
            NORMAL,
            IN_NODE
        };

        enum class TokenType : int
        {
            VARIATION_START,
            VARIATION_END,
            NODE_START,
            KEY,
            VALUE
        };

        class Token
        {
        public:
            TokenType type;
            std::string value;

            Token(TokenType t, const std::string &v) : type(t), value(v) {}
        };

        class Node
        {
        public:
            std::shared_ptr<Node> next;
            std::vector<std::shared_ptr<Node>> children;
            std::map<std::string, std::vector<std::string>> properties;

            Node() : next(nullptr) {}

            std::string propertiesString() const
            {
                std::string result;
                for (const auto &[key, values] : properties)
                {
                    result += "(" + key + ": ";
                    for (size_t i = 0; i < values.size(); ++i)
                    {
                        result += values[i];
                        if (i < values.size() - 1)
                            result += ", ";
                    }
                    result += ") ";
                }
                return result;
            }
            void print(int depth = 0) const {
                // 打印缩进
                for (int i = 0; i < depth; ++i) {
                    std::cout << "  ";
                }
                
                // 打印节点属性
                std::cout << "Node Properties: " << propertiesString() << std::endl;
                
                // 打印子节点
                if (!children.empty()) {
                    for (int i = 0; i < depth; ++i) {
                        std::cout << "  ";
                    }
                    std::cout << "Children:" << std::endl;
                    for (const auto& child : children) {
                        child->print(depth + 1);
                    }
                }
                
                // 打印下一个节点
                if (next) {
                    next->print(depth);
                }else{
                    for (int i = 0; i < depth; ++i) {
                        std::cout << "  ";
                    }
                    std::cout << "end branch"<< std::endl;
                }
            }
        };

        class Game
        {
        public:
            std::shared_ptr<Node> root;

            Game(std::shared_ptr<Node> r) : root(r) {}
            
            void print() const {
                std::cout << "Game Tree:" << std::endl;
                if (root) {
                    root->print();
                } else {
                    std::cout << "Empty game" << std::endl;
                }
            }
        };

        class SgfParser
        {
        public:
            Game parse(const std::string &sgf);

        private:
            std::shared_ptr<Node> parseNode(const std::vector<Token> &tokens);
            std::vector<Token> tokenize(const std::string &sgf);
        };
    }
}

#endif /* SgfParser_h */
