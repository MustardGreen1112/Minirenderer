#include "texture.h"
/// <summary>
/// Return the byte size of a pixel. Ex: RGBA8 is 4 byte size.
/// </summary>
/// <param name="internal_format"></param>
/// <returns></returns>
static size_t get_pixel_size(texture_format internal_format) {
	size_t pixel_size;
	switch (internal_format)
	{
	case TEXTURE_FORMAT_R8:
		pixel_size = 1;
		break;
	case TEXTURE_FORMAT_RGB8:
	case TEXTURE_FORMAT_SRGB8:
		pixel_size = 3;
		break;
	case TEXTURE_FORMAT_RGBA8:
	case TEXTURE_FORMAT_SRGB8_A8:
		pixel_size = 4;
		break;
	case TEXTURE_FORMAT_DEPTH_FLOAT:
		pixel_size = sizeof(float);
		break;
	default:
		pixel_size = 0;
		break;
	}
	return pixel_size;
}
texture::texture() {
	m_format = TEXTURE_FORMAT_DEPTH_FLOAT;
	m_width = 0;
	m_height = 0;
	m_pixels = nullptr;
}

texture::texture(texture_format internal_format, uint32_t width, uint32_t height) {
	if (width == 0 || height == 0)
		texture();
	size_t pixel_count = width * height;
	size_t pixel_size = get_pixel_size(internal_format);
	m_format = internal_format;
	m_width = width;
	m_height = height;
	m_pixels = new size_t[pixel_count * pixel_size];
	if (internal_format != TEXTURE_FORMAT_DEPTH_FLOAT) {
		for (int i = 0; i < pixel_count * pixel_size; i++) {
			*((uint8_t*)m_pixels + i) = 255;
		}
	}
	else {
		for (int i = 0; i < pixel_count; i++) {
			*((float*)m_pixels + i) = 1.0f;
		}
	}
}

texture::texture(uint32_t width, uint32_t height, vector4 color) {
	if (width == 0 || height == 0)
		texture();
	size_t pixel_count = width * height;
	size_t pixel_size = get_pixel_size(TEXTURE_FORMAT_SRGB8_A8);
	m_format = TEXTURE_FORMAT_SRGB8_A8;
	m_width = width;
	m_height = height;
	m_pixels = new size_t[pixel_count * pixel_size];
	for (int i = 0; i < pixel_count * pixel_size; i++) {
		*((uint8_t*)m_pixels + i) = float_to_uint8(color.elements[i%4]);
	}
}

texture::texture(const char* filepath, texture_format internal_format) {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	//decode
	unsigned error = lodepng::decode(image, width, height, filepath);
	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
	m_format = internal_format;
	m_width = width;
	m_height = height;
	auto first = image.begin();
	auto end = image.end();
	size_t size = get_pixel_size(internal_format);
	uint8_t* result = new uint8_t[width * height * size];
	switch (internal_format)
	{
		case TEXTURE_FORMAT_R8:
		{
			int step = 0;
			for (std::vector<unsigned char>::iterator it = first; it != end; it += 4) {
				result[step] = *it;
				++step;
			}
			reverse_v(result, size, width, height);
			break;
		}
		case TEXTURE_FORMAT_RGB8:
		case TEXTURE_FORMAT_SRGB8:
		{
			int step = 0;
			int counter = 0;
			for (std::vector<unsigned char>::iterator it = first; it != end; ++it) {
				if (counter == 3) {
					counter = 0;
					continue;
				}
				result[step] = *it;
				++step;
				++counter;
			}
			reverse_v(result, size, width, height);
			break;
		}
		case TEXTURE_FORMAT_RGBA8:
		case TEXTURE_FORMAT_SRGB8_A8:
		{
			memcpy(result, &image[0], image.size() * sizeof(uint8_t));
			reverse_v(result, size, width, height);
			break;
		}
		case TEXTURE_FORMAT_DEPTH_FLOAT:
			break;
		default:
			break;
	}
	m_pixels = result;
}
texture::~texture() {
	delete[] m_pixels;
}

uint32_t texture::get_texture_width() const {
	return m_width;
}

uint32_t texture::get_texture_height()const {
	return m_height;
}

void* texture::get_texture_pixels()const {
	return m_pixels;
}

vector4 texture::sample2D(vector2 uv) {
	float u = uv.u - floorf(uv.u);
	float v = uv.v - floorf(uv.v);
	size_t col = u * m_width;
	size_t row = v * m_height;
	switch (m_format)
	{
		case TEXTURE_FORMAT_DEPTH_FLOAT:
		{
			float r = *((float*)m_pixels + (row * m_width + col));
			return vector4{ r, r, r, r };
		}
		case TEXTURE_FORMAT_R8:
		{
			uint8_t r = *((uint8_t*)m_pixels + (row * m_width + col));
			float rf = uint8_to_float(r);
			return vector4{ rf, 0, 0, 0 };
		}
		case TEXTURE_FORMAT_RGB8:
		{
			uint8_t r = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format));
			uint8_t g = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 1);
			uint8_t b = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 2);
			float rf = uint8_to_float(r);
			float gf = uint8_to_float(g);
			float bf = uint8_to_float(b);
			return vector4{ rf, gf, bf, 0 };
		}
		case TEXTURE_FORMAT_SRGB8:
		{
			uint8_t r = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format));
			uint8_t g = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 1);
			uint8_t b = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 2);
			float rf = uint8_to_float(r);
			float gf = uint8_to_float(g);
			float bf = uint8_to_float(b);
			return vector4{ srgb_to_linear(rf), srgb_to_linear(gf), srgb_to_linear(bf), 0 };
		}
		case TEXTURE_FORMAT_RGBA8:
		{
			uint8_t r = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format));
			uint8_t g = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 1);
			uint8_t b = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 2);
			uint8_t a = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 3);
			float rf = uint8_to_float(r);
			float gf = uint8_to_float(g);
			float bf = uint8_to_float(b);
			float af = uint8_to_float(a);
			return vector4{ rf, gf, bf, af };
		}
		case TEXTURE_FORMAT_SRGB8_A8:
		{
			uint8_t r = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format));
			uint8_t g = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 1);
			uint8_t b = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 2);
			uint8_t a = *((uint8_t*)m_pixels + (row * m_width + col) * get_pixel_size(m_format) + 3);
			float rf = uint8_to_float(r);
			float gf = uint8_to_float(g);
			float bf = uint8_to_float(b);
			float af = uint8_to_float(a);
			return vector4{ srgb_to_linear(rf), srgb_to_linear(gf), srgb_to_linear(bf), srgb_to_linear(af) };
		}
		default:
			break;
	}
}


