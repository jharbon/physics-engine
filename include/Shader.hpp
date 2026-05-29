#ifndef SHADER_HPP
#define SHADER_HPP

#include <unordered_map>
#include <string>

namespace uniform_names {
    const std::string WORLD_RIGHT = "world_right";
    const std::string OFFSET = "offset";
}

class Shader {
    private:
        unsigned int program;
        std::unordered_map<std::string, int> uniform_locs;

    public:
        Shader();
        ~Shader();

        void bind() const;
        void set_uniform_1f(const std::string& name, const float value) const;
        void set_uniform_2f(const std::string& name, const float value_1, const float value_2) const;
};

#endif