#ifndef MINIRENDERER_CLIPPING_H_
#define MINIRENDERER_CLIPPING_H_
#include "../math/vector.h"
#include "vertex_shader.h"


struct bounding_box {
	vector2 min;
	vector2 max;
};

struct viewport {
	uint32_t width, height;
	float aspect;
};
/// <summary>
/// left-bottom is (0,0).
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void set_viewport(uint32_t width, uint32_t height);
/// <summary>
/// Compare clip-space xyz component with w(-z) component. If xyz component is within clip-space,
/// return false, else return true. x,y,z component should be within (w, -w).
/// </summary>
/// <param name="clipping_test_position"></param>
/// <returns></returns>
bool clipping_test(vector4 clipping_test_position);
/// <summary>
/// vertex.inverse_w will be used for linear interpolation.
/// </summary>
/// <param name="vertex"></param>
void perspective_division(vertex& vertex);
/// <summary>
/// Change vertex clip space position to vertex.screen_space_position.
/// vertex.depth changed from (-1,1) to (0,1).
/// </summary>
/// <param name="vertex"></param>
void viewport_transform(vertex& vertex);
void update_bouding_box(const vertex& vertex, bounding_box& bounding_box);
bool culling(const vertex* vertex, float& area);
vector2 get_pixel_position(uint32_t x, uint32_t y);

#endif // !MINIRENDERER_CLIPPING_H_
