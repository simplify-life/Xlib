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

        Glicko2Player::Glicko2Player(double r, 
                  double rd_val , 
                  double vol )
        : rating(r), rd(rd_val), volatility(vol) {} 

        double Glicko2System::computeVariance(const std::vector<double>& g_phi_j, 
                          const std::vector<double>& E_values) const {
            double sum = 0.0;
            for (size_t i = 0; i < g_phi_j.size(); ++i) {
                sum += g_phi_j[i] * g_phi_j[i] * E_values[i] * (1.0 - E_values[i]);
            }
            if (sum <= 0) return std::numeric_limits<double>::max(); // 避免除零
            return 1.0 / sum;
        }

        double Glicko2System::computeDelta(double v, const std::vector<double>& g_phi_j,
                        const std::vector<double>& E_values,
                        const std::vector<double>& scores) const {
            double sum = 0.0;
            for (size_t i = 0; i < g_phi_j.size(); ++i) {
                sum += g_phi_j[i] * (scores[i] - E_values[i]);
            }
            return v * sum;
        }

        double Glicko2System::computeNewVolatility(double sigma, double phi, double v, double delta) const {
            // 初始检查
            if (std::isnan(delta) || std::isnan(phi) || std::isnan(v)) {
                return sigma; // 返回原值避免NaN传播
            }
            // 初始检查
            if (!std::isfinite(delta) || !std::isfinite(phi) || !std::isfinite(v)) {
                return sigma; // 返回原值避免传播无效值
            }

            // 限制 delta^2 - phi^2 - v 的范围
            double discriminant = delta * delta - phi * phi - v;
            if (discriminant > 1e10) discriminant = 1e10; // 防止对数运算溢出
            
            double a = log(sigma * sigma);
            double x0 = a;
            double x1;
            // 定义局部lambda函数来简化f()的调用
            auto f_local = [&](double x) {
                double ex = exp(x);
                double numerator = ex * (delta * delta - phi * phi - v - ex);
                double denominator = 2.0 * pow(phi * phi + v + ex, 2);
                return numerator / denominator - (x - a) / (tau * tau);
            };

            if (delta * delta > phi * phi + v) {
                x1 = log(delta * delta - phi * phi - v);
            } else {
                int k = 1;
                while (f_local(a - k * tau) >= 0) {
                    k++;
                }
                x1 = a - k * tau;
            }

            double f0 = f_local(x0);
            double f1 = f_local(x1);

            while (fabs(x1 - x0) > EPSILON) {
                double x2 = x0 + (x0 - x1) * f0 / (f1 - f0);
                if (std::isnan(x2)) {
                    std::cerr << "Iteration failed with x2=NaN" << std::endl;
                    break;
                }
                double f2 = f_local(x2);

                if (f2 * f1 <= 0) {
                    x0 = x1;
                    f0 = f1;
                } else {
                    f0 /= 2.0;
                }

                x1 = x2;
                f1 = f2;
            }
            // 最终检查
            if (!std::isfinite(x1) || x1 > 1000.0) {
                return DEFAULT_VOLATILITY; // 返回默认值如0.06
            }
            return exp(x1 / 2.0);
        }

        double Glicko2System::f(double x, double a, double phi, double v, double delta) const {
            double ex = exp(x);
            double numerator = ex * (delta * delta - phi * phi - v - ex);
            double denominator = 2.0 * pow(phi * phi + v + ex, 2);
            return numerator / denominator - (x - a) / (tau * tau);
        }

        Glicko2System::Glicko2System(double t) : tau(t) {}

        void Glicko2System::updatePlayer(Glicko2Player& player,
                        const std::vector<Glicko2Player>& opponents,
                        const std::vector<double>& scores) {
            if(!(opponents.size() == scores.size())){
                throw std::runtime_error("opponents size not equal scores size");
            }
            // Step 2: Convert to Glicko-2 scale
            double mu, phi;
            player.convertToGlicko2(mu, phi);
            std::cout << "[Step 2] mu=" << mu << ", phi=" << phi << std::endl;
            double sigma = player.volatility;

            // Prepare opponent data
            std::vector<double> mu_j(opponents.size());
            std::vector<double> phi_j(opponents.size());
            std::vector<double> g_phi_j(opponents.size());
            std::vector<double> E_values(opponents.size());

            for (size_t i = 0; i < opponents.size(); ++i) {
                double opp_mu, opp_phi;
                opponents[i].convertToGlicko2(opp_mu, opp_phi);
                mu_j[i] = opp_mu;
                phi_j[i] = opp_phi;
                g_phi_j[i] = g(phi_j[i]);
                E_values[i] = E(mu, mu_j[i], phi_j[i]);
            }

            // Step 3: Compute variance
            double v = computeVariance(g_phi_j, E_values);
            std::cout << "[Step 3] v=" << v << std::endl;
            // Step 4: Compute delta
            double delta = computeDelta(v, g_phi_j, E_values, scores);
            std::cout << "[Step 4] delta=" << delta << std::endl;
            // Step 5: Compute new volatility
            double sigma_prime = computeNewVolatility(sigma, phi, v, delta);
            std::cout << "[Step 5] sigma_prime=" << sigma_prime << std::endl;
            // Step 6: Update RD to pre-rating period value
            double phi_star = sqrt(phi * phi + sigma_prime * sigma_prime);
            std::cout << "[Step 6] phi_star=" << phi_star << std::endl;
            // Step 7: Update rating and RD
            double phi_prime = 1.0 / sqrt(1.0 / (phi_star * phi_star) + 1.0 / v);
            
            double sum = 0.0;
            for (size_t i = 0; i < opponents.size(); ++i) {
                sum += g_phi_j[i] * (scores[i] - E_values[i]);
            }
            double mu_prime = mu + phi_prime * phi_prime * sum;

            // Step 8: Convert back to original scale
            player.convertFromGlicko2(mu_prime, phi_prime);
            player.volatility = sigma_prime;
        }
   
    }
}
