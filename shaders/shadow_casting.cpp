#include "shadow_casting.h"

vector4 shadow_casting(shader_context& output, const void* uniform, const void* vertex_attribute)
{
	const standard_uniform* unif = (standard_uniform*)uniform;
	const standard_vertex_attribute* attr = (standard_vertex_attribute*)vertex_attribute;
	//calculate local position.
	vector4 localPosition = vector3_to_4(attr->position);
	localPosition.w = 1.0f;
	//calculate world position
	vector4 world_position = mul(unif->local2world, localPosition);
	//calculate light position and write to register.
	vector4 light_space_position = mul(unif->world2light, world_position);
	output.write_data_v3(vector4_to_3(light_space_position), LIGHT_SPACE_POSITION);
	//return clip space position
	return mul(unif->light2clip, light_space_position);
}

vector4 shadow_casting_fragment(shader_context& input, const void* uniform) {
	//const standard_uniform* unif = (standard_uniform*)uniform;
	//vector3 light_space_position = input.read_data_v3(LIGHT_SPACE_POSITION);
	//vector4 lsp_v4 = vector3_to_4(light_space_position);
	//lsp_v4.w = 1.0f;
	//lsp_v4 = mul(unif->light2clip, lsp_v4);
	//light_space_position = vector4_to_3(lsp_v4);
	//float z = light_space_position.z * 0.5f + 0.5f;
	return vector4{ 0,0,0,1.0f };
}