#include "vertex_shader.h"

static vertex_shader vs = nullptr;
void set_vertex_shader(vertex_shader shader) { vs = shader; }
vector4 proceed_vertex_shader(shader_context& input, const void* uniform, const void* vertex_attribute) { if (vs != nullptr) return vs(input, uniform, vertex_attribute); };