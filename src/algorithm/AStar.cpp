//
//  AStar.cpp
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/19.
//

#include "algorithm/AStar.h"
#include <cmath>

namespace xlib {

    int heuristic(int fromX,int fromY,int toX, int toY){
        return abs(fromX - toX) + abs(fromY - toY);
    }

    int findMinFScore(const std::vector<int>& openList, const std::vector<int>& fScore) {
        int minFScore = INT_MAX;
        int minNode = -1;
        
        for (int node : openList) {
            if (fScore[node] < minFScore) {
                minFScore = fScore[node];
                minNode = node;
            }
        }
        
        return minNode;
    }

    std::vector<int> reconstructPath(const std::vector<int>& parent, int current) {
        std::vector<int> path;
        
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        
        std::reverse(path.begin(), path.end());
        
        return path;
    }

    std::vector<int> getNeighbors(int current, int w) {
        std::vector<int> neighbors;
        
        // Calculate the coordinates of the current node
        int currentX = current % w;
        int currentY = current / w;
        
        // Check the neighboring nodes in the four cardinal directions (up, down, left, right)
        if (currentX > 0) {
            neighbors.push_back(current - 1); // Left neighbor
        }
        if (currentX < w - 1) {
            neighbors.push_back(current + 1); // Right neighbor
        }
        if (currentY > 0) {
            neighbors.push_back(current - w); // Up neighbor
        }
        if (currentY < w - 1) {
            neighbors.push_back(current + w); // Down neighbor
        }
        
        return neighbors;
    }

    std::vector<int> AStarSearch(std::vector<int>& mapArr, int w, int start, int end) {
        // 定义起点和终点的坐标
        int startX = start % w;
        int startY = start / w;
        int endX = end % w;
        int endY = end / w;
        
        // 定义开放列表和关闭列表
        std::vector<int> openList;
        std::vector<int> closeList;
        
        // 定义每个节点的代价和父节点
        std::vector<int> gScore(mapArr.size(), INT_MAX);
        std::vector<int> fScore(mapArr.size(), INT_MAX);
        std::vector<int> parent(mapArr.size(), -1);
        
        // 将起点加入开放列表
        openList.push_back(start);
        gScore[start] = 0;
        fScore[start] = heuristic(startX, startY, endX, endY);
        
        while (!openList.empty()) {
            // 在开放列表中找到 fScore 最小的节点
            int current = findMinFScore(openList, fScore);
            
            // 如果当前节点是终点，说明已经找到了最短路径
            if (current == end) {
                return reconstructPath(parent, current);
            }
            
            // 将当前节点从开放列表中移除，并加入关闭列表
            openList.erase(std::remove(openList.begin(), openList.end(), current), openList.end());
            closeList.push_back(current);
            
            // 获取当前节点的邻居节点
            std::vector<int> neighbors = getNeighbors(current, w);
            
            for (int neighbor : neighbors) {
                // 如果有障碍，跳过
                if(mapArr[neighbor]!=0) continue;
                // 如果邻居节点已经在关闭列表中，跳过
                if (std::find(closeList.begin(), closeList.end(), neighbor) != closeList.end()) {
                    continue;
                }
                // 计算从起点到邻居节点的代价
                int tentativeGScore = gScore[current] + 1;
                
                // 如果邻居节点不在开放列表中，或者新的代价更小，则更新代价和父节点
                if (std::find(openList.begin(), openList.end(), neighbor) == openList.end() || tentativeGScore < gScore[neighbor]) {
                    parent[neighbor] = current;
                    gScore[neighbor] = tentativeGScore;
                    fScore[neighbor] = gScore[neighbor] + heuristic(neighbor % w, neighbor / w, endX, endY);
                    
                    // 如果邻居节点不在开放列表中，将其加入开放列表
                    if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                        openList.push_back(neighbor);
                    }
                }
            }
        }
        
        // 如果开放列表为空，说明无法找到路径
        return std::vector<int>();
    }
}
