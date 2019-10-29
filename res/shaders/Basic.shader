#shader vertex
#version 330 core
/* Specify index of attribute. It's actually a vec2, but we implicitly convert it to vec4. */
layout(location = 0) in vec4 position; // position is an input variable from the vertex buffer
void main(){
  gl_Position = position;              // gl_Position is a predefined variable
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;   // color is an output variable
void main(){
  color = vec4(0.2, 0.3, 0.8, 1.0);    // Specify the color of the pixels
};
