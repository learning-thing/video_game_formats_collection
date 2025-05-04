#include "tfss_read_write.h"

#include <texture_filestorage_system.h>

#include <stdio.h>

#include <stb_image.h>
#include <zstd/zstd.h>

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c) {
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);

    if (pa <= pb && pa <= pc) return a;
    if (pb <= pc) return b;
    return c;
}

void unfilter_none(uint8_t *out, const uint8_t *filtered, size_t length) {
    memcpy(out, filtered, length);
}

void unfilter_sub(uint8_t *out, const uint8_t *filtered, size_t length, int bpp) {
    for (size_t i = 0; i < length; ++i) {
        uint8_t left = (i >= bpp) ? out[i - bpp] : 0;
        out[i] = filtered[i] + left;
    }
}

void unfilter_up(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        uint8_t up = prev ? prev[i] : 0;
        out[i] = filtered[i] + up;
    }
}

void unfilter_avg(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length, int bpp) {
    for (size_t i = 0; i < length; ++i) {
        uint8_t left = (i >= bpp) ? out[i - bpp] : 0;
        uint8_t up = prev ? prev[i] : 0;
        out[i] = filtered[i] + ((left + up) / 2);
    }
}

void unfilter_paeth(uint8_t *out, const uint8_t *filtered, const uint8_t *prev, size_t length, int bpp) {
    for (size_t i = 0; i < length; ++i) {
        uint8_t a = (i >= bpp) ? out[i - bpp] : 0;
        uint8_t b = prev ? prev[i] : 0;
        uint8_t c = (i >= bpp && prev) ? prev[i - bpp] : 0;
        out[i] = filtered[i] + paeth_predictor(a, b, c);
    }
}

void decode_scanline(uint8_t *out, const uint8_t *in, const uint8_t *prev, size_t length, int bpp) {
    uint8_t type = in[0];
    const uint8_t *filtered = in + 1;
    switch (type) {
        case 0: unfilter_none(out, filtered, length); break;
        case 1: unfilter_sub(out, filtered, length, bpp); break;
        case 2: unfilter_up(out, filtered, prev, length); break;
        case 3: unfilter_avg(out, filtered, prev, length, bpp); break;
        case 4: unfilter_paeth(out, filtered, prev, length, bpp); break;
        default: fprintf(stderr, "Unknown filter type: %d\n", type); break;
    }
}

void decode_image(uint8_t *out, const uint8_t *compressed_filtered_data, int width, int height, int bpp) {
    size_t row_length = width * bpp;
    const uint8_t *in_row = compressed_filtered_data;
    uint8_t *prev_row = NULL;

    for (int y = 0; y < height; y++) {
        uint8_t *out_row = out + y * row_length;
        decode_scanline(out_row, in_row, prev_row, row_length, bpp);
        prev_row = out_row;
        in_row += 1 + row_length; // 1 filter byte + row bytes
    }
}



TFSS load_tfss_header(const char* name)
{
    TFSS header = {0};
    FILE* file = fopen(name, "rb");
   
    char magic[4] = {0};
    fread(magic, sizeof(char), 4, file);

    uint32_t size = 0;
    fread(&size, sizeof(uint32_t), 4, file);

    for(int i = 0; i < size; i++)
    {
	
        uint32_t width;            // Texture width
        uint32_t height;           // Texture height
        uint32_t depth;            // 3D texture depth (optional, 0 if 2D)
        
        uint8_t mip_count;         // Number of mipmap levels
        uint8_t array_size;        // Number of layers (for texture arrays), if set to zero then it is only a single texture
        uint8_t image_format;       // Texture format (e.g., RGBA8, BC7)
        uint8_t color_format;      //  color format so number of colors

        uint32_t texture_size;             // Total size of the texture data in bytes

        fread(&width,  sizeof(uint32_t), 4, file);
        fread(&height, sizeof(uint32_t), 4, file);
        fread(&depth,  sizeof(uint32_t), 4, file);
        
        fread(&mip_count, sizeof(uint8_t), 4, file);
        fread(&array_size, sizeof(uint8_t), 4, file);
        fread(&image_format, sizeof(uint8_t), 4, file);
        fread(&color_format, sizeof(uint8_t), 4, file);

        fread(&texture_size,  sizeof(uint32_t), 4, file);

        // set file header here

        fseek(file, size, SEEK_CUR);
    }

    fclose(file);
    
    return header;
}

