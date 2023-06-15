#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 texcoord;


uniform mat4 M, V, P;

out vec2 uv;
out vec3 normal;
out vec4 worldPos;

void main() {
  
  // Normal
  mat3 normalmatrix = mat3(transpose(inverse(M)));
  normal = normalmatrix * in_normal;

  // Texture
  uv= texcoord;

  // Position
  worldPos = M * vec4(in_position, 1.0);

  gl_Position = P * V * M * vec4(in_position, 1.0);
}
