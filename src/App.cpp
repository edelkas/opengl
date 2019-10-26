#include <GLEW/glew.h>                       // OpenGL extensions
#include <GLFW/glfw3.h>                      // Windowing, input, etc.

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
    delete[] errorMessage;

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
  glDetachShader(vs);                        // Delete shader source from memory
  glDeleteShader(fs);
  glDetachShader(fs);

  return program;
}

int createBuffer(){
  /* Buffer data */
  float pos[6] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };              // Coordenadas, datos a guardar en el buffer

  /* Create and populate buffer */
  unsigned int buffer;                                                   // Lugar donde guardar el índice
  glGenBuffers(1, &buffer);                                              // Creo el buffer, cuyo índice se guarda en la dirección de buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer);                                 // Seleccionamos el tipo de buffer, en este caso, un simple array de datos.
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pos, GL_STATIC_DRAW); // Guardamos los datos en el buffer

  /* Layout of buffer (attributes) */
  glEnableVertexAttribArray(0);                                          // El parámetro es el índice del atributo a activar.
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); // Especificamos el layout del primer y único atributo de nuestros vértices
}

int main(void)
{
    GLFWwindow* window;                      // Create the window
    if (!glfwInit()) return -1;              // Initialize GLFW

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);          // Make window the current context
    if (glewInit() != GLEW_OK) return -1;    // Initialize GLEW

    createBuffer();

    while (!glfwWindowShouldClose(window)) { // Loop until window is closed
        /* Start of rendering */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Esto es un ejemplo de Legacy OpenGL, que no necesita buffers, shaders, ni nada */
        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f,-0.5f);
        // glVertex2f(0.5f,-0.5f);
        // glVertex2f(0.0f,0.5f);
        // glEnd();

        /* En OpenGL moderno, usamos esto */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* End of rendering */
        glfwSwapBuffers(window);             // Swap front and back buffers
        glfwPollEvents();                    // Poll for and process events
    }

    glfwTerminate();
    return 0;
}
