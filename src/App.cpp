#include <GLEW/glew.h>                       // OpenGL extensions
#include <GLFW/glfw3.h>                      // Windowing, input, etc.
#include <string>                            // std::string class
#include <iostream>                          // I/O manipulation
#include <fstream>                           // File stream
#include <sstream>                           // String stream
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// -----------------------------------------------------------------------------
//                                SHADER CODE
// -----------------------------------------------------------------------------
struct ShaderSource{
  std::string vertexSource;
  std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string& filepath){
  std::ifstream stream(filepath);                    // Input file stream
  std::string line;                                  // Store current line
  std::stringstream ss[2];                           // Store code for each shader

  enum class ShaderType{ NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  ShaderType type = ShaderType::NONE;

  /* Store each line of code into the appropriate stream, either vertex or fragment */
  while(getline(stream, line)){                      // Loop through lines of stream
    if (line.find("#shader") != std::string::npos){  // Shader sections
      if (line.find("vertex") != std::string::npos){ // Vertex shader header
        type = ShaderType::VERTEX;
      } else {                                       // Fragment shader header
        type = ShaderType::FRAGMENT;
      }
    } else {                                         // Any other line is code
      if (type != ShaderType::NONE) ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };               // Implicitly create struct
}

static unsigned int CompileShader(unsigned int type, const std::string& source){
  /* Compile shader */
  unsigned int id = glCreateShader(type);    // Create shader and store id
  const char* src = &source[0];              // Cast source to C string
  glShaderSource(id, 1, &src, nullptr);      // Select source of shader
  glCompileShader(id);                       // Compile shader

  /* Handle errors */
  // Retrieve compile status
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE){
    // Retrieve length of error message
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    // Retrieve error message
    char *message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);

    // Print error message
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n" << message << std::endl;
    delete[] message;

    // Delete erroneous shader
    glDeleteShader(id);
    return 0;
  }

  // Return
  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  // TODO: Error handling: Check for compile errors.

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);                        // Delete intermediate object files
  glDetachShader(program, vs);               // Delete shader source from memory
  glDeleteShader(fs);
  glDetachShader(program, fs);

  return program;
}

// -----------------------------------------------------------------------------
//                                  EXAMPLES
// -----------------------------------------------------------------------------

/**
 * EXAMPLE 1 - TRIANGLE
 */
int createBuffer1(){
  /* Buffer data, coordinates, vertices */
  float pos[6] = {
                  -0.5f, -0.5f,
                   0.5f, -0.5f,
                   0.0f,  0.5f
                 };

  /* Create vertex array */
  unsigned int vao; // Vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* Create and populate buffer */
  unsigned int buffer;                                                   // Pointer to index of buffer
  glGenBuffers(1, &buffer);                                              // Create buffer, index is saved in pointer
  glBindBuffer(GL_ARRAY_BUFFER, buffer);                                 // Bind buffer and establish type, mere array of data
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pos, GL_STATIC_DRAW); // Save data in buffer

  /* Layout of buffer (attributes) */
  glEnableVertexAttribArray(0);                                          // Enable parameter of index 0
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); // Specify layout of first and only attribute
}

int renderScene1(){
  /*
   * @mode  - self-explanatory
   * @first - index of first vertex
   * @count - amount of vertices
   */
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

/**
 * EXAMPLE 2 - SQUARE, W/O INDEX BUFFERS
 */
int createBuffer2(){
  /* Buffer data */
  float pos[12] = {
                    -0.5f, -0.5f, // First triangle
                     0.5f, -0.5f,
                     0.5f,  0.5f,

                     0.5f,  0.5f, // Second triangle
                    -0.5f,  0.5f,
                    -0.5f, -0.5f
                  };

  /* Create vertex array */
  unsigned int vao; // Vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* Create and populate buffer */
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 2* 6 * sizeof(float), pos, GL_STATIC_DRAW);

  /* Layout of buffer (attributes) */
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

int renderScene2(){
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

/**
 * EXAMPLE 3 - SQUARE, W/ INDEX BUFFERS
 */
int createBuffer3(){
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
  bl->Push<float>(2);              // Push 2 floats
  va->AddBuffer(*vb, *bl);           // Add buffer and layout to vertex array
}

int renderScene3(){
  /*
   * @mode    - self-explanatory
   * @count   - number of indices
   * @type    - type of data in index buffer
   * @indices - pointer to index buffer (optional if its bound)
   */
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

/**
 * EXAMPLE 4 - SQUARE, W/ INDEX BUFFERS, W/ UNIFORMS
 */
int createBuffer4(){
  createBuffer3();
}

int renderScene4(int uniform){
  GLCall(glUniform4f(uniform, 0.2f, 0.3f, 0.8f, 1.0f));
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

/**
 * EXAMPLE 5 - SQUARE, W/ INDEX BUFFERS, W/ UNIFORMS, ANIMATED
 */
int createBuffer5(){
  createBuffer3();
}

int renderScene5(int frame, int uniform){
  /*
   * The uniform index is -1 if it was not found.
   */
  if (uniform != -1) {
    int base = 100;
    int limit = 50;
    float g = ((float)(frame % limit)) / (float)base;
    if ((frame % (2 * limit)) >= limit) g = (float)limit / (float)base - g;
    GLCall(glUniform4f(uniform, 0.0f, g, 0.0f, 1.0f));
  }
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

// -----------------------------------------------------------------------------
//                              RENDERING CODE
// -----------------------------------------------------------------------------
static unsigned int renderInit(unsigned int &shader, int &uniform){
  /* Set up data buffer */
  createBuffer5();

  /* Read shader sources */
  ShaderSource src = ParseShader("res/shaders/Basic.shader");

  /* Set up shader */
  shader = CreateShader(src.vertexSource, src.fragmentSource);

  /* Bind shader */
  GLCall(glUseProgram(shader));

  /* Set up uniform */
  uniform = glGetUniformLocation(shader, "u_Color");
}

static void renderLoop(unsigned int frame, int uniform){
  renderScene5(frame, uniform);
}

static void renderTerminate(unsigned int shader){
  GLCall(glDeleteProgram(shader));             // Delete shader from memory
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
    int uniform;
    unsigned int shader;
    renderInit(shader, uniform);               // Set up our scene

    while (!glfwWindowShouldClose(window)) {   // Loop until window is closed
        /* Start of rendering */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Rendering */
        renderLoop(frame, uniform);                   // Render the scene

        /* End of rendering */
        glfwSwapBuffers(window);               // Swap front and back buffers
        glfwPollEvents();                      // Poll for and process events
        frame++;                               // Increment frame count
    }

    renderTerminate(shader);                   // Terminate rendering
    glfwTerminate();
    return 0;
}
