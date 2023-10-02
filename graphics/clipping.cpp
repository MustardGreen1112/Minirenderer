#include "clipping.h"

static struct viewport viewport = { 0,0,0 };

void set_viewport(uint32_t width, uint32_t height) {
	viewport.width = width;
	viewport.height = height;
	viewport.aspect = (float)width / (float)height;
}

bool clipping_test(vector4 clipping_test_position)
{
	float w = clipping_test_position.w;
	for (int c = 0; c < 3; c++) {
		float component = clipping_test_position.elements[c];
		if (component < -w || component > w) {
			return true;
		}
	}
	return false;
}

void perspective_division(vertex& vertex) {
	float inverse_w = 1.0f / vertex.position.w;
	vertex.inverse_w = inverse_w;
	for (int c = 0; c < 4; c++)
	{
		vertex.position.elements[c] *= inverse_w;
	}
}

void viewport_transform(vertex& vertex) {
	vertex.screen_space_position.x = (0.5f * vertex.position.x + 0.5f) * (float)viewport.width;
	vertex.screen_space_position.y = (0.5f * vertex.position.y + 0.5f) * (float)viewport.height;
	vertex.depth = (vertex.position.z * 0.5f) + 0.5f;
}

void update_bouding_box(const vertex& vertex, bounding_box& bounding_box) {
	const vector2 position = vertex.screen_space_position;
	vector2& min = bounding_box.min;
	vector2& max = bounding_box.max;
	min.x = float_min(min.x, position.x);
	min.y = float_min(min.y, position.y);
	max.x = float_max(max.x, position.x);
	max.y = float_max(max.y, position.y);
}


/// <summary>
/// 
/// </summary>
/// <param name="vertices"></param>
/// <returns> If triangle is clockwise or degenerated, triangle is culled, returns true.</returns>
bool culling(const vertex* vertices, float& area) {
	vector2 a = vertices[0].screen_space_position;
	vector2 b = vertices[1].screen_space_position;
	vector2 c = vertices[2].screen_space_position;
	
	vector2 AB = b - a;
	vector2 AC = c - a;

	float f1 = cross(AC, AB);
	///if f1 > 0, the triangle is clockwise;
	///if if == 0 , the triangle is degenerated;
	///on both cases, the triangle should not be drawn.
	area = f1;
	if (f1 >= 0)
		return true;
	return false;
}

vector2 get_pixel_position(uint32_t x, uint32_t y) {
	if (x < viewport.width && y < viewport.height) {
		return vector2{ x + 0.5f, y + 0.5f };
	}
	return VECTOR2_ZERO;
}