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
in vec3 TexCoords;

uniform samplerCube cubemap;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Material material;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 outputColor = texture(cubemap, TexCoords);

    color = outputColor; //vec4(0.75f, 0.75f, 0.75f, 1.0f);
}