//------------------------------------------------------------------------------
//                               VERTEX SHADER
//------------------------------------------------------------------------------

#shader vertex                         // User-defined tag to separate sections of shader
#version 330 core                      // GLSL version, ignore deprecated functions

/*
 * Declare an INPUT variable from the vertex buffer, of TYPE vec4.
 * Specify index of attribute of said vertex, using layout.
 * Note: It's actually a vec2, but we implicitly convert it to a vec4.
 */
layout(location = 0) in vec4 position; // Spatial coordinates of vertex
layout(location = 1) in vec2 texCoord; // Texture coordinates

out vec2 v_texCoord;

void main(){
  gl_Position = position;              // gl_Position is a predefined GLSL variable
  v_texCoord = texCoord;
};

//------------------------------------------------------------------------------
//                              FRAGMENT SHADER
//------------------------------------------------------------------------------

#shader fragment
#version 330 core
in vec2 v_texCoord;                    // Capture INPUT variable from vertex shader
layout(location = 0) out vec4 color;   // Declare the OUTPUT variable, color
uniform vec4 u_Color;
uniform sampler2D u_Texture;
void main(){
  /*
   * Example 1: Explicit specification
   */
  // color = vec4(0.2, 0.3, 0.8, 1.0);    // Specify value of output color

  /*
   * Example 2: Using uniforms
   */
  //color = u_Color;                     // Specify value of output color

  /*
   * Example 3: Using textures
   */
  vec4 texColor = texture(u_Texture, v_texCoord); // Sample texture by interpolating
  color = texColor;                               // Specify value of output color
};
