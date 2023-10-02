#ifndef _MINIRENDERER_TEXTURE_H_
#define _MINIRENDERER_TEXTURE_H_
#include <cstdint>
#include "../externals/lodepng/lodepng.h"
#include "../utilities/image.h"
#include "../math/vector.h"

enum texture_format {
    ///
    /// The format has only an R component, the type is 8-bit unsigned integer.
    ///
    TEXTURE_FORMAT_R8,
    ///
    /// The components included in this format are R, G, B, and each component
    /// is an 8-bit unsigned integer type.
    ///
    TEXTURE_FORMAT_RGB8,
    ///
    /// The components included in this format are R, G, B, each component is an
    /// 8-bit unsigned integer type. And the three components are considered to
    /// be encoded in the sRGB color space.
    ///
    TEXTURE_FORMAT_SRGB8,
    ///
    /// The components included in this format are R, G, B, A, and each
    /// component is an 8-bit unsigned integer type.
    ///
    TEXTURE_FORMAT_RGBA8,
    ///
    /// The components included in this format are R, G, B, A, each component is
    /// an 8-bit unsigned integer type. And the color values of the three
    /// components of R, G, and B are considered to be encoded in the sRGB color
    /// space.
    ///
    TEXTURE_FORMAT_SRGB8_A8,
    ///
    /// The format used to store depth information, the type is float.
    ///
    TEXTURE_FORMAT_DEPTH_FLOAT
};
class texture
{
public:
    //Constructor and deconstructor.
    
    /// <summary>
    /// Default constructor.
    /// </summary>
    texture();
    /// <summary>
    /// Create a new texture. Usually used for buffer.
    /// Float texture will be initialized to 1.0f per pixel.
    /// Color texture will be initialized to 255 per chanel.
    /// </summary>
    /// <param name="internal_format"></param>
    /// <param name="width"></param>
    /// <param name="height"></param>
    texture(texture_format internal_format, uint32_t width, uint32_t height);
    /// <summary>
    /// Create a SRGB8_A8 Texture and init the texture to given color.
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <param name="color"></param>
    texture(uint32_t width, uint32_t height, vector4 color);
    /// <summary>
    /// Read texture from a file.
    /// </summary>
    /// <param name="filepath"></param>
    /// <param name="internal_format"></param>
    texture(const char* filepath, texture_format internal_format);
    ~texture();

    uint32_t get_texture_width() const;
    uint32_t get_texture_height() const;
    /// <summary>
    /// Return m_pixels.
    /// </summary>
    /// <returns></returns>
    void* get_texture_pixels() const;
    /// <summary>
    /// Sample texture by uv coordinate. Repeat mode. 
    /// </summary>
    /// <param name="uv"></param>
    /// <returns></returns>
    vector4 sample2D(vector2 uv);
private:
    enum texture_format m_format;
    uint32_t m_width, m_height;
    void* m_pixels;
};
#endif // !_MINIRENDERER_TEXTURE_H_
