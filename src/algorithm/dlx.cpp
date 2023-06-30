//
//  dlx.cpp
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/30.
//

#include "algorithm/dlx.h"
#include <stack>
#include <cmath>
#include <iostream>

namespace xlib
{

    Dlx::Dlx(std::vector<std::vector<int>> &data)
    {
        answer = std::vector<int>(data.size(), -1);
        head = new DlxNode();
        head->name = "h";
        int lenX = data[0].size();
        int lenY = data.size();

        columnNodes = std::vector<DlxNode *>(lenX);
        for (int i = 0; i < lenX; i++)
        {
            columnNodes[i] = new DlxNode();
            columnNodes[i]->left = (i > 0) ? columnNodes[i - 1] : head;
            columnNodes[i]->left->right = columnNodes[i];
            columnNodes[i]->name = "c" + std::to_string(i);
        }

        columnNodes[lenX - 1]->right = head;

        for (int i = 0; i < lenY; i++)
        {
            for (int j = 0; j < lenX; j++)
            {
                if (data[i][j] != 0)
                {
                    int row = i + 1;
                    DlxNode *left = lastRowNode(row);
                    DlxNode *right = firstRowNode(row);
                    DlxNode *node = createNode("c" + std::to_string(j) + "-" + std::to_string(i), row, columnNodes[j]);
                    node->up = lastDownNode(j);
                    node->up->down = node;
                    node->down = columnNodes[j];
                    columnNodes[j]->up = node;
                    if (left != nullptr)
                    {
                        node->left = left;
                        node->left->right = node;
                    }
                    if (right != nullptr)
                    {
                        node->right = right;
                        node->right->left = node;
                    }
                }
            }
        }
    }

    DlxNode *Dlx::createNode(std::string name, int row, DlxNode *&column)
    {
        DlxNode *node = new DlxNode();
        node->name = name;
        node->row = row;
        node->column = column;
        column->size++;
        return node;
    }

    DlxNode *Dlx::lastDownNode(int column)
    {
        DlxNode *node = columnNodes[column];
        while (node->down != columnNodes[column])
        {
            node = node->down;
        }
        return node;
    }

    DlxNode *Dlx::lastRowNode(int row)
    {
        int maxX = columnNodes.size();
        for (int j = maxX - 1; j >= 0; j--)
        {
            DlxNode *node = columnNodes[j];
            while (node->down != columnNodes[j])
            {
                node = node->down;
                if (node->row == row)
                {
                    return node;
                }
                if (node->row > row)
                {
                    break;
                }
            }
        }
        return nullptr;
    }

    DlxNode *Dlx::firstRowNode(int row)
    {
        int maxX = columnNodes.size();
        for (int i = 0; i < maxX; i++)
        {
            DlxNode *node = columnNodes[i];
            while (node->down != columnNodes[i])
            {
                node = node->down;
                if (node->row == row)
                {
                    return node;
                }
                if (node->row > row)
                {
                    break;
                }
            }
        }
        return nullptr;
    }

    void Dlx::cover(DlxNode *node)
    {
        node->right->left = node->left;
        node->left->right = node->right;
        for (DlxNode *i = node->down; i != node; i = i->down)
        {
            for (DlxNode *j = i->right; j != i; j = j->right)
            {
                j->down->up = j->up;
                j->up->down = j->down;
                j->column->size--;
            }
        }
    }

    void Dlx::uncover(DlxNode *node)
    {
        for (DlxNode *i = node->up; i != node; i = i->up)
        {
            for (DlxNode *j = i->left; j != i; j = j->left)
            {
                j->down->up = j->up->down = j;
                j->column->size++;
            }
        }
        node->right->left = node->left->right = node;
    }

    bool Dlx::dancing(int d)
    {
        if (this->head->right == this->head)
            return true;
        DlxNode *col = this->head->right;

        for (DlxNode *node = this->head->right; node != this->head; node = node->right)
        {
            if (node->size < col->size)
            {
                col = node;
            }
        }
        if (col->size < 1)
        {
            return false;
        }
        this->cover(col);
        for (DlxNode *node = col->down; node != col; node = node->down)
        {
            this->answer[d] = node->row;
            for (DlxNode *nj = node->right; nj != node; nj = nj->right)
                this->cover(nj->column);
            if (this->dancing(d + 1))
                return true;
            for (DlxNode *nj = node->left; nj != node; nj = nj->left)
                this->uncover(nj->column);
        }
        this->uncover(col);
        return false;
    }

    std::string solveStandardSudoku(std::string subject)
    {
        auto getArr = [&](int gongW, int gongH, int i, int v, int gong)
        {
            int x = i % gong;
            int y = i / gong;
            std::vector<int> arr(gong * gong * 4, 0);
            int c1 = i;
            int c2 = gong * gong + x * gong + v - 1;
            int c3 = gong * gong * 2 + y * gong + v - 1;
            int gongX = x / gongW;
            int gongY = y / gongH;
            int gongP = gongX + gongY * (gong / gongW);
            int c4 = gong * gong * 3 + gongP * gong + v - 1;
            arr[c1] = 1;
            arr[c2] = 1;
            arr[c3] = 1;
            arr[c4] = 1;
            return arr;
        };

        auto getWH = [&](int g)
        {
            std::vector<int> wh = {g, 1};
            for (int i = 2; i <= g; i++)
            {
                int j = g / i;
                if (i > j)
                    return wh;
                if (g % i == 0)
                {
                    if (i + j < wh[0] + wh[1])
                    {
                        wh[0] = j;
                        wh[1] = i;
                    }
                }
            }
            return wh;
        };
        // 将subject字符串转换为二维数组
        std::vector<std::vector<int>> sudoArr;
        std::vector<std::vector<int>> rowArr;
        int gong = sqrt(subject.size());
        auto wh = getWH(gong);
        int gongW = wh[0];
        int gongH = wh[1];
        for (int i = 0; i < gong * gong; i++)
        {
            int digit = subject[i] - '0';
            if (digit == 0)
            {
                for (int j = 1; j <= gong; j++)
                {
                    sudoArr.push_back(getArr(gongW, gongH, i, j, gong));
                    rowArr.push_back({i, j});
                }
            }
            else
            {
                sudoArr.push_back(getArr(gongW, gongH, i, digit, gong));
                rowArr.push_back({i, digit});
            }
        }

        // 创建DLX对象并解决数独问题
        xlib::Dlx dlx(sudoArr);
        bool success = dlx.dancing(0);
        if (success)
        {
            // 将解决方案转换为字符串形式
            std::vector<int> ansA = std::vector<int>(gong * gong, 0);
            std::string solution = "";
            for (int i = 0; i < dlx.answer.size(); i++)
            {
                if (dlx.answer[i] > 0)
                {
                    auto a = rowArr[dlx.answer[i] - 1];
                    ansA[a[0]] = a[1];
                }
            }
            for (int i = 0; i < ansA.size(); i++)
            {
                solution += std::to_string(ansA[i]);
            }
            return solution;
        }
        return "";
    }
}
