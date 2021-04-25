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
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform bool processed;
uniform bool surface_flag;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform Material material;

in GS_OUT {
    float surface_flag;
    vec2 texcoord;
    vec3 frag_pos;
    vec3 camera_pos;
    vec3 normal;
} fs_in;

layout (std140) uniform point_light_property
{
    PointLight pointLight;
};

out vec4 frag_color;

vec4 calculate_texture()
{
    vec4 tex;
    if (surface_flag || processed || fs_in.surface_flag - 0.5f > 0.0f) {
        tex = texture(texture1, fs_in.texcoord);
    }
    else {
        tex = texture(texture0, fs_in.texcoord);
    }
    return tex;
}

vec4 calculate_light()
{
    //light
    vec3 norm = normalize(fs_in.normal);
    vec3 light_dir = normalize(pointLight.position - fs_in.frag_pos);

    vec3 ambient = pointLight.ambient * material.ambient;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * pointLight.diffuse * material.diffuse;

    vec3 view_dir = normalize(fs_in.camera_pos - fs_in.frag_pos);
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = spec * pointLight.specular * material.specular;

    float distance = length(pointLight.position - fs_in.frag_pos);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear*distance + pointLight.quadratic*(distance*distance));
   
    vec4 light = vec4((ambient + diffuse + specular) * attenuation, 1.0f);
    return light;
}

void main()
{
    //tex
    vec4 tex = calculate_texture();

    vec4 light = calculate_light();

    frag_color = tex * light;
}