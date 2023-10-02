#ifndef _MINIRENDERER_FRAGMENT_SHADER_H_
#define _MINIRENDERER_FRAGMENT_SHADER_H_
#include "shader_context.h"
#include "../shaders/material.h"
typedef vector4 (*fragment_shader) (shader_context& input, const void* uniform, const material& mat);
void set_fragment_shader(fragment_shader shader);
vector4 proceed_fragment_shader(shader_context& input, const void* uniform, const material& mat);
#endif // !_MINIRENDERER_FRAGMENT_SHADER_H_
