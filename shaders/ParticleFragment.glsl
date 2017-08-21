
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 color;

uniform sampler2D textureSampler;

void main(){
        // Output color = color of the texture at the specified UV
        color = texture2D(textureSampler, UV);
        color *= particlecolor;
        float dis = length(UV - vec2(0.5f));
        if (dis > 0.5f) dis = 0.5f;
        float invDisFromCenter = 1.0f - 4.0f * dis * dis;
        color.a *= invDisFromCenter;
}
