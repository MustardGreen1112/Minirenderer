#include "shader_context.h"

shader_context::shader_context()
{
	float_variables = variables<float, MAX_FLOAT_VARIABLES>();
	vector2_variables = variables<vector2, MAX_VECTOR2_VARIABLES>();
	vector3_variables = variables<vector3, MAX_VECTOR3_VARIABLES>();
	vector4_variables = variables<vector4, MAX_VECTOR4_VARIABLES>();
}

shader_context::~shader_context()
{
}