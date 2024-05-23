#version 330 core
out vec4 FragColor;

in vec3 Normal;  // the normal of the vertex
in vec3 FragPos; // the position of the fragment

uniform vec3 lightPos;  // the position of the light
uniform vec3 lightColor; // the color of the light

void main()
{
    // calculate the direction of the light
    vec3 lightDir = normalize(lightPos - FragPos);

    // calculate the cosine of the angle between the light direction and the surface normal
    float diff = max(dot(lightDir, Normal), 0.0);

    // calculate the color of the light based on the diffuse lighting model
    vec3 diffuse = diff * lightColor;

    // set the color of the fragment
    FragColor = vec4(diffuse, 1.0);
}
