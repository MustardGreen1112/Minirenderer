#ifndef MINIRENDERER_STANDARD_VERTEX_SHADER_H_
#define MINIRENDERER_STANDARD_VERTEX_SHADER_H_
#include "../math/matrix.h"
#include "../graphics/shader_context.h"
#include "../graphics/texture.h"
#include "constants.h"
#include "../math/vector.h"

struct standard_uniform {
    mat4x4 local2world;
    mat4x4 world2clip;
    mat3x3 local2world_direction;
    mat3x3 local2world_normal;
    // Camera position in world space.
    vector3 camera_position;
    // Normalized directional light direction in world space.
    vector3 light_direction;
    // Transform vertex positions from world space to directional light¡®s light
    // space.
    mat4x4 world2light;
    // Directional light shadow map
    mat4x4 light2clip;
    texture* shadowmap;
    /// <summary>
    /// x,y,z components are color values and w component is strength.
    /// </summary>
    vector4 ambientColor;
};

struct standard_vertex_attribute {
    vector3 position;
    vector3 normal;
    vector2 uv;
    vector4 tangent;
};
vector4 standard_vertex_shader(shader_context& output, const void* uniform, const void* vertex_attribute);


#endif // !MINIRENDERER_STANDARD_VERTEX_SHADER_H_
