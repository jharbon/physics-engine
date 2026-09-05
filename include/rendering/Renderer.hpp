#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <rendering/Shader.hpp>
#include <particles/Particle.hpp>

#include <memory>
#include <vector>

class Renderer {
    private:
        GLFWwindow* window;
        unsigned int VBO, VAO;
        std::unique_ptr<Shader> shader;
        
    public:
        Renderer(unsigned int width, unsigned int height);
        ~Renderer();

        // Disable copying to avoid sharing resources
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;  

        // Disable moving for now
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        bool should_close() const;
        void poll_events();

        void clear(float r, float g, float b, float a);
        void draw(float world_right, const std::vector<Particle>& particles);

        GLFWwindow* get_window() const;
};

#endif