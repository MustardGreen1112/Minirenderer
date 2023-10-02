#include "mesh.h"
Mesh::Mesh(vector3* positions, uint32_t* positionIndexArray,
	vector3* normals, vector2* uvs, vector4* tangents,
	uint32_t vertex_count, uint32_t triangle_count,const char* name)
{
	m_positions = positions;
	m_position_indices = positionIndexArray;
	m_normals = normals;
	m_uvs = uvs;
	m_tangents = tangents;
	m_vertex_count = vertex_count;
	m_triangle_count = triangle_count;
	int length = strlen(name) + 1;
	m_name = new char[length];
	memcpy(m_name, name, sizeof(char) * length);
}

Mesh::~Mesh()
{
	delete[] m_positions;
	delete[] m_position_indices;
	delete[] m_normals;
	delete[] m_uvs;
	delete[] m_tangents;
	delete[] m_name;
}

vector3 Mesh::getPosition(const uint32_t triangle_index, const uint32_t vertex_index) const {
	uint32_t index = 3 * triangle_index + vertex_index;
	uint32_t positionIndex = m_position_indices[index];
	return m_positions[positionIndex];
}
vector3 Mesh::getNormal(const uint32_t triangle_index, const uint32_t vertex_index) const {
	return m_normals[3 * triangle_index + vertex_index];
}
vector2 Mesh::getUV(const uint32_t triangle_index, const uint32_t vertex_index) const {
	return m_uvs[3 * triangle_index + vertex_index];
}
vector4 Mesh::getTangent(const uint32_t triangle_index, const uint32_t vertex_index) const {
	return m_tangents[3 * triangle_index + vertex_index];
}


