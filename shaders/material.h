#ifndef _MINIRENDERER_MATERIAL_H_
#define _MINIRENDERER_MATERIAL_H_
#include "../graphics/texture.h"
class material
{
public:
	material();
	~material();
	/// <summary>
	/// uv_transform.xy=> tiling, uv_transform.zw=>offset.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="diffuse"></param>
	/// <param name="normal"></param>
	/// <param name="specular"></param>
	/// <param name="uv_transform"></param>
	material(vector4 color,float gloss, texture* diffuse, texture* normal, texture* specular, vector4 uv_transform);
	material& operator=(const material& mat);
	material(const material& mat);
	void SetColor(vector4 color);
	void SetColor(float gloss);
	void SetDiffuseTexture(texture* pimage);
	void SetNormalTexture(texture* pimage);
	void SetSpecularTexture(texture* pimage);
	void SetUVTransform(vector2 tiling, vector2 offset);
	inline texture* GetDiffuseTexutre() const { return m_diffuse; }
	inline texture* GetNormalTexture() const { return m_normal; }
	inline texture* GetSpecularTexture() const { return m_specular; }
	inline float GetGlossValue() const { return m_gloss; }
	inline vector4 GetColor() const { return m_color; }
	inline vector4 GetUVTransform() const { return m_transform_xy_tiling_zw_offset; }

private:
	int m_index;
	vector4 m_color;
	texture* m_diffuse;
	texture* m_normal;
	texture* m_specular;
	float m_gloss;
	vector4 m_transform_xy_tiling_zw_offset;
};



#endif // !_MINIRENDERER_MATERIAL_H_
