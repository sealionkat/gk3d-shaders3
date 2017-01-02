#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform Material material;

void main()
{
    vec4 outputColor = vec4(objectColor, 1.0f);

    color = outputColor;
}