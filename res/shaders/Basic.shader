#shader vertex
#version 330 core

// fullcreen quad vertex shader

layout (location = 1) in vec2 Position;
layout (location = 2) in vec2 UV;
layout (location = 0) in vec4 Color;
//layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoords;

void main()
{
    // hard coded fullcreen triangle strip vertices
    // vec2 positions[4] = vec2[4](
    //     vec2(-1.0, -1.0), // Bottom-left
    //     vec2(1.0, -1.0),  // Bottom-right
    //     vec2(1.0, 1.0),   // Top-right
    //     vec2(-1.0, 1.0)   // Top-left
    // );

    // Assign position to gl_Position
    gl_Position = vec4(Position.xy, 0, 1);
    
    // Texture coordinates for each vertex
    //v_TexCoords = (positions[gl_VertexID] + 1.0) * 0.5;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//in vec2 v_TexCoords;

//uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
   //color = texture(u_Texture, v_TexCoords);
   color = vec4(1, 0, 0, 1);
};