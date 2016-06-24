#ifndef lighting_header
#define lighting_header

#include "stm32f4xx_hal.h"
#include "ws2812b.h"

typedef struct lighting_rgb {
	double red, green, blue;
} lighting_rgb;

typedef struct lighting_hsv {
	double hue, saturation, value;
} lighting_hsv;

typedef struct lighting_config {
	uint16_t diodes_count;
	lighting_rgb * colors_array;
} lighting_config;

lighting_config lighting_init(uint16_t diodes_count);
lighting_rgb lighting_hsv2rgb(lighting_hsv in);
void lighting_set_diode_color_rgb(lighting_config * config, int diode_no,
		lighting_rgb rgb);
void lighting_set_diode_color_hsv(lighting_config * config, int diode_no,
		lighting_hsv hsv);
void lighting_draw_gradient_rgb(lighting_config * config, int from_diode,
		int to_diode, lighting_rgb from_color, lighting_rgb to_color);
void lighting_draw_gradient_hsv(lighting_config * config, int from_diode,
		int to_diode, lighting_hsv from_color, lighting_hsv to_color);
double lighting_gamma_correction(double in);
void lighting_update_ws2812b(lighting_config * lighting_c,
		ws2812b_config * ws2812b_c);

#endif
