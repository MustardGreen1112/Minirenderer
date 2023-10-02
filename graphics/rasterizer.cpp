#include "rasterizer.h"

static bool depth_test(const float barycentric[], const vertex* vertices, uint32_t x, uint32_t y, framebuffer& fb, attachment_type buffer) {
	float* depth_buffer = (float*)fb.get_buffer(buffer);
	if (depth_buffer != nullptr) {
		uint32_t framebuffer_width = fb.get_buffer_width();
		float new_depth = barycentric[0] * vertices[0].depth +
			barycentric[1] * vertices[1].depth +
			barycentric[2] * vertices[2].depth;
		float& depth = depth_buffer[x + y * framebuffer_width];
		bool is_hidden = false;
		if (new_depth <= depth) {
			depth = new_depth;
			is_hidden = false;
		}
		else {
			is_hidden = true;
		}
		return is_hidden;
	}
	else { return true; };
}

void interpolate_f(vertex* vertices, shader_context* result, float* bc_divided_by_w, float z) {
	int8_t* queue = vertices[0].context.get_queue_f();
	int8_t count = vertices[0].context.get_count_f();
	for (int i = 0; i < count; i++)
	{
		int index = queue[i];
		float data0 = vertices[0].context.read_data_f(index);
		float data1 = vertices[1].context.read_data_f(index);
		float data2 = vertices[2].context.read_data_f(index);
		float data = (data0 * bc_divided_by_w[0] + data1 * bc_divided_by_w[1] + data2 * bc_divided_by_w[2]) * z;
		result->write_data_f(data, index);
	}
}
void interpolate_v2(vertex* vertices, shader_context* result, float* bc_divided_by_w, float z) {
	int8_t* queue = vertices[0].context.get_queue_v2();
	int8_t count = vertices[0].context.get_count_v2();
	for (int i = 0; i < count; i++)
	{
		int index = queue[i];
		vector2 data0 = vertices[0].context.read_data_v2(index);
		vector2 data1 = vertices[1].context.read_data_v2(index);
		vector2 data2 = vertices[2].context.read_data_v2(index);
		vector2 data = (data0 * bc_divided_by_w[0] + data1 * bc_divided_by_w[1] + data2 * bc_divided_by_w[2]) * z;
		result->write_data_v2(data, index);
	}
}
void interpolate_v3(vertex* vertices, shader_context* result, float* bc_divided_by_w, float z) {
	int8_t* queue = vertices[0].context.get_queue_v3();
	int8_t count = vertices[0].context.get_count_v3();
	for (int i = 0; i < count; i++)
	{
		int index = queue[i];
		vector3 data0 = vertices[0].context.read_data_v3(index);
		vector3 data1 = vertices[1].context.read_data_v3(index);
		vector3 data2 = vertices[2].context.read_data_v3(index);
		vector3 data = (data0 * bc_divided_by_w[0] + data1 * bc_divided_by_w[1] + data2 * bc_divided_by_w[2]) * z;
		result->write_data_v3(data, index);
	}
}
void interpolate_v4(vertex* vertices, shader_context* result, float* bc_divided_by_w, float z) {
	int8_t* queue = vertices[0].context.get_queue_v4();
	int8_t count = vertices[0].context.get_count_v4();
	for (int i = 0; i < count; i++)
	{
		int index = queue[i];
		vector4 data0 = vertices[0].context.read_data_v4(index);
		vector4 data1 = vertices[1].context.read_data_v4(index);
		vector4 data2 = vertices[2].context.read_data_v4(index);
		vector4 data = (data0 * bc_divided_by_w[0] + data1 * bc_divided_by_w[1] + data2 * bc_divided_by_w[2]) * z;
		result->write_data_v4(data, index);
	}
}

