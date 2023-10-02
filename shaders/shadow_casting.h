#ifndef _MINIRENDERER_SHADOW_CASTING_H_
#define _MINIRENDERER_SHADOW_CASTING_H_
#include "../math/vector.h"
#include "standard_vertex_shader.h"
#include "../graphics/texture.h"
#include "constants.h"
vector4 shadow_casting(shader_context& output, const void* uniform, const void* vertex_attribute);
vector4 shadow_casting_fragment(shader_context& input, const void* uniform);
#endif // !_MINIRENDERER_SHADOW_CASTING_H_
