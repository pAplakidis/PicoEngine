#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTexture;

void main()
{
	vec4 baseColor = vec4(1.0);
	if (u_UseTexture == 1)
		baseColor = texture(u_Texture, v_TexCoord);
	color = baseColor * u_Color;
}
