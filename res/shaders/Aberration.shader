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
uniform vec3 u_Offset;

void main()
{
    vec4 rValuee = texture(u_Texture, v_TexCoords + vec2(0, u_Offset.r));
    vec4 gValuee = texture(u_Texture, v_TexCoords + vec2(u_Offset.g, 0));
    vec4 bValuee = texture(u_Texture, v_TexCoords + vec2(-u_Offset.b, 0));

    color = vec4(rValuee.r, gValuee.g, bValuee.b, 1.0f);
};