#ifndef _MINIRENDERER_STANDARD_FRAGMENT_SHADER_H_
#define _MINIRENDERER_STANDARD_FRAGMENT_SHADER_H_
#include "../math/vector.h"
#include "../graphics/shader_context.h"
#include "../graphics/texture.h"
#include "material.h"
#include "constants.h"
#include "standard_vertex_shader.h"
vector4 standard_fragment_shader(shader_context& input, const void* uniform, const material& mat);

#endif // !_MINIRENDERER_STANDARD_FRAGMENT_SHADER_H_
