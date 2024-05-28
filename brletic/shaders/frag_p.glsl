#version 330 core

in vec2 ex_texcoord;
in vec3 ex_normal;
in vec3 ex_fragPos;

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D normal_map;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;

vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
    vec3 norm = texture(material.normal_map, ex_texcoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 viewDir = normalize(viewPos - ex_fragPos);
    vec3 result = calc_point_light(pointLight, norm, ex_fragPos, viewDir);
    fragColor = vec4(result, 1.0);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.ambient * vec3(255. / 255, 140. / 255, 0. / 255);
    vec3 diffuse = light.diffuse * diff * vec3(255. / 255, 140. / 255, 0. / 255);
    vec3 specular = light.specular * spec * vec3(255. / 255, 140. / 255, 0. / 255);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}