#type vertex
layout(location = 0) in vec3 a_Position;
layout(location = 5) in int a_EntityId;

layout(std140) uniform Camera
{
    mat4 u_ViewProjection;
};

flat out int v_EntityId;

void main()
{
	v_EntityId = a_EntityId;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
layout(location = 0) out int entityId;
flat in int v_EntityId;

void main()
{
	entityId = v_EntityId;
}
