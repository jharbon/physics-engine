#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

    // Run main loop until window is closed
    while (!glfwWindowShouldClose(window)) {
        // Check for new events
        glfwPollEvents();

        // Clear screen to RGBA colour
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Display frame
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}