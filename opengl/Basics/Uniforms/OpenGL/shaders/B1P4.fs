#version 330 core
out vec4 FragColor;

in vec3 color;
//uniform vec4 triangle_color;
//uniform vec3 RGB = vec3(1.0f, 1.0f, 1.0f);

uniform float R = 1.0f;
uniform float G = 1.0f;
uniform float B = 1.0f;

void main()
{
    //FragColor = vec4(color.x * RGB.x, color.y * RGB.y, color.z * RGB.z, 1.0f);
    FragColor = vec4(color.x * R, color.y * G, color.z * B, 1.0f);
}