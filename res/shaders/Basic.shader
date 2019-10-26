#shader vertex
#version 330 core
layout(location = 0) in vec4 position; // Specify index of attribute, and source. It's actually a vec2, but we implicitly convert it to vec4
void main(){
  gl_Position = position;
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 position;
void main(){
  color = vec4(1.0, 0.0, 0.0, 1.0); // Specify the color of the pixels
};
