#include <GLEW/glew.h>                        // OpenGL extensions
#include <GLFW/glfw3.h>                       // Windowing, input, etc.

int main(void)
{
    GLFWwindow* window;                       // Create the window
    if (!glfwInit()) return -1;               // Initialize GLFW

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);           // Make window the current context
    if (glewInit() != GLEW_OK) return -1;     // Initialize GLEW

    while (!glfwWindowShouldClose(window)) {  // Loop until window is closed
        /* Start of rendering */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.5f,-0.5f);
        glVertex2f(0.0f,0.5f);
        glEnd();

        /* End of rendering */
        glfwSwapBuffers(window);               // Swap front and back buffers
        glfwPollEvents();                      // Poll for and process events
    }

    glfwTerminate();
    return 0;
}
