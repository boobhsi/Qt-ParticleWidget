#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color;

out vec2 UV;
out vec4 particlecolor;

uniform mat4 mvp; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 upAxis;
uniform vec3 rightAxis;

void main()
{
        float particleSize = xyzs.w;
	vec3 particleCenter_wordspace = xyzs.xyz;
	
        vec3 vertexPosition_worldspace = particleCenter_wordspace +
              ( normalize(rightAxis) * squareVertices.x + normalize(upAxis) * squareVertices.y) * particleSize;

        gl_Position = mvp * vec4(vertexPosition_worldspace, 1.0f);

	UV = squareVertices.xy + vec2(0.5, 0.5);
        particlecolor = color;
}

