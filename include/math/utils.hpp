#ifndef MATH_UTILS_HPP  
#define MATH_UTILS_HPP

#include <random>

namespace math_utils {
    inline constexpr uint32_t DEFAULT_RNG_SEED = 3141592653;
}

class RandomNumberGenerator {
    private:
        std::mt19937 mt_engine;
    public:
        RandomNumberGenerator(uint32_t seed = math_utils::DEFAULT_RNG_SEED);
        
        float uniform_draw(float min, float max);
};

#endif