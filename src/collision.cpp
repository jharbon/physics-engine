#include <collision.hpp>

#include <cmath>

constexpr float EPSILON = 1e-6f;

bool particles_colliding(const Particle& p1, const Particle& p2) {
    // Compare relative squared distance to squared radii sum; avoids expensive sqrt
    Vec2 rel_pos = p1.get_pos() - p2.get_pos();
    float r1 = p1.get_radius();
    float r2 = p2.get_radius();
    if (rel_pos.dot(rel_pos) <= (r1 + r2)*(r1 + r2)) {
        return true;
    }
    else {
        return false;
    }
}

std::pair<Vec2, Vec2> solve_collision_vels(
        const float m1,
        const Vec2& u1,
        const float m2,
        const Vec2& u2,
        const Vec2& col_norm,
        const Vec2& col_tan
) {
    // Components of velocity vectors along collision tangent; these are unchanged
    float u1_tan = u1.dot(col_tan);
    float u2_tan = u2.dot(col_tan);
    
    // Components of velocity vectors along collision normal; these form 1D collision dynamics
    float u1_norm = u1.dot(col_norm);
    float u2_norm = u2.dot(col_norm);

    float M = m1 + m2;
    float v1_norm = (((m1 - m2) * u1_norm) + (2 * m2 * u2_norm)) / M;
    float v2_norm = ((2 * m1 * u1_norm) + ((m2 - m1) * u2_norm)) / M;

    // Use collision normal and tangent as unit basis vectors to get final particle velocities in world frame
    Vec2 v1 = (col_tan * u1_tan) + (col_norm * v1_norm);
    Vec2 v2 = (col_tan * u2_tan) + (col_norm * v2_norm);

    return {v1, v2};
}

void resolve_collision(Particle& p1, Particle& p2) {
    const Vec2 p1_pos = p1.get_pos();
    const Vec2 p2_pos = p2.get_pos();
    const Vec2 rel_pos = p1_pos - p2_pos;
    const float dist = rel_pos.mag();
    // Particles could be directly on top of each other in rare cases due to numerical issues
    if (dist < EPSILON) {
        return;
    }
    const Vec2 p1_vel = p1.get_vel();
    const Vec2 p2_vel = p2.get_vel();
    const Vec2 rel_vel = p1_vel - p2_vel;

    // Compute normalised relative position (collision normal) and vector perpendicular to this (collision tangent)
    const Vec2 collision_norm = rel_pos.norm();
    const Vec2 collision_tan(-collision_norm[1], collision_norm[0]); 

    // Check if particles are already moving apart along collision normal; avoids double resolution 
    if (rel_vel.dot(collision_norm) > 0) {
        return;
    }
    
    std::pair<Vec2, Vec2> vels = solve_collision_vels(
        p1.get_mass(),
        p1_vel,
        p2.get_mass(),
        p2_vel,
        collision_norm,
        collision_tan
    );

    // Set new velocities 
    p1.set_vel(vels.first);
    p2.set_vel(vels.second);

    float r1 = p1.get_radius();
    float r2 = p2.get_radius();
    float penetration_depth = r1 + r2 - dist;
    p1.set_pos(p1_pos + (collision_norm * 0.5 * penetration_depth));
    p2.set_pos(p2_pos - (collision_norm * 0.5 * penetration_depth));
}