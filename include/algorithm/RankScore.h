#ifndef rankScore_h
#define rankScore_h

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
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

        const double PI = 3.14159265358979323846;
        const double CONVERSION_FACTOR = 173.7178;
        const double DEFAULT_RATING = 1500.0;
        const double DEFAULT_RD = 350.0;
        const double DEFAULT_VOLATILITY = 0.06;
        const double EPSILON = 0.000001;

        class Glicko2Player {
        public:
            double rating;
            double rd;
            double volatility;

            Glicko2Player(double r = DEFAULT_RATING, 
                        double rd_val = DEFAULT_RD, 
                        double vol = DEFAULT_VOLATILITY);
            inline void convertToGlicko2(double& mu, double& phi) const {
                mu = (rating - DEFAULT_RATING) / CONVERSION_FACTOR;
                phi = rd / CONVERSION_FACTOR;
            }

            inline void convertFromGlicko2(double mu, double phi) {
                rating = CONVERSION_FACTOR * mu + DEFAULT_RATING;
                rd = CONVERSION_FACTOR * phi;
            }
        };

        class Glicko2System {
        private:
            double tau;

            inline double g(double phi) const {
                return 1.0 / sqrt(1.0 + 3.0 * phi * phi / (PI * PI));
            }

            inline double E(double mu, double mu_j, double phi_j) const {
                return 1.0 / (1.0 + exp(-g(phi_j) * (mu - mu_j)));
            }

            double computeVariance(const std::vector<double>& g_phi_j, 
                                const std::vector<double>& E_values) const;

            double computeDelta(double v, const std::vector<double>& g_phi_j,
                            const std::vector<double>& E_values,
                            const std::vector<double>& scores) const;

            double computeNewVolatility(double sigma, double phi, double v, double delta) const;

            double f(double x, double a, double phi, double v, double delta) const;

            public:
                Glicko2System(double t = 0.5);

                void updatePlayer(Glicko2Player& player,
                                const std::vector<Glicko2Player>& opponents,
                                const std::vector<double>& scores);
        };

    }
}

#endif //rankScore_h
