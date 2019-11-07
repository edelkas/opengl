#include <GLEW/glew.h>                       // OpenGL extensions
#include <GLFW/glfw3.h>                      // Windowing, input, etc.
#include <string>                            // std::string class
#include <iostream>                          // I/O manipulation
#include <fstream>                           // File stream
#include <sstream>                           // String stream
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// -----------------------------------------------------------------------------
//                                BUFFER CODE
// -----------------------------------------------------------------------------

int createBuffer(){
  /* Vertex buffer data */
  float pos[] = {
                    -0.5f, -0.5f, // 0
                     0.5f, -0.5f, // 1
                     0.5f,  0.5f, // 2
                   - 0.5f,  0.5f  // 3
                  };

  /* Index buffer data */
  unsigned int indices[] = {
    0, 1, 2, // First triangle
    2, 3, 0  // Second triangle
  };

  /* Create vertex array */
  VertexArray *va = new VertexArray();

  /* Populate vertex buffer */
  VertexBuffer *vb = new VertexBuffer(pos, 4 * 2 * sizeof(float));

  /* Populate index buffer */
  IndexBuffer *ib = new IndexBuffer(indices, 6);

  /* Layout of vertex buffer (attributes) */
  BufferLayout *bl = new BufferLayout();
  bl->AddAttribute("float", 2);              // Push 2 floats
  va->AddBuffer(*vb, *bl);           // Add buffer and layout to vertex array
}

// -----------------------------------------------------------------------------
//                              RENDERING CODE
// -----------------------------------------------------------------------------
static void renderInit(Shader& shader){
  /* Set up data buffer */
  createBuffer();

  /* Set up the shader */
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
}

static void renderLoop(unsigned int frame, Shader& shader){
  int base = 100;
  int limit = 50;
  float g = ((float)(frame % limit)) / (float)base;
  if ((frame % (2 * limit)) >= limit) g = (float)limit / (float)base - g;
  shader.SetUniform4f("u_Color", 0.0f, g, 0.0f, 1.0f);
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

// -----------------------------------------------------------------------------
//                                MAIN PROGRAM
// -----------------------------------------------------------------------------
int main(void)
{
    GLFWwindow* window;                        // Create the window
    if (!glfwInit()) return -1;                // Initialize GLFW

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);            // Make window the current context
    glfwSwapInterval(1);                       // Sync framerate with refresh rate
    if (glewInit() != GLEW_OK) return -1;      // Initialize GLEW

    unsigned int frame = 0;
    Shader shader("res/shaders/Basic.shader");
    renderInit(shader);                        // Set up our scene

    while (!glfwWindowShouldClose(window)) {   // Loop until window is closed
        /* Start of rendering */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Rendering */
        renderLoop(frame, shader);             // Render the scene

        /* End of rendering */
        glfwSwapBuffers(window);               // Swap front and back buffers
        glfwPollEvents();                      // Poll for and process events
        frame++;                               // Increment frame count
    }

    glfwTerminate();
    return 0;
}
