#ifndef TFSS_FILE_CREATION_H
#define TFSS_FILE_CREATION_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c);

void unfilter_none(uint8_t *out, const uint8_t *filtered, size_t length);
void unfilter_sub(uint8_t *out, const uint8_t *filtered, size_t length, int bpp);
void unfilter_up(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length);
void unfilter_avg(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length, int bpp);
void unfilter_paeth(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length, int bpp);

void filter_none(uint8_t *filtered, const uint8_t *scanline, size_t length);
void filter_sub(uint8_t *filtered, const uint8_t *scanline, size_t length, int bpp);
void filter_up(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length);
void filter_avg(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length, int bpp);
void filter_paeth(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length, int bpp);

void get_tfss_resolution(const char* name, int* width, int* height);
void save_tfss_zstd(const char* name, uint8_t* data, int bytes_per_pixel, int width, int height, int image_format, char texture_name[32], int mip_count, int array_size, int compression_level);
void load_tfss_zstd(const char* name, uint8_t* data, int* bytes_per_pixel, int* width, int* height);

void load_tfss_index(const char* name, uint8_t* data, int* bytes_per_pixel, int* width, int* height, int index);

#endif
