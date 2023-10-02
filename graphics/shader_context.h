#ifndef MINIRENDERER_SHADER_CONTEXT_H
#define MINIRENDERER_SHADER_CONTEXT_H

#include <stdbool.h>
#include <stdint.h>
#include"variables_template.h"
#include "../math/vector.h"

const int MAX_FLOAT_VARIABLES = 2;
const int MAX_VECTOR2_VARIABLES = 2;
const int MAX_VECTOR3_VARIABLES = 5;
const int MAX_VECTOR4_VARIABLES = 2;

class shader_context
{
public:
	shader_context();
	~shader_context();
	inline void write_data_f(float data, int8_t index){ float_variables.write(data, index); }
	inline void write_data_v2(vector2 data, int8_t index){ vector2_variables.write(data, index); }
	inline void write_data_v3(vector3 data, int8_t index){ vector3_variables.write(data, index); }
	inline void write_data_v4(vector4 data, int8_t index){ vector4_variables.write(data, index); }

	inline int8_t get_count_f(){ return float_variables.get_count(); }
	inline int8_t get_count_v2(){ return vector2_variables.get_count(); }
	inline int8_t get_count_v3(){ return vector3_variables.get_count(); }
	inline int8_t get_count_v4(){ return vector4_variables.get_count(); }

	inline int8_t* get_queue_f() { return float_variables.get_queue(); }
	inline int8_t* get_queue_v2() { return vector2_variables.get_queue(); }
	inline int8_t* get_queue_v3() { return vector3_variables.get_queue(); }
	inline int8_t* get_queue_v4() { return vector4_variables.get_queue(); }

	inline float read_data_f(int8_t index){ return *float_variables.read(index); }
	inline vector2 read_data_v2(int8_t index){ return *vector2_variables.read(index); }
	inline vector3 read_data_v3(int8_t index){ return *vector3_variables.read(index); }
	inline vector4 read_data_v4(int8_t index){ return *vector4_variables.read(index); }

private:
	variables<float, MAX_FLOAT_VARIABLES> float_variables;
	variables<vector2, MAX_VECTOR2_VARIABLES> vector2_variables;
	variables<vector3, MAX_VECTOR3_VARIABLES> vector3_variables;
	variables<vector4, MAX_VECTOR4_VARIABLES> vector4_variables;
};

#endif // !MINIRENDERER_SHADER_CONTEXT_H

