#pragma once


static uint16_t color12_palette[16] = { // source >> lospec.com, SWEETIE 16, created by GrafxKid.
  0x112, 0x525, 0xb35, 0xe75,
  0xfc7, 0xaf7, 0x3b6, 0x277,
  0x236, 0x35c, 0x4af, 0x7ef,
  0xfff, 0x9bc, 0x568, 0x335
};

extern uint16_t color12_from_rgb( uint8_t r, uint8_t g, uint8_t b );

extern uint16_t color12_from_hsl( float h, float s, float l );
