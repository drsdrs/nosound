#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "color12.h"

typedef struct rgb {
  float r, g, b;
} RGB;

typedef struct hsl {
  float h, s, l;
} HSL;


/* Converts an HUE to r, g or b. returns float in the set [0, 1]. */
float hue2rgb(float p, float q, float t) {
  if (t < 0) t += 1;
  if (t > 1) t -= 1;
  if (t < 1./6) return p + (q - p) * 6 * t;
  if (t < 1./2) return q;
  if (t < 2./3) return p + (q - p) * (2./3 - t) * 6;
  return p;
}


/* Converts an HSL color value to RGB. Conversion formula adapted from http://en.wikipedia.org/wiki/HSL_color_space.
Assumes h, s, and l are contained in the set [0, 1] and returns RGB in the set [0, 255]. */
RGB hsl2rgb(float h, float s, float l) {
  RGB result;

  if(0 == s) {
    result.r = result.g = result.b = l; // achromatic
  } else {
    float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    float p = 2 * l - q;
    result.r = hue2rgb(p, q, h + 1./3) * 255;
    result.g = hue2rgb(p, q, h) * 255;
    result.b = hue2rgb(p, q, h - 1./3) * 255;
  }
  return result;
}


RGB hslToRgb( float h, float s, float l ){
    float r, g, b;
		RGB res;

    if(s == 0){
        r = g = b = l; // achromatic
    } else {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

		res.r = (r * 255);
		res.g = (g * 255);
		res.b = (b * 255);
    return res;
}

uint16_t color12_from_rgb( uint8_t r, uint8_t g, uint8_t b ){
	return ((r>>4)<<8) | ((g>>4)<<4) | (b>>4);
}

uint16_t color12_from_hsl( float h, float s, float l ){
	RGB col = hsl2rgb( h, s, l);
	//printf("col12 from hsl: %f %f %f\n", col.r, col.g, col.b);
	return ((((uint8_t)col.r)>>4)<<8) | ((((uint8_t)col.g)>>4)<<4) | (((uint8_t)col.b)>>4);
}