TFSS load_tfss_header_index(const char* name, int position)
{
    TFSS header = {0};
    FILE* file = fopen(name, "rb");
   
    char magic[4] = {0};
    fread(magic, sizeof(char), 4, file);

    uint32_t size = 0;
    fread(&size, sizeof(uint32_t), 4, file);


    for(int i = 0; i < position; i++)
    {
        fseek(file, 48, SEEK_CUR);
        uint32_t temp_read = 0;
        fread(&temp_read,  sizeof(uint32_t), 4, file);
        fseek(file, temp_read, SEEK_CUR);
    }
    

    uint32_t width;            // Texture width
    uint32_t height;           // Texture height
    uint32_t depth;            // 3D texture depth (optional, 0 if 2D)
    
    uint8_t mip_count;         // Number of mipmap levels
    uint8_t array_size;        // Number of layers (for texture arrays), if set to zero then it is only a single texture
    uint8_t image_format;       // Texture format (e.g., RGBA8, BC7)
    uint8_t color_format;      //  color format so number of colors

    uint32_t texture_size;             // Total size of the texture data in bytes

    fread(&width,  sizeof(uint32_t), 4, file);
    fread(&height, sizeof(uint32_t), 4, file);
    fread(&depth,  sizeof(uint32_t), 4, file);
    
    fread(&mip_count, sizeof(uint8_t), 4, file);
    fread(&array_size, sizeof(uint8_t), 4, file);
    fread(&image_format, sizeof(uint8_t), 4, file);
    fread(&color_format, sizeof(uint8_t), 4, file);

    fread(&texture_size,  sizeof(uint32_t), 4, file);

    fclose(file);
    
    return header;
}

void get_tfss_resolution(const char* name, int* width, int* height) {
    FILE* file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "error: file did not open correctly\n");
        return;
    }

    char magic[4] = {0};
    int width_buf = 0, height_buf = 0;
    fread(magic, sizeof(char), 4, file);
    if (memcmp(magic, "MTSS", 4) != 0) {
        fprintf(stderr, "error: invalid magic\n");
        fclose(file);
        return;
    }

    printf("%s\n", magic);

    fseek(file, 38, SEEK_CUR);
    
    fread(&width_buf, sizeof(int), 1, file);
    fread(&height_buf, sizeof(int), 1, file);
    //printf("%dx%d\n", *width, *height);
    
    fseek(file, 8, SEEK_CUR);

    uint8_t bpp_buf = 0;
    fread(&bpp_buf, sizeof(char), 1, file);
    fclose(file);
    *width = width_buf;
    *height = height_buf;
}

void load_tfss_index(const char* name, uint8_t* data, int* bytes_per_pixel, int* width, int* height, int index)
{
    FILE* file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "error: file did not open correctly\n");
        return;
    }

    char magic[4] = {0};
    fread(magic, sizeof(char), 4, file);
    if (memcmp(magic, "MTSS", 4) != 0) {
        fprintf(stderr, "error: invalid magic\n");
        fclose(file);
        return;
    }

    fseek(file, 36, SEEK_CUR);

    int width_buf = 0, height_buf = 0;
    fread(&width_buf, sizeof(int), 1, file);
    fread(&height_buf, sizeof(int), 1, file);

    printf("%dx%d\n", width_buf, height_buf);
    
    fseek(file, 7, SEEK_CUR);

    uint8_t bpp_buf = 0;
    fread(&bpp_buf, sizeof(char), 1, file);

    uint32_t file_size = 0;
    fread(&file_size, sizeof(uint32_t), 1, file);

    uint8_t* compressed_data = malloc(file_size);
    fread(compressed_data, 1, file_size, file);
    fclose(file);

    size_t row_len = width_buf * bpp_buf;
    size_t total_filtered_size = height_buf * (1 + row_len);

    uint8_t* filtered = malloc(total_filtered_size);
    size_t decompressed_size = ZSTD_decompress(filtered, total_filtered_size, compressed_data, file_size);
    free(compressed_data);

    if (ZSTD_isError(decompressed_size)) {
        fprintf(stderr, "ZSTD error: %s\n", ZSTD_getErrorName(decompressed_size));
        free(filtered);
        return;
    }

    for (int y = 0; y < height_buf; ++y) {
        uint8_t* prev = (y == 0) ? NULL : data + (y - 1) * row_len;
        decode_scanline(data + y * row_len, filtered + y * (1 + row_len), prev, row_len, bpp_buf);
    }

    free(filtered);

    *width = width_buf;
    *height = height_buf;
    *bytes_per_pixel = bpp_buf;
}


