#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <cmath>
#include <iostream>

const float FLOAT_EQ_TOLERANCE = 5e-5;

bool compare_floats(
        float f1,
        float f2,
        float tolerance = FLOAT_EQ_TOLERANCE
) {
    return std::abs(f1 - f2) < tolerance;
}

void run_test(const char* name, void(*fn)()) {
    std::cout << "Running " << name << "...\n";
    fn();
    std::cout << "Passed " << name << "\n\n";    
}

#endif