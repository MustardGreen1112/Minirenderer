#ifndef _MINIRENDERER_COLOR_H
#define _MINIRENDERER_COLOR_H
#include <corecrt_math.h>
#include <stdint.h>
#include "../math/vector.h"
#include "../shaders/constants.h"
const vector4 WHITECOLOR = vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
const vector4 BLACKCOLOR = vector4{ 0.0f, 0.0f, 0.0f, 1.0f };

//////////////////////////////////////
///
/// Gamma correction. 
/// 
//////////////////////////////////////
inline float linear_to_srgb(float value) { return powf(value, 1.0 / GAMMA); }
inline float srgb_to_linear(float value) { return powf(value, GAMMA); }
/// <summary>
/// Convert values from 0 - 1.0f to 0 - 255.
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline uint8_t float_to_uint8(float value) { return value * 0xFF; }
/// <summary>
/// Convert values from 0 - 255 to 0 - 1.0f  .
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline float uint8_to_float(uint8_t value) { return value / 255.0f; }
#endif // !_MINIRENDERER_COLOR_H
