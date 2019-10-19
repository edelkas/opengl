Explanation of the required libraries:

# libGL: Basically any OpenGL application must link this library, it contains
# 	the basic OpenGL API entrypoints.
# libGLFW: It implements basic functionality, like windowing, inputs, etc.
# libGLEW: It recognizes the platform and loads the correspoding OpenGL
# 	extensions, or in other words, the implementation for your GPU.

Notes for the libraries:

# GLEW must be included before GLFW.
