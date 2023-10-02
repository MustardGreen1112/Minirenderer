#ifndef _MINIRENDERE_VERTEX_SHADER_H_
#define _MINIRENDERE_VERTEX_SHADER_H_
#include "../math/vector.h"
#include "shader_context.h"


struct vertex {
	shader_context context;
	/// <summary>
	/// Clip-space position.
	/// </summary>
	vector4 position;
	vector2 screen_space_position;
	/// <summary>
	/// Range 0 - 1.
	/// </summary>
	float depth;
	float inverse_w;
};

typedef vector4 (*vertex_shader) (shader_context& output, const void* uniform, const void* vertex_attribute);
void set_vertex_shader(vertex_shader shader);
vector4 proceed_vertex_shader(shader_context& input, const void* uniform, const void* vertex_attribute);
#endif // !_MINIRENDERE_VERTEX_SHADER_H_
