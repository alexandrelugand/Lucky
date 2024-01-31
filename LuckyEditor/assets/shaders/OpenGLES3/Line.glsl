// ------------------------------------
// - Lucky -
// Renderer 2D Line Shader
// ------------------------------------

#type vertex
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityId;

layout(std140) uniform Camera
{
    mat4 u_ViewProjection;
};

out vec4 v_Color;
flat out int v_EntityId;

void main()
{
	v_Color = a_Color;
	v_EntityId = a_EntityId;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityId;
in vec4 v_Color;
flat in int v_EntityId;

void main()
{
	o_Color = v_Color;
	o_EntityId = v_EntityId;
}
