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

layout (std140) uniform point_light_property
{
    PointLight pointLight;
};
uniform vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec3 cameraPos;

out vec4 fragColor;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pointLight.position - fragPos);

    vec3 ambient = pointLight.ambient;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLight.diffuse;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * pointLight.specular;

    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear*distance + pointLight.quadratic*(distance*distance));
   
    vec3 result = vec3(color) * (ambient + diffuse + specular) * attenuation;
    fragColor = vec4(result, 1.0f);
}