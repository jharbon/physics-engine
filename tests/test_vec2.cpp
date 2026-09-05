#include <math/Vec2.hpp>
#include <helpers.hpp>

#include <cassert>
#include <iostream>

void test_constructor() {
    Vec2 a(0.5, 2.2);
    Vec2 b;

    assert(compare_floats(a[0], 0.5));
    assert(compare_floats(a[1], 2.2));

    assert(compare_floats(b[0], 0.0));
    assert(compare_floats(b[1], 0.0));
}

void test_index() {
    Vec2 a(0.5, 2.2);

    bool negative_idx_caught = false;
    try {
        a[-1];
    }
    catch (...) {
        negative_idx_caught = true;
    }

    assert(negative_idx_caught);

    bool high_idx_caught = false;
    try {
        a[2];
    }
    catch (...) {
        high_idx_caught = true;
    }

    assert(high_idx_caught);

    assert(compare_floats(a[0], 0.5));
    assert(compare_floats(a[1], 2.2));
}

void test_const_index() {
    const Vec2 a(0.5, 2.2);

    bool can_read = true;
    try {
        a[0];
    }
    catch (...) {
        can_read = false;
    }
    try {
        a[1];
    }
    catch (...) {
        can_read = false;
    }
    
    assert(can_read);

    bool negative_idx_caught = false;
    try {
        a[-1];
    }
    catch (...) {
        negative_idx_caught = true;
    }

    assert(negative_idx_caught);

    bool high_idx_caught = false;
    try {
        a[2];
    }
    catch (...) {
        high_idx_caught = true;
    }

    assert(high_idx_caught);

    assert(compare_floats(a[0], 0.5));
    assert(compare_floats(a[1], 2.2));

}

void test_set_value() {
    Vec2 a(0.5, 2.2);
    Vec2 b(2.5, -0.2);
    Vec2 c(0.2, 0.0);

    a[0] = 3.0;
    a[1] = 5.0;

    b[0] = 0.0;
    b[1] = 0.0;

    c[0] = -1.5;
    c[1] = -4.0;

    assert(compare_floats(a[0], 3.0));
    assert(compare_floats(a[1], 5.0));

    assert(compare_floats(b[0], 0.0));
    assert(compare_floats(b[1], 0.0));

    assert(compare_floats(c[0], -1.5));
    assert(compare_floats(c[1], -4.0));
}

void test_magnitude() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);

    float a_mag = a.mag();
    float b_mag = b.mag();
    float c_mag = c.mag();
    float d_mag = d.mag();

    assert(compare_floats(a_mag, 2.2561));
    assert(compare_floats(b_mag, 0.2000));
    assert(compare_floats(c_mag, 0.2000));
    assert(compare_floats(d_mag, 3.4015));
}

void test_normalisation() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);
    Vec2 e;

    Vec2 a_norm = a.norm();
    Vec2 b_norm = b.norm();
    Vec2 c_norm = c.norm();
    Vec2 d_norm = d.norm();

    assert(compare_floats(a_norm[0], 0.2216));
    assert(compare_floats(a_norm[1], 0.9751));

    assert(compare_floats(b_norm[0], 0.0000));
    assert(compare_floats(b_norm[1], -1.0000));

    assert(compare_floats(c_norm[0], 1.0000));
    assert(compare_floats(c_norm[1], 0.0000));

    assert(compare_floats(d_norm[0], -0.9114));
    assert(compare_floats(d_norm[1], 0.4116));

    bool zero_norm_caught = false;
    try{
        e.norm();
    }
    catch (...) {
        zero_norm_caught = true;
    }

    assert(zero_norm_caught);
}

void test_multiply() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);

    Vec2 a_scaled = a * 2.0;
    Vec2 b_scaled = b * 1.0;
    Vec2 c_scaled = c * 0.5;
    Vec2 d_scaled = d * 0.0;

    assert(compare_floats(a_scaled[0], 1.0));
    assert(compare_floats(a_scaled[1], 4.4));

    assert(compare_floats(b_scaled[0], 0.0));
    assert(compare_floats(b_scaled[1], -0.2));

    assert(compare_floats(c_scaled[0], 0.1));
    assert(compare_floats(c_scaled[1], 0.0));

    assert(compare_floats(d_scaled[0], 0.0));
    assert(compare_floats(d_scaled[1], 0.0));
}

void test_divide() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);

    Vec2 a_scaled = a / 2.0;
    Vec2 b_scaled = b / 1.0;
    Vec2 c_scaled = c / 0.5;

    assert(compare_floats(a_scaled[0], 0.25));
    assert(compare_floats(a_scaled[1], 1.1));

    assert(compare_floats(b_scaled[0], 0.0));
    assert(compare_floats(b_scaled[1], -0.2));

    assert(compare_floats(c_scaled[0], 0.4));
    assert(compare_floats(c_scaled[1], 0.0));

    bool zero_div_caught = false;
    try {
        Vec2 d_scaled = d / 0.0;
    }
    catch (...) {
        zero_div_caught = true;
    }
    
    assert(zero_div_caught);
}

void test_addition() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);

    Vec2 e = a + b + c + d;
    assert(compare_floats(e[0], -2.4));
    assert(compare_floats(e[1], 3.4));

    e += a + b + c + d;
    assert(compare_floats(e[0], -4.8));
    assert(compare_floats(e[1], 6.8));
}

void test_subtraction() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(-3.1, 1.4);

    Vec2 e = a - b - c - d;
    assert(compare_floats(e[0], 3.4));
    assert(compare_floats(e[1], 1.0));
}

void test_dot_product() {
    Vec2 a(0.5, 2.2);
    Vec2 b(0.0, -0.2);
    Vec2 c(0.2, 0.0);
    Vec2 d(3.1, 1.4);

    float a_dot_b = a.dot(b);
    float c_dot_d = c.dot(d);

    assert(compare_floats(a_dot_b, -0.44));
    assert(compare_floats(c_dot_d, 0.62));
}

int main(int argc, char* argv[]) {
    run_test("test_constructor", test_constructor);
    run_test("test_index", test_index);
    run_test("test_const_index", test_const_index);
    run_test("test_set_value", test_set_value);
    run_test("test_magnitude", test_magnitude);
    run_test("test_normalisation", test_normalisation);
    run_test("test_multiply", test_multiply);
    run_test("test_divide", test_divide);
    run_test("test_addition", test_addition);
    run_test("test_subtraction", test_subtraction);
    run_test("test_dot_product", test_dot_product);

    std::cout << "All test_vec2.cpp tests ran successfully" << std::endl;
}