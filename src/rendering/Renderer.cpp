#include <rendering/Renderer.hpp>

#include <iostream>
#include <stdexcept>

Renderer::Renderer(unsigned int width, unsigned int height)
    : window{nullptr}, VBO{0}, VAO{0}, shader{nullptr} {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise GLFW");
    }

    this->window = glfwCreateWindow(
        width,
        height,  
        "Physics Engine",  // Title
        NULL,  // Windowed mode
        NULL  // Do not share resources
    );

    if (!this->window) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialise window");
    }

    // Make all OpenGL commands affect window
    glfwMakeContextCurrent(this->window);

    // Load pointers to OpenGL functions from driver into GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialise GLAD");
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, width, height);

    // Define 2.0 x 2.0 quad using two triangles; dimensions are scaled using particle radius in vertex shader
    float vertices[] = {
        // Vertices      // UVs
        -1.0f, -1.0f,    0.0f, 0.0f,   
         1.0f, -1.0f,    1.0f, 0.0f,    
         1.0f,  1.0f,    1.0f, 1.0f,

        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,    0.0f, 1.0f
    };

    // Generate and bind Vertex Array Object (interpret data) and Vertex Buffer Object (store data on GPU)
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // Upload vertex data from CPU to GPU; copies vertices array into VBO
    glBufferData(
        GL_ARRAY_BUFFER,  // Target
        sizeof(vertices), 
        vertices,  // Pointer to CPU-side data 
        GL_STATIC_DRAW
    );
    // Tell GPU how to interpret position coordinates within each vertex
    glVertexAttribPointer(
        0,  // Attribute location (see shader)
        2,  // Values per position (x,y)
        GL_FLOAT,  // Value type
        GL_FALSE,  // Do not normalise (values already in correct range)
        4 * sizeof(float),  // Size of vertex
        (void*)0  // Where attribute starts in buffer
    );
    glEnableVertexAttribArray(0);
    // Tell GPU how to interpret UV coordinates within each vertex
    glVertexAttribPointer(
        1,  // Attribute location (see shader)
        2,  // Values per UV (u,v)
        GL_FLOAT,  // Value type
        GL_FALSE,  // Do not normalise (values already in correct range)
        4 * sizeof(float),  // Size of vertex
        (void*)(2 * sizeof(float))  // Where attribute starts in buffer
    );
    glEnableVertexAttribArray(1);

    this->shader = std::make_unique<Shader>();
}

Renderer::~Renderer() {
    if (this->VBO != 0) {
        glDeleteBuffers(1, &this->VBO);
    }
    if (this->VAO != 0) {
        glDeleteVertexArrays(1, &this->VAO);
    }

    if (this->window != nullptr) {
        glfwDestroyWindow(this->window);
    }
    glfwTerminate();
}

bool Renderer::should_close() const {
    return glfwWindowShouldClose(this->window);
}

void Renderer::poll_events() {
    glfwPollEvents();
}

void Renderer::draw(float world_right, const std::vector<Particle>& particles) {
    this->shader->bind();
    this->shader->set_uniform_1f(uniform_names::WORLD_RIGHT, world_right);
    glBindVertexArray(this->VAO);

    for (const auto& p : particles) {
        const Vec2 pos = p.get_pos();
        // Render circle via mask applied to quad
        this->shader->set_uniform_1f(uniform_names::RADIUS, p.get_radius());
        this->shader->set_uniform_2f(uniform_names::OFFSET, pos[0], pos[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Display frame
    glfwSwapBuffers(this->window);
}

void Renderer::clear(float r, float g, float b, float a) {
    // Clear screen to RGBA colour
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Renderer::get_window() const {
    return this->window;
}