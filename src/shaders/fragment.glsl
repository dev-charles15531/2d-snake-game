#version 330 core

in vec2 finalTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;

void main()
{
  FragColor = mix(texture(ourTexture, finalTexCoord), texture(ourTexture1, finalTexCoord), 0.4);
}
