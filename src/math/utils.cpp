#include <math/utils.hpp>

RandomNumberGenerator::RandomNumberGenerator(uint32_t seed) 
        : mt_engine(seed) {}

float RandomNumberGenerator::uniform_draw(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(this->mt_engine);
}