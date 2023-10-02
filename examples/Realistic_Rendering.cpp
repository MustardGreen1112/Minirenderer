#include "../math/vector.h"
#include "../math/matrix.h"
#include "../utilities/mesh.h"
#include "../shaders/standard_vertex_shader.h"
#include "../graphics/vertex_shader.h"
#include "../graphics/clipping.h"
#include "../graphics/rasterizer.h"
#include "../graphics/shader_context.h"
#include "../graphics/fragment_shader.h"
#include "../shaders/standard_fragment_shader.h"
#include "../shaders/standard_vertex_shader.h"
#include "../externals/lodepng/lodepng.h"
#include <iostream>
#include "../utilities/image.h"
#include "../shaders/material.h"
#include "../shaders/shadow_casting.h"
#include "../utilities/model.h"


void render_shadowmap(Mesh* pmesh, standard_uniform& uniform, framebuffer* pframebuffer, int triangle_count) {
	for (int t = 0; t < triangle_count; t++) {
		vertex vertices[3];
		bool clipped = false;
		standard_vertex_attribute attributes[3];
		bounding_box bound{ {{FLT_MAX, FLT_MAX}}, {{FLT_MIN, FLT_MIN}} };
		for (uint32_t v = 0; v < 3; v++) {
			attributes[v].position = pmesh->getPosition(t, v);
			vertex* vertex = vertices + v;
			//Vertex shader:
				//position is clip-space position.
			vertex->position = proceed_vertex_shader(vertex->context, &uniform, (void*)&(attributes[v]));
			//Clipping:
			clipped = clipping_test(vertex->position);
			if (clipped)
				continue;
			//NDC position. Change vertex.position to NDC position.
			perspective_division(*vertex);
			//Screen position saved to vertex->screen_space_position. Change vertex.screenspace position.
			viewport_transform(*vertex);
			//Update bounding box.
			update_bouding_box(*vertex, bound);
		}
		if (clipped)
			continue;
		//Cull the back-faced triangle.
		float area;
		bool culled = culling(vertices, area);
		if (culled)
			continue;
		//---------Back  End-------------------------------------------------------------------//
		//Rasterize:
		std::vector<shader_context>* fragment_inputs = new std::vector<shader_context>;
		*fragment_inputs = rasterize(vertices, bound, area, *pframebuffer, SHADOW_ATTACHMENT);
		delete fragment_inputs;
	}
}
void render_model(Mesh* pmesh, standard_uniform& uniform, framebuffer* pframebuffer, int triangle_count, material& mat) {
	uint8_t* color_buffer = (uint8_t*)(pframebuffer->get_buffer(COLOR_ATTACHMENT));
	for (int t = 0; t < triangle_count; t++) {
		vertex vertices[3];
		bool clipped = false;
		standard_vertex_attribute attributes[3];
		bounding_box bound{ {{FLT_MAX, FLT_MAX}}, {{FLT_MIN, FLT_MIN}} };
		for (uint32_t v = 0; v < 3; v++) {
			vertex* vert = vertices + v;
			attributes[v].position = pmesh->getPosition(t, v);
			attributes[v].normal = pmesh->getNormal(t, v);
			attributes[v].uv = pmesh->getUV(t, v);
			attributes[v].tangent = pmesh->getTangent(t, v);
//Vertex shader:
			//vert->position is clip-space position.
			vert->position = proceed_vertex_shader(vert->context, &uniform, (void*)&(attributes[v]));
//Clipping:
			clipped = clipping_test(vert->position);
			if (clipped)
				break;
//NDC position. Change vertex.position to NDC position.
			perspective_division(*vert);
//Screen position saved to vertex->screen_space_position. Change vertex.screenspace position.
			viewport_transform(*vert);
//Update bounding box.
			update_bouding_box(*vert, bound);
		}
		if (clipped)
			continue;
//Cull the back-faced triangle.
		float area;
		bool culled = culling(vertices, area);
		if (culled)
			continue;
//---------Back  End-------------------------------------------------------------------//
//Rasterize:
		std::vector<shader_context>* fragment_inputs = new std::vector<shader_context>;
		*fragment_inputs = rasterize(vertices,bound,area, *pframebuffer, DEPTH_ATTACHMENT);
		
//Fragment shader:
		auto first = fragment_inputs->begin();
		auto end = fragment_inputs->end();
		for (std::vector<shader_context>::iterator it = first; it != end; ++it) {
			vector4 fragment_color = proceed_fragment_shader(*it, &uniform, mat);
			if (color_buffer != nullptr) {
				vector2 pixel_position = (*it).read_data_v2(PIXEL_POSITION);
				int frame_buffer_width = pframebuffer->get_buffer_width();
				//color_buffer is an uint8_t array arranged as r, g, b, a, r, g, b, a......
				//So everytime offset the pointer at 4* sizeof(uint8_t), pointer will point at r.
				uint8_t* pixel = color_buffer + ((int)pixel_position.y * frame_buffer_width + (int)pixel_position.x) * 4;
				write_color(pixel, fragment_color, true);
			}
		}
		delete fragment_inputs;
	}
}

