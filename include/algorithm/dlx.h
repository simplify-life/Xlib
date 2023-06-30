
#ifndef dlx_h
#define dlx_h

#include<vector>
#include<string>

namespace xlib {

    class DlxNode {
        public:
            DlxNode* left;
            DlxNode* right;
            DlxNode* up;
            DlxNode* down;
            DlxNode* column;
            int row;
            int size;
            std::string name;

            DlxNode() {
                left = this;
                right = this;
                up = this;
                down = this;
                column = this;
                row = 0;
                size = 0;
            }
    };

    class Dlx{
        public:
            std::vector<int> answer;
            Dlx(std::vector<std::vector<int>>& data);
            bool dancing(int d);
        private:
            DlxNode* createNode(std::string name, int row, DlxNode*& column);
            DlxNode* lastDownNode(int column);
            DlxNode* lastRowNode(int row);
            DlxNode* firstRowNode(int row);
            void cover(DlxNode* node);
            void uncover(DlxNode* node);
        private:
            DlxNode* head;
            std::vector<DlxNode*> columnNodes;
    };

    //解决标准的方阵数独
    std::string solveStandardSudoku(std::string subject);
}

#endif /* dlx_h */
