#type vertex
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	//gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

#ifndef GL_ES
uniform sampler2D u_Textures[32];
#else
uniform sampler2D u_Textures[16];
#endif

#ifdef GL_ES
vec4 GetTextureColor(int index, vec2 texCoord)
{
	switch (index)
	{
		case 0:	 return texture(u_Textures[0], texCoord);
		case 1:	 return texture(u_Textures[1], texCoord);
		case 2:	 return texture(u_Textures[2], texCoord);
		case 3:	 return texture(u_Textures[3], texCoord);
		case 4:	 return texture(u_Textures[4], texCoord);
		case 5:	 return texture(u_Textures[5], texCoord);
		case 6:	 return texture(u_Textures[6], texCoord);
		case 7:	 return texture(u_Textures[7], texCoord);
		case 8:	 return texture(u_Textures[8], texCoord);
		case 9:	 return texture(u_Textures[9], texCoord);
		case 10: return texture(u_Textures[10], texCoord);
		case 11: return texture(u_Textures[11], texCoord);
		case 12: return texture(u_Textures[12], texCoord);
		case 13: return texture(u_Textures[13], texCoord);
		case 14: return texture(u_Textures[14], texCoord);
		case 15: return texture(u_Textures[15], texCoord);
		default: return texture(u_Textures[0], texCoord);
	}
}
#endif

void main()
{
	#ifndef GL_ES
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
	#else
	color = GetTextureColor(int(v_TexIndex), v_TexCoord * v_TilingFactor) * v_Color;
	#endif
}