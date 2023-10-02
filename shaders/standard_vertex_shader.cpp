#include "standard_vertex_shader.h"


/// <summary>
/// Calculate world position(0), uv(0), world space normal(1), light space position(2).
/// </summary>
/// <param name="output">Clip-space position.</param>
/// <param name="uniform"></param>
/// <param name="vertex_attribute"></param>
/// <returns>Clip space position.</returns>
vector4 standard_vertex_shader(shader_context& output, const void* uniform, const void* vertex_attribute)
{
	const standard_uniform* unif = (standard_uniform*)uniform;
	const standard_vertex_attribute* attr = (standard_vertex_attribute*)vertex_attribute;

	output.write_data_v2(attr->uv, TEXCOORD);

	vector4 local_position = vector3_to_4(attr->position);
	local_position.w = 1.0f;
	vector4 world_position = mul(unif->local2world, local_position);
	output.write_data_v3(vector4_to_3(world_position),WORLD_SPACE_POSITION);
	output.write_data_v3(mul(unif->local2world_normal, attr->normal), WORLD_SPACE_NORMAL);
	output.write_data_v4(attr->tangent, WORLD_SPACE_TANGENT);

	vector4 light_space_position = mul(unif->world2light, world_position);
	output.write_data_v3 (vector4_to_3(light_space_position), LIGHT_SPACE_POSITION);

	return mul(unif->world2clip, world_position);
}