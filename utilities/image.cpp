#include "image.h"

void reverse_v(uint8_t* buffer, const uint8_t size, const uint32_t width, const uint32_t height) {
	uint8_t* temp = new uint8_t[size];
	int flip_row_num = height / 2;
	for (int row = 0; row < flip_row_num; ++row) {
		for (int col = 0; col < width; ++col) {
			uint8_t* source = buffer + (row * width + col) * 4;
			uint8_t* target = buffer + (width * (height - row - 1) + col) * 4;
			memcpy(temp, source, size);
			memcpy(source, target, size);
			memcpy(target, temp, size);
		}
	}
	delete[] temp;
}
void save_image(const char* filepath, uint8_t* image, uint32_t width, uint32_t height) {
	//Reverse horizontally
	reverse_v(image, 4, width, height);
	//Encode the image
	unsigned error = lodepng::encode(filepath, image, width, height);
	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
