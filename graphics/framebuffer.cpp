#include "framebuffer.h"
#include <string>
//Public:
int framebuffer::get_buffer_width() const { return m_width; }
int framebuffer::get_buffer_height() const { return m_height; }

framebuffer::framebuffer()
{
	m_width = UINT32_MAX;
	m_height = UINT32_MAX;
	m_depth_buffer = nullptr;
	m_color_buffer = nullptr;
}

framebuffer::~framebuffer()
{
}

bool framebuffer::attach_texture(attachment_type attachment, texture& texture) {
	bool result = false;
	if (texture.get_texture_pixels() != nullptr) {
		m_width = uint32_min(texture.get_texture_width(), m_width);
		m_height = uint32_min(texture.get_texture_height(), m_height);
		switch (attachment)
		{
			case COLOR_ATTACHMENT:
				m_color_buffer = (uint8_t*)(texture.get_texture_pixels());
				result = true;
				break;
			case DEPTH_ATTACHMENT:
				m_depth_buffer = (float*)(texture.get_texture_pixels());
				result = true;
				break;
			case SHADOW_ATTACHMENT:
				m_shadow_buffer = (float*)(texture.get_texture_pixels());
				result = true;
				break;
			default:
				result = false;
				break;
		}
	}
	return result;
}

void* framebuffer::get_buffer(attachment_type type) const {
	switch (type)
	{
	case COLOR_ATTACHMENT:
		return m_color_buffer;
	case DEPTH_ATTACHMENT:
		return m_depth_buffer;
	case SHADOW_ATTACHMENT:
		return m_shadow_buffer;
	default:
		break;
	}
}

void write_color(uint8_t* pixel, vector4 color, bool convert_to_srgb) {
	float r = float_clamp01(color.r);
	float g = float_clamp01(color.g);
	float b = float_clamp01(color.b);
	float a = float_clamp01(color.a);
	if (convert_to_srgb) {
		r = linear_to_srgb(r);
		g = linear_to_srgb(g);
		b = linear_to_srgb(b);
		a = linear_to_srgb(a);
	}
	*pixel = float_to_uint8(r);
	*(pixel + 1) = float_to_uint8(g);
	*(pixel + 2) = float_to_uint8(b);
	*(pixel + 3) = float_to_uint8(a);
}