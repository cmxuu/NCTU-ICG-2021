#version 330

uniform sampler2D Texture;

in vec2 texcoord;
out vec4 outColor;

void main() {
	outColor = texture2D(Texture, texcoord);
}