int main()
{
	//Set output size.
	set_viewport(1024, 1024);
	//Initialize framebuffer.
	framebuffer* pframebuffer = new framebuffer();
	texture* pdepth_texture = new texture(TEXTURE_FORMAT_DEPTH_FLOAT, 1024, 1024);
	texture* pcolor_texture = new texture(1024, 1024,vector4{ 0.647f,0.839f,1.0f,1.0f});
	texture* pshadow_texture = new texture(TEXTURE_FORMAT_DEPTH_FLOAT, 1024, 1024);
	pframebuffer->attach_texture(DEPTH_ATTACHMENT, *pdepth_texture);
	pframebuffer->attach_texture(COLOR_ATTACHMENT, *pcolor_texture);
	pframebuffer->attach_texture(SHADOW_ATTACHMENT, *pshadow_texture);
	uint8_t* color_buffer = (uint8_t*)pframebuffer->get_buffer(COLOR_ATTACHMENT);
	float* depth_buffer = (float*)pframebuffer->get_buffer(DEPTH_ATTACHMENT);
	float* shadow_buffer = (float*)pframebuffer->get_buffer(SHADOW_ATTACHMENT);
	//Create materials and textures.
	texture* minirenderer_diffuse = new texture("assets/minirenderer_diffuse.png", TEXTURE_FORMAT_SRGB8_A8);
	material* minirenderer_material = new material(WHITECOLOR, 7.0f, minirenderer_diffuse, nullptr, nullptr, vector4{ 1.0f,1.0f,0,0 });

	texture* minirender_wall_diffuse = new texture("assets/minirenderer_wall_diffuse.png", TEXTURE_FORMAT_SRGB8_A8);
	texture* minirender_wall_normal = new texture("assets/minirenderer_wall_normal.png", TEXTURE_FORMAT_RGBA8);
	material* minirender_wall_material = new material(WHITECOLOR, 1.0f, minirender_wall_diffuse, minirender_wall_normal, nullptr, vector4{ 1.0f,1.0f,0,0 });

	material* minirender_cloud_material = new material(vector4{ 0.217f,0.358f,0.800f, 1.0f }, 1.9f, nullptr, nullptr, nullptr, vector4{ 1.0f,1.0f,0,0 });

	material* default_material = new material(WHITECOLOR, 1.0f, nullptr, nullptr, nullptr, vector4{ 1.0f,1.0f,0,0 });

	//Set uniforms.
	standard_uniform uniform;
	vector3 camera_target = VECTOR3_ZERO;
	uniform.camera_position = vector3{ 6.0f, 20.0f, 19.0f };
	vector3 light_position = vector3{ 20.0f, 15.0f, -20.0f };
	uniform.light_direction = normalize(light_position) * (-1.0f);
	//There will be no translate or transform if local2world and local2world_normal are both I.
	uniform.local2world = MATRIX4x4_IDENTITY;
	uniform.local2world_normal = MATRIX3x3_IDENTITY;

	mat4x4 world2view = mat4x4_W2V(uniform.camera_position, camera_target, vector3{ 0,1.0f,0 });
	mat4x4 view2clip = mat4x4_perspective(39.6f / 180.0f * PI, 1.0f, 0.1f, 50.0f);
	mat4x4 world2clip = mul(view2clip, world2view);
	uniform.world2clip = world2clip;

	uniform.local2world_direction = MATRIX3x3_IDENTITY;
	uniform.world2light = mat4x4_W2V(light_position, VECTOR3_ZERO, vector3{ 0,1.0f,0 });
	uniform.light2clip = mat4x4_orthographic(7.0f, 7.0f, 0.1f, 100.0f);
	uniform.shadowmap = pshadow_texture;
	uniform.ambientColor = vector4{ 0.647f,0.839f,1.0f, 0.02f };

	//Load model from fbx to data stream.
	Model model = Model::LoadModel("assets/minirenderer.fbx");

	//Render shadow map:
	set_vertex_shader(shadow_casting);
	set_fragment_shader(nullptr);
	for (int meshId = 0; meshId < model.GetMeshCount(); meshId++) {
		Mesh* mesh = model.m_pmesharr[meshId];
		int triangle_count = mesh->getTriangleCount();
		render_shadowmap(mesh, uniform, pframebuffer, triangle_count);
	}
	//Output shadow map:
	if (shadow_buffer != nullptr) {
		uint8_t* shadowMap = new uint8_t[1024 * 1024 * 4];
		for (int i = 0; i < 1024 * 1024; i++) {
			float val_f = shadow_buffer[i];
			uint8_t val = float_to_uint8(val_f);
			*(shadowMap + 4 * i) = val;
			*(shadowMap + 4 * i + 1) = val;
			*(shadowMap + 4 * i + 2) = val;
			*(shadowMap + 4 * i + 3) = 255;
		}
		save_image("output/realistic_rendering_shadowmap.png", shadowMap, 1024, 1024);
		delete[] shadowMap;
	}

	//Render Models:
	set_vertex_shader(standard_vertex_shader);
	set_fragment_shader(standard_fragment_shader);
	for (int meshId = 0; meshId < model.GetMeshCount(); meshId++) {
		Mesh* pmesh = model.m_pmesharr[meshId];
		uint32_t triangle_count = pmesh->getTriangleCount();
		const char* name = pmesh->getName();
		if (strcmp(name,"Plane")==0) {
			render_model(pmesh, uniform, pframebuffer, triangle_count, *minirender_wall_material);
		}
		else if (strcmp(name, "Cloud")==0) {
			render_model(pmesh, uniform, pframebuffer, triangle_count, *minirender_cloud_material);
		}
		else if (strcmp(name, "Characters")==0) {
			render_model(pmesh, uniform, pframebuffer, triangle_count, *minirenderer_material);
		}
		else {
			render_model(pmesh, uniform, pframebuffer, triangle_count, *default_material);
		}

	}
	const char* output_png_file = "output/realistic_rendering.png";
	if (color_buffer != nullptr) {
		save_image(output_png_file, color_buffer, 1024, 1024);
	}

	delete pframebuffer;
	delete pcolor_texture;
	delete pdepth_texture;
	delete pshadow_texture;
	delete minirenderer_material;
	delete minirenderer_diffuse;
	delete default_material;
	delete minirender_wall_material;
	delete minirender_wall_diffuse;
	delete minirender_wall_normal;

}


