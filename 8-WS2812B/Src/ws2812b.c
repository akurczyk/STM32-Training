#include "ws2812b.h"

ws2812b_config ws2812b_init(SPI_HandleTypeDef * spi_handler,
		uint16_t diodes_count) {
	ws2812b_config config;
	config.spi_handler = spi_handler;
	config.diodes_count = diodes_count;
	config.colors_array = calloc(diodes_count, sizeof(ws2812b_color));
	return config;
}

void ws2812b_set_diode_color(ws2812b_config * config, uint16_t diode_id,
		ws2812b_color color) {
	config->colors_array[diode_id] = color;
}

void ws2812b_refresh(ws2812b_config * config) {
	const uint8_t zero = 0b00011111;
	const uint8_t one = 0b00000111;

	uint8_t buffer[30 * 24];

	for (uint16_t i = 0, j = 0; i < config->diodes_count; i++) {
		// Green
		for (int16_t k = 7; k >= 0; k--) {
			if ((config->colors_array[i].green & (1 << k)) == 0)
				buffer[j] = one;
			else
				buffer[j] = zero;
			j++;
		}

		// Red
		for (int16_t k = 7; k >= 0; k--) {
			if ((config->colors_array[i].red & (1 << k)) == 0)
				buffer[j] = one;
			else
				buffer[j] = zero;
			j++;
		}

		// Blue
		for (int16_t k = 7; k >= 0; k--) {
			if ((config->colors_array[i].blue & (1 << k)) == 0)
				buffer[j] = one;
			else
				buffer[j] = zero;
			j++;
		}
	}

	HAL_SPI_Transmit(config->spi_handler, &buffer, config->diodes_count * 24,
			1000);
	HAL_Delay(1);
}
