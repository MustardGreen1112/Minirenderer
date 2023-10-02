#include "fragment_shader.h"

static fragment_shader fs = nullptr;
void set_fragment_shader(fragment_shader shader) { fs = shader; }
vector4 proceed_fragment_shader(shader_context& input, const void* uniform, const material& mat) { 
	if (fs != nullptr) return fs(input, uniform, mat);
	else return vector4{ 0,0,0,0 };
}