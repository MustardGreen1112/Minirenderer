#ifndef _MINIRENDERER_RASTERIZER_H_
#define _MINIRENDERER_RASTERIZER_H_

#include "shader_context.h"
#include "vertex_shader.h"
#include "clipping.h"
#include "framebuffer.h"
#include <vector>;
#include "../shaders/constants.h"

/// <summary>
/// Vector2 shader context.
/// </summary>

std::vector<shader_context> rasterize(vertex* vertices, bounding_box bound, float area, framebuffer& fb, attachment_type depth_test_buffer_type);

#endif // !_MINIRENDERER_RASTERIZER_H
