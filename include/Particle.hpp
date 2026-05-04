#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <Vec2.hpp>

class Particle {
    private:
        Vec2 pos;
        Vec2 vel;
        Vec2 accel;

    public:
        Particle(
                const Vec2& init_pos,
                const Vec2& init_vel,
                const Vec2& init_accel
        );

        void update(float delta_t);

        Vec2 get_pos() const;
        Vec2 get_vel() const;
        Vec2 get_accel() const;
};

#endif