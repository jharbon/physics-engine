#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Particle.hpp>
#include <Vec2.hpp>

#include <iostream>
#include <chrono>

using sc = std::chrono::steady_clock;

constexpr float G_ACCEL = 9.81;  // m/s^2
constexpr double SIM_DELTA_T = 1.0 / 60.0;  // s - corresponds to 60Hz 
constexpr double FRAME_TIME_CLAMP = 0.25;  // s

void update(
        const double delta_t,
        Particle& p
);

void render(
        GLFWwindow* window,
        unsigned int VAO,
        unsigned int shaderProgram,
        int offsetLoc,
        const Particle& p
);

int main(int argc, char* argv[]) {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(
        960,  // Width
        540,  // Height
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

    float vertices[] = {
        0.0f,  0.5f,   // top
        -0.25f, -0.25f,   // bottom left
        0.25f, -0.25f    // bottom right      
    };

    // Generate and bind Vertex Array Object (intepret data) and Vertex Buffer Object (store data on GPU)
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
    // Tell GPU how to interpret vertex data
    glVertexAttribPointer(
        0,  // Attribute location (see shader)
        2,  // Values per vertex (x,y)
        GL_FLOAT,  // Value type
        GL_FALSE,  // Do not normalise (values already in correct range)
        2 * sizeof(float),  // Size of vertex
        (void*)0  // Where attribute starts in buffer
    );
    glEnableVertexAttribArray(0);

    // Shader to process positions
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    uniform vec2 offset;

    void main() {
        gl_Position = vec4(aPos + offset, 0.0, 1.0);
    }
    )";
    // Shader to set pixel colour
    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    )";

    // Compile and link shaders:

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int offsetLoc = glGetUniformLocation(shaderProgram, "offset");

    Particle particle(Vec2(0, 0), Vec2(0.5, 5.0), Vec2(0, -G_ACCEL));
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

        render(window, VAO, shaderProgram, offsetLoc, particle);
    }

    glfwTerminate();
    return 0;
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
        const Particle& p
) {
    // Clear screen to RGBA colour
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Vec2 pos = p.get_pos();
    // Use shader to draw triangle
    glUseProgram(shaderProgram);
    glUniform2f(offsetLoc, pos[0], pos[1]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Display frame
    glfwSwapBuffers(window);
}