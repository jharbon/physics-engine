#include <glad/glad.h>
#include <fmt/core.h>

#include <rendering/Shader.hpp>

#include <iostream>
#include <stdexcept>


namespace {
    void check_shader_compilation(
        const unsigned int shader,
        const char* name
    ) {
        int success;
        char log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            std::cout << "Shader compilation info log for '" << name << "':\n";
            glGetShaderInfoLog(shader, 512, NULL, log);
            std::cerr << log << std::endl;
            std::string err_msg = fmt::format("Shader source with name '{}' failed compilation", name);
            throw std::runtime_error(err_msg);
        }
}

    void check_program_linking(
            const unsigned int program,
            const char* name
    ) {
        int success;
        char log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        
        if (!success) {
            std::cout << "Shader program linking info log for '" << name << "':\n";
            glGetProgramInfoLog(program, 512, NULL, log);
            std::cerr << log << std::endl;
            std::string err_msg = fmt::format("Shader program with name '{}' failed linking", name);
            throw std::runtime_error(err_msg);
        }
    }
}

Shader::Shader()
    : program{0}, uniform_locs{} {
    // Shader to process vertices; runs once per vertex 
    std::string vertex_shader_source = fmt::format(
        R"(#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aUV;

        uniform float {};
        uniform vec2 {};
        uniform float {};

        out vec2 uv;

        void main() {{
            uv = aUV;

            // Scale vertex with particle radius, shift by offset, and project world coordinates to NDC
            vec2 ndc;
            ndc.x = (aPos.x * radius + offset.x) / world_right;
            ndc.y = aPos.y * radius + offset.y;

            gl_Position = vec4(ndc, 0.0, 1.0);
        }}
        )",
        uniform_names::RADIUS,
        uniform_names::OFFSET,
        uniform_names::WORLD_RIGHT
    );

    const char* vertex_shader_source_cstr = vertex_shader_source.c_str();

    // Shader to process fragments; each pixel is a fragment and this runs once per fragment
    const char* fragment_shader_source = 
        R"(#version 330 core
        in vec2 uv;
        out vec4 FragColor;

        // Define radius in UV space such that coloured circle has radius equal to half the quad dimension 
        const float radius = 0.5;
        const vec2 centre = vec2(0.5, 0.5);

        void main() {
            // Find frag point relative to centre and compute distance
            vec2 p = uv - centre;
            float dist = length(p);

            // Only colour fragments on circle
            if (dist > radius)
                discard;
                
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        )";

    // Compile and link shaders:

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source_cstr, NULL);
    glCompileShader(vertex_shader);

    check_shader_compilation(vertex_shader, "Vertex Shader");

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    check_shader_compilation(fragment_shader, "Fragment Shader");

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    check_program_linking(shader_program, "Shader Program");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    this->program = shader_program;
    this->uniform_locs[uniform_names::RADIUS] = glGetUniformLocation(shader_program, uniform_names::RADIUS.c_str());
    this->uniform_locs[uniform_names::OFFSET] = glGetUniformLocation(shader_program, uniform_names::OFFSET.c_str());
    this->uniform_locs[uniform_names::WORLD_RIGHT] = glGetUniformLocation(shader_program, uniform_names::WORLD_RIGHT.c_str());
}

Shader::~Shader() {
    if (this->program != 0) {
        glDeleteProgram(this->program);
    }
}

void Shader::bind() const {
    glUseProgram(this->program);
}

void Shader::set_uniform_1f(const std::string& name, const float value) const {
    glUniform1f(this->uniform_locs.at(name), value);
}

void Shader::set_uniform_2f(const std::string& name, const float value_1, const float value_2) const {
    glUniform2f(this->uniform_locs.at(name), value_1, value_2);
}