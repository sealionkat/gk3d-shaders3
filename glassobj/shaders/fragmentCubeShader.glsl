#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    color = vec4(0.75f, 0.75f, 0.75f, 1.0f);
}