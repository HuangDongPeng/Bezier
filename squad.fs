#version 330
in vec2 texcoord;
out vec4 FragColor;

uniform sampler2D tex;

void main(void) {
  FragColor = texture(tex, texcoord);
  //FragColor=vec4(texcoord,1,1);
}
