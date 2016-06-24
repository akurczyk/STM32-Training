#include "lighting.h"

lighting_config lighting_init(uint16_t diodes_count) {
	lighting_config config;
	config.diodes_count = diodes_count;
	config.colors_array = calloc(diodes_count, sizeof(lighting_rgb));
	return config;
}

lighting_rgb lighting_hsv2rgb(lighting_hsv in) {
	double hh, p, q, t, ff;
	long i;
	lighting_rgb out;

	if (in.saturation <= 0.0) {
		out.red = in.value;
		out.green = in.value;
		out.blue = in.value;
		return out;
	}

	hh = in.hue;
	if (hh >= 360.0)
		hh = 0.0;
	hh /= 60.0;

	i = (long) hh;
	ff = hh - i;

	p = in.value * (1.0 - in.saturation);
	q = in.value * (1.0 - (in.saturation * ff));
	t = in.value * (1.0 - (in.saturation * (1.0 - ff)));

	switch (i) {
	case 0:
		out.red = in.value;
		out.green = t;
		out.blue = p;
		break;
	case 1:
		out.red = q;
		out.green = in.value;
		out.blue = p;
		break;
	case 2:
		out.red = p;
		out.green = in.value;
		out.blue = t;
		break;

	case 3:
		out.red = p;
		out.green = q;
		out.blue = in.value;
		break;
	case 4:
		out.red = t;
		out.green = p;
		out.blue = in.value;
		break;
	case 5:
	default:
		out.red = in.value;
		out.green = p;
		out.blue = q;
		break;
	}
	return out;
}

void lighting_set_diode_color_rgb(lighting_config * config, int diode_no,
		lighting_rgb rgb) {
	config->colors_array[diode_no].red = rgb.red;
	config->colors_array[diode_no].green = rgb.green;
	config->colors_array[diode_no].blue = rgb.blue;
}

void lighting_set_diode_color_hsv(lighting_config * config, int diode_no,
		lighting_hsv hsv) {
	lighting_rgb rgb = lighting_hsv2rgb(hsv);
	lighting_set_diode_color_rgb(config, diode_no, rgb);
}

void lighting_draw_gradient_rgb(lighting_config * config, int from_diode,
		int to_diode, lighting_rgb from_color, lighting_rgb to_color) {
	for (int i = from_diode; i <= to_diode; i++) {
		double percent = (double) (i - from_diode)
				/ (double) (to_diode - from_diode);
		lighting_rgb color;
		color.red = from_color.red + percent * (to_color.red - from_color.red);
		color.green = from_color.green
				+ percent * (to_color.green - from_color.green);
		color.blue = from_color.blue
				+ percent * (to_color.blue - from_color.blue);
		lighting_set_diode_color_rgb(config, i, color);
	}
}

void lighting_draw_gradient_hsv(lighting_config * config, int from_diode,
		int to_diode, lighting_hsv from_color, lighting_hsv to_color) {
	lighting_rgb from_rgb = lighting_hsv2rgb(from_color);
	lighting_rgb to_rgb = lighting_hsv2rgb(to_color);
	lighting_draw_gradient_rgb(config, from_diode, to_diode, from_rgb, to_rgb);
}

double lighting_gamma_correction(double in) {
	return powf(in, 2.2);
}

void lighting_update_ws2812b(lighting_config * lighting_c,
		ws2812b_config * ws2812b_c) {
	for (int i = 0; i < ws2812b_c->diodes_count; i++) {
		ws2812b_c->colors_array[i].red = (uint8_t) (lighting_gamma_correction(
				lighting_c->colors_array[i].red) * 255);
		ws2812b_c->colors_array[i].green = (uint8_t) (lighting_gamma_correction(
				lighting_c->colors_array[i].green) * 255);
		ws2812b_c->colors_array[i].blue = (uint8_t) (lighting_gamma_correction(
				lighting_c->colors_array[i].blue) * 255);
	}
}
