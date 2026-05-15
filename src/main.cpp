#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Particle.hpp>
#include <Vec2.hpp>

#include <iostream>
#include <chrono>

using sc = std::chrono::steady_clock;

constexpr int WIN_WIDTH = 960;
constexpr int WIN_HEIGHT = 540;

constexpr float PARTICLE_RADIUS = 0.2;
constexpr float G_ACCEL = 9.81;  // m/s^2
constexpr double SIM_DELTA_T = 1.0 / 60.0;  // s - corresponds to 60Hz 
constexpr double FRAME_TIME_CLAMP = 0.25;  // s

void check_shader_compilation(const unsigned int shader, const char* name);
void check_program_linking(const unsigned int program, const char* name);

void update(
        const double delta_t,
        Particle& p
);

void render(
        GLFWwindow* window,
        unsigned int VAO,
        unsigned int shaderProgram,
        int offsetLoc,
        int resolutionLoc,
        const Particle& p
);

int main(int argc, char* argv[]) {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(
        WIN_WIDTH,
        WIN_HEIGHT,  
        "Physics Engine",  // Title
        NULL,  // Windowed mode
        NULL  // Do not share resources
    );

    if (!window) {
        std::cerr << "Failed to initialise window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make all OpenGL commands affect window
    glfwMakeContextCurrent(window);

    // Load pointers to OpenGL functions from driver into GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

    const float r = PARTICLE_RADIUS;
    // Define quad with dimensions 2r x 2r using two triangles; this small local quad moves to particle position
    float vertices[] = {
        // Vertices     // UVs
        -r, -r,         0.0f, 0.0f,   
         r, -r,         1.0f, 0.0f,    
         r,  r,         1.0f, 1.0f,

        -r, -r,         0.0f, 0.0f,
         r,  r,         1.0f, 1.0f,
        -r,  r,         0.0f, 1.0f
    };

    // Generate and bind Vertex Array Object (interpret data) and Vertex Buffer Object (store data on GPU)
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

    // Shader to process vertices; runs once per vertex 
    const char* vertexShaderSource = 
        R"(#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aUV;

        uniform vec2 offset;

        out vec2 uv;

        void main() {
            uv = aUV;
            gl_Position = vec4(aPos + offset, 0.0, 1.0);
        }
        )";

    // Shader to process fragments; each pixel is a fragment and this runs once per fragment
    const char* fragmentShaderSource = 
        R"(#version 330 core
        in vec2 uv;
        out vec4 FragColor;

        uniform vec2 resolution;

        // Define radius in UV space such that coloured circle has radius equal to half the quad dimension 
        const float radius = 0.5;
        const vec2 centre = vec2(0.5, 0.5);

        void main() {
            // Find frag point relative to centre and scale according to aspect ratio
            vec2 p = uv - centre;
            p.x *= resolution.x / resolution.y;
            float dist = length(p);

            // Only colour fragments on circle
            if (dist > radius)
                discard;
                
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        )";

    // Compile and link shaders:

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    check_shader_compilation(vertexShader, "Vertex Shader");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    check_shader_compilation(fragmentShader, "Fragment Shader");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    check_program_linking(shaderProgram, "Shader Program");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int offsetLoc = glGetUniformLocation(shaderProgram, "offset");
    int resolutionLoc = glGetUniformLocation(shaderProgram, "resolution");

    Particle particle(PARTICLE_RADIUS, 0.1, Vec2(0, 0), Vec2(0.25, 2.0), Vec2(0, -G_ACCEL / 5));
    auto last = sc::now();
    auto current = sc::now();
    double frame_time;
    double accumulator = 0.0;

    // Run main loop until window is closed
    while (!glfwWindowShouldClose(window)) {
        // Check for new events
        glfwPollEvents();
        
        current = sc::now();
        frame_time = std::chrono::duration<double>(current - last).count();
        last = current;

        // Clamp very large frame times which could occur due to stutter, user interaction, etc...
        if (frame_time > FRAME_TIME_CLAMP) {
            frame_time = FRAME_TIME_CLAMP;
        }

        accumulator += frame_time;

        while (accumulator >= SIM_DELTA_T) {
            update(SIM_DELTA_T, particle);
            accumulator -= SIM_DELTA_T;
        }

        render(window, VAO, shaderProgram, offsetLoc, resolutionLoc, particle);
    }

    glfwTerminate();
    return 0;
}

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
    }
}

void update(
        const double delta_t,
        Particle& p
) {
    p.update(delta_t);
}

void render(
        GLFWwindow* window,
        unsigned int VAO,
        unsigned int shaderProgram,
        int offsetLoc,
        int resolutionLoc,
        const Particle& p
) {
    // Clear screen to RGBA colour
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Vec2 pos = p.get_pos();
    // Render circle via mask applied to quad
    glUseProgram(shaderProgram);
    glUniform2f(offsetLoc, pos[0], pos[1]);
    glUniform2f(resolutionLoc, WIN_WIDTH, WIN_HEIGHT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Display frame
    glfwSwapBuffers(window);
}