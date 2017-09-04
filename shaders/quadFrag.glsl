#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D renderTexture;
/*
uniform int blurTimes;
uniform int resolution;
*/
void main(void)
{
    //float blurStep = 1.0f / resolution;

        color = texture2D(renderTexture, UV);// * 0.2270270270;

        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * -4.0f)) * 0.0162162162;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * -3.0f)) * 0.0540540541;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * -2.0f)) * 0.1216216216;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * -1.0f)) * 0.1945945946;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * 1.0f)) * 0.1945945946;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * 2.0f)) * 0.1216216216;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * 3.0f)) * 0.0540540541;
        //color += texture2D(renderTexture, UV + vec2(0.0f, blurStep * 4.0f)) * 0.0162162162;

}
