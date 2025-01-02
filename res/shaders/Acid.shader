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
uniform sampler2D u_PrevFrame;
uniform vec2 u_Resolution;
uniform float u_Time;
uniform float u_DeltaTime;

vec3 overlay(vec3 base, vec3 blend, float decay) {
    return mix(2.0 * base * blend, 1.0 - 2.0 * (1.0 - base) * (1.0 - blend), step(0.5, base));
}

void main()
{
    float dynamicDecay = exp(-1.0f * u_DeltaTime);

    vec2 uv = v_TexCoords.xy / u_Resolution;
    vec4 prev_frame = texture(u_PrevFrame, v_TexCoords);
    vec4 c = texture(u_Texture, v_TexCoords);
    c.rgb = sin(c.rgb*(6.2)+0.8f*u_Time+vec3(3., 1.5,.5*texture(u_Texture, vec2(1.5*length(uv-.5),1.)).r))*0.5f+0.5f;
    vec3 blendedColor = overlay(c.rgb, prev_frame.rgb, dynamicDecay);
    color = vec4(blendedColor, 1.0f);
};