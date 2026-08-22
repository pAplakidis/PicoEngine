#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTexture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    float useTexture = float(u_UseTexture);

    vec4 baseColor = mix(vec4(1.0), texColor, useTexture);

    color = baseColor * v_Color * u_Color;
}