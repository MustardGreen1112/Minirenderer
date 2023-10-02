#ifndef MINIRENDERER_UTILITIES_MESH_H
#define MINIRENDERER_UTILITIES_MESH_H

#include <stdint.h>
#include "../math/vector.h"
#include <fbxsdk.h>
class Mesh
{
public:
	Mesh(vector3* positions, uint32_t* positionIndexArray,
		vector3* normals, vector2* uvs, vector4* tangents, 
		uint32_t vertex_count, uint32_t triangle_count, const char* name);
	~Mesh();
	vector3 getPosition(const uint32_t triangle_index, const uint32_t vertex_index) const;
	vector3 getNormal(const uint32_t triangle_index, const uint32_t vertex_index) const;
	vector2 getUV(const uint32_t triangle_index, const uint32_t vertex_index) const;
	vector4 getTangent(const uint32_t triangle_index, const uint32_t vertex_index) const;
	inline uint32_t getVertexCount()const { return m_vertex_count; };
	inline uint32_t getTriangleCount()const { return m_triangle_count; };
	inline const char* getName() const { return m_name; };
private:
	vector3* m_positions;
	uint32_t* m_position_indices;
	vector2* m_uvs;
	vector3* m_normals;
	vector4* m_tangents;
	uint32_t m_vertex_count;
	uint32_t m_triangle_count;
	char* m_name;
};

#endif // ! MINIRENDERER_UTILITIES_MESH_H
