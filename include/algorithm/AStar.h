//
//  AStar.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/19.
//

#ifndef AStar_h
#define AStar_h

#include<vector>

namespace xlib {
    
    //通衢
    const int A_STAR_EMPTY = 0;
    //障碍物
    const int A_STAR_OBSTACLE = 1;
    
    /**
      A* 寻路算法
     @param mapArr 地图
     @param w 地图宽度
     @param start 开始点
     @param end 终点
     @return 返回可达路径
     */
    std::vector<int> AStarSearch(std::vector<int>& mapArr,int w, int start, int end);
}

#endif /* AStar_h */
