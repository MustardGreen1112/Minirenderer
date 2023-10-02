#ifndef _MINIRENDERER_VARIABLES_TEMPLATE_H_
#define _MINIRENDERER_VARIABLES_TEMPLATE_H_
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
template <typename Type, int max_variables_num>
class variables {
private:
	Type m_data[max_variables_num];
	bool m_allocations[max_variables_num];
	int8_t m_index_queue[max_variables_num];
	int8_t m_count;
public:
	variables();
	void write(const Type data, int8_t index);
	const Type* read(int8_t index) const;
	int8_t* get_queue();
	int8_t get_count();
	//void clear();
};
//template<typename Type, int max_variables_num>
//void variables<Type, max_variables_num>::clear() {
//	for (int8_t i = 0; i < max_variables_num; i++) {
//		m_allocations[i] = false;
//	}
//	m_count = 0;
//	m_data = Type();
//}

template<typename Type, int max_variables_num>
variables<Type, max_variables_num>::variables() {
	for (int8_t i = 0; i < max_variables_num; i++) {
		m_allocations[i] = false;
	}
	m_count = 0;
}

template<typename Type, int max_variables_num>
void variables<Type, max_variables_num>::write(const Type data, int8_t index) {
	if (index >= max_variables_num)
		return;
	if (!m_allocations[index])
	{
		m_data[index] = data;
		m_allocations[index] = true;
		m_index_queue[m_count] = index;
		m_count++;
	}
}

template<typename Type, int max_variables_num>
const Type* variables<Type, max_variables_num>::read(int8_t index) const {
	if (index >= max_variables_num || m_allocations[index] == false) {
		return nullptr;
	}
	return m_data + index;
}

template<typename Type, int max_variables_num>
int8_t* variables<Type, max_variables_num>::get_queue() {
	return m_index_queue;
}

template<typename Type, int max_variables_num>
int8_t variables<Type, max_variables_num>::get_count() {
	return m_count;
}
#endif // !_MINIRENDERER_VARIABLES_TEMPLATE_H_
