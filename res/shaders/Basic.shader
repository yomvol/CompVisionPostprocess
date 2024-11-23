#shader vertex
#version 330 core

layout (location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(pos.xy, 0.0, 1.0);
    v_TexCoords = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
   //color = texture(u_Texture, v_TexCoords);
   color = vec4(0, 1, 0, 1);
};