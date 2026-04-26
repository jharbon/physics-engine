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

    // Run main loop until window is closed
    while (!glfwWindowShouldClose(window)) {
        // Check for new events
        glfwPollEvents();
        // Display frame
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}