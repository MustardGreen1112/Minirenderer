#ifndef _MINIRENDERER_IMAGE_H_
#define _MINIRENDERER_IMAGE_H_
#include <cstdint>
#include "../externals/lodepng/lodepng.h"
#include <iostream>
#include "../color/color.h"

void save_image(const char* filepath, uint8_t* image, uint32_t width, uint32_t height);
void reverse_v(uint8_t* buffer, const uint8_t size, const uint32_t width, const uint32_t height);
#endif // !_MINIRENDERER_IMAGE_H_


