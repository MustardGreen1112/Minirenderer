#ifndef _MINIRENDERER_FRAMEBUFFER_H_
#define _MINIRENDERER_FRAMEBUFFER_H_
#include "texture.h"
#include "../math/math_utility.h"
#include <cfloat>
#include "../math/vector.h"
enum attachment_type
{
	COLOR_ATTACHMENT,
	DEPTH_ATTACHMENT,
	SHADOW_ATTACHMENT,
};
class framebuffer
{
public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	framebuffer();
	/// <summary>
	/// Deconstructor.
	/// </summary>
	~framebuffer();
	/// <summary>
	/// Attach texture to framebuffer, update framebuffer size.
	/// When all buffer attached, framebuffer will be initialized.
	/// Basically it need two texture: depth buffer and color buffer.
	/// </summary>
	/// <param name="attachment"></param>
	/// <param name="texture"></param>
	/// <returns></returns>
	bool attach_texture(attachment_type attachment, texture& texture);
	/// <summary>
	/// Return raw pixel pointer of buffer.
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	void* get_buffer(attachment_type type) const;
	int get_buffer_width() const;
	int get_buffer_height() const;
private:
	uint32_t m_width, m_height;
	float* m_depth_buffer;
	float* m_shadow_buffer;
	uint8_t* m_color_buffer;
};
/// <summary>
/// Write color into color buffer.
/// </summary>
/// <param name="pixel"></param>
/// <param name="color"></param>
void write_color(uint8_t* pixel, vector4 color, bool convert_to_srgb);
#endif // !_MINIRENDERER_FRAMEBUFFER_H_
