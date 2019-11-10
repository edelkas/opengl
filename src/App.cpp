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
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"             // Parsing images for texture load

// -----------------------------------------------------------------------------
//                              RENDERING CODE
// -----------------------------------------------------------------------------
static DrawElement renderInit(Shader& shader){
  /**
   * Vertex buffer data
   *
   * - First 2 floats are normalized spatial coordinates of the vertices.
   * - Second 2 floats are normalized texture coordinates. Origin is at
   */
  float pos[] = {
                    -0.5f, -0.5f, 0.0f, 0.0f, // 0
                     0.5f, -0.5f, 1.0f, 0.0f, // 1
                     0.5f,  0.5f, 1.0f, 1.0f, // 2
                    -0.5f,  0.5f, 0.0f, 1.0f  // 3
                  };

  /* Index buffer data */
  unsigned int indices[] = {
    0, 1, 2, // First triangle
    2, 3, 0  // Second triangle
  };

  /* Create vertex array */
  VertexArray *va = new VertexArray();

  /* Populate vertex buffer */
  VertexBuffer *vb = new VertexBuffer(pos, 4 * 4 * sizeof(float));

  /* Populate index buffer */
  IndexBuffer *ib = new IndexBuffer(indices, 6);

  /* Layout of vertex buffer (attributes) */
  BufferLayout *bl = new BufferLayout();
  bl->AddAttribute("float", 2);              // Push 2 floats, spatial coords
  bl->AddAttribute("float", 2);              // Push 2 floats, texture coords
  va->AddBuffer(*vb, *bl);                   // Add buffer and layout to vertex array

  /* Set up the shader */
  shader.Bind();
  //shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

  /* Set up texture */
  Texture texture("res/textures/texture.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  /* Return Draw Element */
  return {*va, *ib, shader};
}

static void renderLoop(unsigned int frame, const Renderer& renderer, DrawElement& elem){
  int base = 100;
  int limit = 50;
  float g = ((float)(frame % limit)) / (float)base;
  if ((frame % (2 * limit)) >= limit) g = (float)limit / (float)base - g;
  //elem.shader.SetUniform4f("u_Color", 0.0f, g, 0.0f, 1.0f);
  renderer.Draw(elem);
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

    unsigned int frame = 0;                    // Frame counter
    Renderer renderer;
    Shader shader("res/shaders/Basic.shader"); // Set up shader
    DrawElement elem = renderInit(shader);     // Set up our scene

    while (!glfwWindowShouldClose(window)) {   // Loop until window is closed
        /* Start of rendering */
        renderer.Clear();

        /* Rendering */
        renderLoop(frame, renderer, elem);     // Render the scene

        /* End of rendering */
        glfwSwapBuffers(window);               // Swap front and back buffers
        glfwPollEvents();                      // Poll for and process events
        frame++;                               // Increment frame count
    }

    glfwTerminate();
    return 0;
}
