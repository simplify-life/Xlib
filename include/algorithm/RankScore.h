#ifndef rankScore_h
#define rankScore_h

#include <vector>
#include <cmath>
#include <algorithm>
#include "math/Fraction.h"

namespace xlib {
    namespace algorithm {
        /**
         * ELO 评分系统
         * 用于计算玩家或团队相对实力的评分系统
         */
        class EloRating {
        public:
            /**
             * 计算 ELO 评分变化
             * @param playerRating 玩家当前评分
             * @param opponentRating 对手当前评分
             * @param result 比赛结果 (1.0 表示胜利, 0.5 表示平局, 0.0 表示失败)
             * @param kFactor K因子，决定评分变化的幅度 (默认值 32)
             * @return 评分变化值
             */
            static double calculateRatingChange(double playerRating, double opponentRating, double result, double kFactor = 32.0);
            
            /**
             * 计算 ELO 评分变化 (使用分数)
             * @param playerRating 玩家当前评分
             * @param opponentRating 对手当前评分
             * @param result 比赛结果 (1.0 表示胜利, 0.5 表示平局, 0.0 表示失败)
             * @param kFactor K因子，决定评分变化的幅度 (默认值 32)
             * @return 评分变化值 (分数)
             */
            static Fraction calculateRatingChangeFraction(double playerRating, double opponentRating, double result, double kFactor = 32.0);
            
            /**
             * 计算预期胜率
             * @param playerRating 玩家当前评分
             * @param opponentRating 对手当前评分
             * @return 预期胜率 (0.0 到 1.0 之间)
             */
            static double expectedScore(double playerRating, double opponentRating);
            
            /**
             * 批量计算 ELO 评分变化
             * @param playerRating 玩家当前评分
             * @param opponentRatings 对手评分列表
             * @param results 比赛结果列表 (1.0 表示胜利, 0.5 表示平局, 0.0 表示失败)
             * @param kFactor K因子，决定评分变化的幅度 (默认值 32)
             * @return 评分变化值
             */
            static double calculateRatingChangeBatch(double playerRating, const std::vector<double>& opponentRatings, const std::vector<double>& results, double kFactor = 32.0);
        };
    }
}

#endif //rankScore_h