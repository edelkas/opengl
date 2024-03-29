#pragma once
#include <GLEW/glew.h>
#include <assert.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// -----------------------------------------------------------------------------
//                               ERROR HANDLING
// -----------------------------------------------------------------------------
#define DEBUG

/*
 * Only check for errors in debug mode
 * Abort program in case of error
 */
#ifdef DEBUG
    #define GLCall(x) GLClearErrors(); x; assert(!GLCheckErrors(#x, __FILE__, __LINE__));
#else
    #define GLCall(x) x
#endif

/* Clear all error flags */
void GLClearErrors();
/* Read, print and clear all errorss */
bool GLCheckErrors(const char* function, const char* file, int line);
// TODO: Translate OpenGL error codes into enums (or even words)

// -----------------------------------------------------------------------------
//                                 RENDERER
// -----------------------------------------------------------------------------
struct DrawElement {
  VertexArray va;
  IndexBuffer ib;
  Shader shader;
};

class Renderer
{
public:
  void Clear() const;
  void Draw(const DrawElement& elem) const;
};
