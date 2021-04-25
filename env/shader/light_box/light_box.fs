#version 330 core
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 color;
out vec4 FragColor;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform PointLight pointLight;
void main()
{
   vec3 temp = color * pointLight.ambient;
   FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2) * vec4(temp, 1.0);
}