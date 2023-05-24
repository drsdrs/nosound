/**
 *  @file color12.h
 *  @brief 12 bit color struct
 */

#pragma once

/** @brief 12 bit color type
 * RGB - 4 bit per channel */ 
typedef uint16_t Color12;

/** @brief hold rgb values as 3 floats
 * 
 * range 0-255 */
typedef struct RGB {
  float r;  /**<Red */
  float g;  /**<Blue */
  float b;  /**<Green */
} RGB;

/** Color palette basic - 16 colors */
static Color12 color12_palette[16] = { // source >> lospec.com, SWEETIE 16, created by GrafxKid.
  0x112, 0x525, 0xb35, 0xe75,
  0xfc7, 0xaf7, 0x3b6, 0x277,
  0x236, 0x35c, 0x4af, 0x7ef,
  0xfff, 0x9bc, 0x568, 0x335
};

/** Converts an RGB color value to Color12.
 * @param r red range 0-255
 * @param b blue range 0-255
 * @param g green range 0-255
 * @return Color12 color */
extern Color12 color12_from_rgb( uint8_t r, uint8_t g, uint8_t b );

/** Converts HSL color value to RGB.
 * @param h hue range 0-1
 * @param s saturation range 0-1
 * @param l lightnessr ange 0-1
 * @return Color12 color */
extern Color12 color12_from_hsl( float h, float s, float l );
