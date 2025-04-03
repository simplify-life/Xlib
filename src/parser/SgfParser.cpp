//
//  SgfParser.cpp
//  xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/5/19.
//

#include "parser/SgfParser.h"
#include "base/XString.h"
#include <sstream>



namespace xlib {
    
    namespace sgf{
    
        Game SgfParser::parse(const std::string& sgf) {
            return Game(parseNode(tokenize(sgf)));
        }

        std::shared_ptr<Node> SgfParser::parseNode(const std::vector<Token>& tokens) {
            auto root = std::make_shared<Node>();
            auto current = root;
            std::vector<std::shared_ptr<Node>> parentStack;
            std::string currentKey;
            bool branchStart = false;

            for (size_t i = 1; i < tokens.size() - 1; ++i) {
                const auto& token = tokens[i];
                switch (token.type) {
                    case TokenType::VARIATION_START:
                        parentStack.push_back(current);
                        branchStart = true;
                        break;

                    case TokenType::VARIATION_END:
                        if (!parentStack.empty()) {
                            current = parentStack.back();
                            parentStack.pop_back();
                        }
                        break;

                    case TokenType::NODE_START:
                        if (branchStart) {
                            branchStart = false;
                            auto newNode = std::make_shared<Node>();
                            current->children.push_back(newNode);
                            current = newNode;
                        } else {
                            auto newNode = std::make_shared<Node>();
                            current->next = newNode;
                            current = newNode;
                        }
                        break;

                    case TokenType::KEY:
                        currentKey = token.value;
                        current->properties[currentKey] = std::vector<std::string>();
                        break;

                    case TokenType::VALUE:
                        if (!currentKey.empty()) {
                            current->properties[currentKey].push_back(token.value);
                        }
                        break;
                }
            }
            return root->next;
        }

        std::vector<Token> SgfParser::tokenize(const std::string& sgf) {
            std::vector<Token> tokens;
            ParserState state = ParserState::SGF_START;
            std::string currentKey;
            std::string currentValue;
            bool readValue = false;

            for (size_t i = 0; i < sgf.length(); ++i) {
                char c = sgf[i];
                switch (state) {
                    case ParserState::SGF_START:
                        if (c == '(') {
                            tokens.emplace_back(TokenType::VARIATION_START, std::string(1, c));
                            state = ParserState::NORMAL;
                        }
                        break;

                    case ParserState::NORMAL:
                        if (c == '(') {
                            tokens.emplace_back(TokenType::VARIATION_START, std::string(1, c));
                        } else if (c == ')') {
                            tokens.emplace_back(TokenType::VARIATION_END, std::string(1, c));
                        } else if (c == ';') {
                            tokens.emplace_back(TokenType::NODE_START, std::string(1, c));
                            state = ParserState::IN_NODE;
                        }
                        break;

                    case ParserState::IN_NODE:
                        if (readValue) {
                            if (c == ']') {
                                tokens.emplace_back(TokenType::VALUE, currentValue);
                                readValue = false;
                                currentValue.clear();
                            } else {
                                currentValue += c;
                            }
                        } else {
                            if (std::isalnum(c)) {
                                currentKey += c;
                            } else if (c == ';') {
                                tokens.emplace_back(TokenType::NODE_START, std::string(1, c));
                                state = ParserState::IN_NODE;
                                currentKey.clear();
                                currentValue.clear();
                            } else if (c == '(') {
                                tokens.emplace_back(TokenType::VARIATION_START, std::string(1, c));
                                state = ParserState::NORMAL;
                                currentKey.clear();
                                currentValue.clear();
                            } else if (c == ')') {
                                tokens.emplace_back(TokenType::VARIATION_END, std::string(1, c));
                                state = ParserState::NORMAL;
                                currentKey.clear();
                                currentValue.clear();
                            } else if (c == '[') {
                                if (!currentKey.empty()) {
                                    tokens.emplace_back(TokenType::KEY, currentKey);
                                    currentKey.clear();
                                }
                                readValue = true;
                            }
                        }
                        break;
                }
            }
            return tokens;
        }
    }
}
