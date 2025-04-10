#include "algorithm/RankScore.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace xlib {
    namespace algorithm {
        
        // ELO 评分系统实现
        
        double EloRating::expectedScore(double playerRating, double opponentRating) {
            return 1.0 / (1.0 + std::pow(10.0, (opponentRating - playerRating) / 400.0));
        }
        
        double EloRating::calculateRatingChange(double playerRating, double opponentRating, double result, double kFactor) {
            double expected = expectedScore(playerRating, opponentRating);
            return kFactor * (result - expected);
        }
        
        Fraction EloRating::calculateRatingChangeFraction(double playerRating, double opponentRating, double result, double kFactor) {
            // 使用分数计算 ELO 评分变化
            // 预期胜率: E = 1 / (1 + 10^((R2-R1)/400))
            // 评分变化: ΔR = K * (S - E)
            // 其中 S 是实际结果 (1.0, 0.5, 0.0)
            
            // 计算 (R2-R1)/400
            Fraction ratingDiff = Fraction(opponentRating - playerRating) / Fraction(400.0);
            
            // 计算 10^((R2-R1)/400)
            // 由于分数不支持直接计算指数，我们需要使用近似值
            // 这里我们使用 double 计算，然后转换为分数
            double expValue = std::pow(10.0, (opponentRating - playerRating) / 400.0);
            
            // 计算 1 / (1 + 10^((R2-R1)/400))
            Fraction expected = Fraction(1.0) / (Fraction(1.0) + Fraction(expValue));
            
            // 计算 K * (S - E)
            return Fraction(kFactor) * (Fraction(result) - expected);
        }
        
        double EloRating::calculateRatingChangeBatch(double playerRating, const std::vector<double>& opponentRatings, const std::vector<double>& results, double kFactor) {
            if (opponentRatings.size() != results.size()) {
                throw std::invalid_argument("Opponent ratings and results must have the same size");
            }
            
            double totalChange = 0.0;
            for (size_t i = 0; i < opponentRatings.size(); ++i) {
                totalChange += calculateRatingChange(playerRating, opponentRatings[i], results[i], kFactor);
            }
            
            return totalChange;
        }    
    }
}
