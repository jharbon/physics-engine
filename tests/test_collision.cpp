#include <collision.hpp>
#include <Particle.hpp>
#include <Vec2.hpp>
#include <helpers.hpp>

#include <cassert>
#include <iostream>

void test_particles_colliding() {
    // x-axis only
    Particle p1(0.2, 0.5, Vec2(-1.8, 0), Vec2(4.0, 0), Vec2(0, 0));
    Particle p2(2.0, 1.0, Vec2(-0.5, 0), Vec2(1.0, 0), Vec2(0, 0));
    Particle p3(1.0, 4.0, Vec2(4.5, 0), Vec2(-0.5, 0), Vec2(0, 0));

    assert(particles_colliding(p1, p2));
    assert(!particles_colliding(p1, p3));
    assert(particles_colliding(p2, p3));

    // y-axis only
    Particle p4(0.2, 0.5, Vec2(0, -1.8), Vec2(0, 4.0), Vec2(0, 0));
    Particle p5(2.0, 1.0, Vec2(0, -0.5), Vec2(0, 1.0), Vec2(0, 0));
    Particle p6(1.0, 4.0, Vec2(0, 4.5), Vec2(0, -0.5), Vec2(0, 0));

    assert(particles_colliding(p4, p5));
    assert(!particles_colliding(p4, p6));
    assert(particles_colliding(p5, p6));

    // x-axis AND y-axis
    Particle p7(0.2, 4.2, Vec2(-4.0, -3.0), Vec2(0.2, 2.0), Vec2(0, 0));
    Particle p8(2.0, 1.0, Vec2(0, 0), Vec2(0, 1.0), Vec2(0, 0));
    Particle p9(1.0, 4.0, Vec2(3.0, 4.0), Vec2(-1.1, -0.6), Vec2(0, 0));

    assert(particles_colliding(p7, p8));
    assert(!particles_colliding(p7, p9));
    assert(particles_colliding(p8, p9));
}

void test_solve_collision_vels() {
    // x-axis only
    std::pair<Vec2, Vec2> vels_x = solve_collision_vels(
        1.0,  // m1
        Vec2(3.5, 0),  // u1
        2.0,  // m2
        Vec2(-2.5, 0),  // u2
        Vec2(1, 0),  // col_norm
        Vec2(-0, 1)  // col_tan
    );

    // v1
    assert(compare_floats(vels_x.first[0], -4.5));
    assert(compare_floats(vels_x.first[1], 0));
    // v2
    assert(compare_floats(vels_x.second[0], 1.5));
    assert(compare_floats(vels_x.second[1], 0));

    // y-axis only
    std::pair<Vec2, Vec2> vels_y = solve_collision_vels(
        1.0,  
        Vec2(0, 3.5), 
        2.0,  
        Vec2(0, -2.5), 
        Vec2(0, 1), 
        Vec2(-1, 0)  
    );

    assert(compare_floats(vels_y.first[0], 0));
    assert(compare_floats(vels_y.first[1], -4.5));
    assert(compare_floats(vels_y.second[0], 0));
    assert(compare_floats(vels_y.second[1], 1.5));

    // x-axis AND y-axis
    std::pair<Vec2, Vec2> vels_xy = solve_collision_vels(
        3.0,  
        Vec2(4.0, 2.0), 
        1.0, 
        Vec2(-1.0, 3.0), 
        Vec2(0.6, 0.8), 
        Vec2(-0.8, 0.6) 
    );

    assert(compare_floats(vels_xy.first[0], 3.34));
    assert(compare_floats(vels_xy.first[1], 1.12));
    assert(compare_floats(vels_xy.second[0], 0.98));
    assert(compare_floats(vels_xy.second[1], 5.64));
}

void test_resolve_collision() {
    // x-axis only
    Particle p1(1.0, 1.0, Vec2(-1.5, 0), Vec2(3.5, 0), Vec2(0, 0));
    Particle p2(2.0, 1.0, Vec2(0.5, 0), Vec2(-2.5, 0), Vec2(0, 0));
    resolve_collision(p1, p2);

    assert(compare_floats(p1.get_vel()[0], -4.5));
    assert(compare_floats(p1.get_vel()[1], 0));
    assert(compare_floats(p2.get_vel()[0], 1.5));
    assert(compare_floats(p2.get_vel()[1], 0));
    
    // y-axis only
    Particle p3(1.0, 1.0, Vec2(0, -1.5), Vec2(0, 3.5), Vec2(0, 0));
    Particle p4(2.0, 1.0, Vec2(0, 0.5), Vec2(0, -2.5), Vec2(0, 0));
    resolve_collision(p3, p4);

    assert(compare_floats(p3.get_vel()[0], 0));
    assert(compare_floats(p3.get_vel()[1], -4.5));
    assert(compare_floats(p4.get_vel()[0], 0));
    assert(compare_floats(p4.get_vel()[1], 1.5));

    // x-axis AND y-axis
    Particle p5(3.0, 0.5, Vec2(0, 0), Vec2(4.0, 1.0), Vec2(0, 0));
    Particle p6(1.0, 0.5, Vec2(0.6, 0.8), Vec2(-2.0, -1.0), Vec2(0, 0));
    resolve_collision(p5, p6);

    assert(compare_floats(p5.get_vel()[0], 2.44));
    assert(compare_floats(p5.get_vel()[1], -1.08));
    assert(compare_floats(p6.get_vel()[0], 2.68));
    assert(compare_floats(p6.get_vel()[1], 5.24));
}

int main(int argc, char* argv[]) {
    run_test("test_particles_colliding", test_particles_colliding);
    run_test("test_solve_collision_vels", test_solve_collision_vels);
    run_test("test_resolve_collision", test_resolve_collision);

    std::cout << "All test_collision.cpp tests ran successfully" << std::endl;
}