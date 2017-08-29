#version 330 core

layout(location = 0) in vec3 quadPosition;

out vec2 UV;

void main(void)
{
    gl_Position = vec4(quadPosition, 1.0f);

    UV = (quadPosition.xy + vec2(1.0f)) / 2.0f;
}