/// <summary>
/// Interpolate values from vertices by barycentric coordinates. Including a perspective correction.
/// </summary>
/// <param name="fragment_input">Result.</param>
/// <param name="vertices">Three vertices of the triangle.</param>
/// <param name="barycentric_coordinate">BC of fragment in the triangle. </param>
void set_fragment_input(shader_context* p_out_fragment_input, vertex* vertices, const float barycentric_coordinate[]) {
	float bc_divided_by_w[3];
	for (int i = 0; i < 3; i++) {
		bc_divided_by_w[i] = barycentric_coordinate[i] * vertices[i].inverse_w;
	}
	float z = 1.0f / (bc_divided_by_w[0] + bc_divided_by_w[1] + bc_divided_by_w[2]);
	interpolate_f(vertices, p_out_fragment_input, bc_divided_by_w, z);
	interpolate_v2(vertices, p_out_fragment_input, bc_divided_by_w, z);
	interpolate_v3(vertices, p_out_fragment_input, bc_divided_by_w, z);
	interpolate_v4(vertices, p_out_fragment_input, bc_divided_by_w, z);
}


std::vector<shader_context> rasterize(vertex* vertices, const bounding_box bound, const float area, framebuffer& fb, attachment_type depth_test_buffer_type) {
	uint32_t xmin = floorf(bound.min.x);
	uint32_t ymin = floorf(bound.min.y);
	uint32_t xmax = floorf(bound.max.x);
	uint32_t ymax = floorf(bound.max.y);

	vector2 AB = vertices[1].screen_space_position - vertices[0].screen_space_position;
	vector2 BC = vertices[2].screen_space_position - vertices[1].screen_space_position;
	vector2 CA = vertices[0].screen_space_position - vertices[2].screen_space_position;
	std::vector<shader_context> fragments_inputs;
	float inverse_area = 1.0f / area;
	for (int y = ymin; y <= ymax; y++) {
		for (int x = xmin; x <= xmax; x++) {
			vector2 pixel_coordinate = get_pixel_position(x, y);
			if (pixel_coordinate == VECTOR2_ZERO)
				continue;
			float barycentric_coordinates[3];
			vector2 ap = pixel_coordinate - vertices[0].screen_space_position;
			vector2 bp = pixel_coordinate - vertices[1].screen_space_position;
			vector2 cp = pixel_coordinate - vertices[2].screen_space_position;

			barycentric_coordinates[0] = cross(bp, BC);
			barycentric_coordinates[1] = cross(cp, CA);
			barycentric_coordinates[2] = cross(ap, AB);
#ifdef MSAA
			vector2* sample_points = new vector2[4];
			sample_points[0] = pixel_coordinate + vector2{-0.25f, -0.25f};
			sample_points[1] = pixel_coordinate + vector2{-0.25f, 0.25f};
			sample_points[2] = pixel_coordinate + vector2{0.25f, -0.25f};
			sample_points[3] = pixel_coordinate + vector2{0.25f, 0.25f};
			float result = 0;
			float weight[3];
			for (int i = 0; i < 4; i++) {
				vector2 as = sample_points[i] - vertices[0].screen_space_position;
				vector2 bs = sample_points[i] - vertices[1].screen_space_position;
				vector2 cs = sample_points[i] - vertices[2].screen_space_position;
				weight[0] = cross(bs, BC);
				weight[1] = cross(cs, CA);
				weight[2] = cross(as, AB);
				if (weight[0] > 0 || weight[1] > 0 || weight[2] > 0)
					continue;
				else
					result += 0.25f;
			}
			if (result == 0)
				continue;
#endif // MSAA

#ifndef MSAA
			///Take care of the sign.(ABC is aligned as anti-clockwise. clockwise vertices are culled already.).
			if (barycentric_coordinates[0] > 0 || barycentric_coordinates[1] > 0 || barycentric_coordinates[2] > 0)
				continue;
#endif // !MSAA

			barycentric_coordinates[0] *= inverse_area;
			barycentric_coordinates[1] *= inverse_area;
			barycentric_coordinates[2] *= inverse_area;
			bool ishidden = depth_test(barycentric_coordinates, vertices, x, y, fb, depth_test_buffer_type);
			if (ishidden)
				continue;
			else {
				shader_context fragments_input;
				set_fragment_input(&fragments_input, vertices, barycentric_coordinates);
				//Pixel position does not need perspective correction.
				fragments_input.write_data_v2(vector2{ (float)x,(float)y }, PIXEL_POSITION);
#ifdef MSAA
				fragments_input.write_data_f(result, ALPHA_VALUE);
#endif // MSAA
				fragments_inputs.push_back(fragments_input);
			}
		}
	}
	return fragments_inputs;
}
