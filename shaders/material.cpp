#include "material.h"
static int index;
material::material()
{
	m_index = index;
	index++;
	m_color = vector4{ 1.0f,1.0f,1.0f,1.0f };
	m_diffuse = nullptr;
	m_normal = nullptr;
	m_specular = nullptr;
	m_gloss = 1;
	m_transform_xy_tiling_zw_offset = vector4{ 1.0f,1.0f, 0,0 };
}

material::~material()
{
	index--;
}

material::material(vector4 color, float gloss, texture* diffuse, texture* normal, texture* specular, vector4 uv_transform) {
	m_index = index;
	index++;
	m_color = color;
	m_diffuse = diffuse;
	m_normal = normal;
	m_specular = specular;
	m_gloss = gloss;
	m_transform_xy_tiling_zw_offset = uv_transform;
}

material& material::operator=(const material& mat) {
	m_index = index;
	index++;
	m_color = mat.m_color;
	m_diffuse = mat.m_diffuse;
	m_normal = mat.m_normal;
	m_specular = mat.m_specular;
	m_gloss = mat.m_gloss;
	m_transform_xy_tiling_zw_offset = mat.m_transform_xy_tiling_zw_offset;
	return *this;
}

material::material(const material& mat) {
	m_index = index;
	index++;
	m_color = mat.m_color;
	m_diffuse = mat.m_diffuse;
	m_normal = mat.m_normal;
	m_specular = mat.m_specular;
	m_gloss = mat.m_gloss;
	m_transform_xy_tiling_zw_offset = mat.m_transform_xy_tiling_zw_offset;
}

void material::SetColor(vector4 color) {
	m_color = color;
}

void material::SetDiffuseTexture(texture* pimage) {
	m_diffuse = pimage;
}

void material::SetNormalTexture(texture* pimage) {
	m_normal = pimage;
}

void material::SetSpecularTexture(texture* pimage) {
	m_specular = pimage;
}

void material::SetUVTransform(vector2 tiling, vector2 offset) {
	m_transform_xy_tiling_zw_offset = vector4{ tiling.x, tiling.y, offset.x, offset.y };
}

void material::SetColor(float gloss) {
	m_gloss = gloss;
}

