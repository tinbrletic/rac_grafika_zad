#version 430
uniform vec3 lightColor;

out vec4 fragColor;
void main(void)
{
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    if (dot(circCoord, circCoord) > 1.0) {
        discard;
    }
    fragColor = vec4(lightColor, 1.);
}