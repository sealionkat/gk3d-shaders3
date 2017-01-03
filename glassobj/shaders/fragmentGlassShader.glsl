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
in vec3 Position;

uniform samplerCube cubemap;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform Material material;

void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 outputColor = texture(cubemap, R);//vec4(objectColor, 1.0f);

    color = outputColor;
}