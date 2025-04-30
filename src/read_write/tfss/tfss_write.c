#include "tfss_read_write.h"
#include "texture_filestorage_system.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include "zstd/zstd.h"

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c) {
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);

    if (pa <= pb && pa <= pc) return a;
    if (pb <= pc) return b;
    return c;
}


// Filter Type 0 - None
void filter_none(uint8_t *filtered, const uint8_t *scanline, size_t length) {
    for (size_t i = 0; i < length; i++)
        filtered[i] = scanline[i];
}

// Filter Type 1 - Sub
void filter_sub(uint8_t *filtered, const uint8_t *scanline, size_t length, int bpp) {
    for (size_t i = 0; i < length; i++) {
        uint8_t left = (i >= bpp) ? scanline[i - bpp] : 0;
        filtered[i] = scanline[i] - left;
    }
}

// Filter Type 2 - Up
void filter_up(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length) {
    for (size_t i = 0; i < length; i++) {
        uint8_t up = prev ? prev[i] : 0;
        filtered[i] = scanline[i] - up;
    }
}

// Filter Type 3 - Average
void filter_avg(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length, int bpp) {
    for (size_t i = 0; i < length; i++) {
        uint8_t left = (i >= bpp) ? scanline[i - bpp] : 0;
        uint8_t up = prev ? prev[i] : 0;
        filtered[i] = scanline[i] - ((left + up) / 2);
    }
}

// Filter Type 4 - Paeth
void filter_paeth(uint8_t *filtered, const uint8_t *scanline, const uint8_t *prev, size_t length, int bpp) {
    for (size_t i = 0; i < length; i++) {
        uint8_t a = (i >= bpp) ? scanline[i - bpp] : 0;        // left
        uint8_t b = prev ? prev[i] : 0;                        // above
        uint8_t c = (i >= bpp && prev) ? prev[i - bpp] : 0;    // upper-left
        filtered[i] = scanline[i] - paeth_predictor(a, b, c);
    }
}

int select_best_filter(uint8_t *out, const uint8_t *scanline, const uint8_t *prev, size_t length, int bpp) {
    uint8_t temp[5][length];
    int32_t sums[5] = {0};

    filter_none(temp[0], scanline, length);
    filter_sub(temp[1], scanline, length, bpp);
    filter_up(temp[2], scanline, prev, length);
    filter_avg(temp[3], scanline, prev, length, bpp);
    filter_paeth(temp[4], scanline, prev, length, bpp);

    for (int f = 0; f < 5; ++f) {
        for (size_t i = 0; i < length; ++i)
            sums[f] += abs((int)(temp[f][i]));
    }

    int best_filter = 0;
    int32_t best_score = sums[0];
    for (int f = 1; f < 5; ++f) {
        if (sums[f] < best_score) {
            best_score = sums[f];
            best_filter = f;
        }
    }

    // Write best filtered scanline
    out[0] = (uint8_t)best_filter;
    memcpy(out + 1, temp[best_filter], length);
    return best_filter;
}

void save_tfss_zstd(const char* name, uint8_t* data, int bytes_per_pixel, int width, int height, int image_format, char texture_name[32], int mip_count, int array_size, int compression_level)
{
    int filtered_size;

    size_t row_length = width * bytes_per_pixel;
    size_t total_size = height * (1 + row_length);

    uint8_t* filtered = malloc(total_size);
    if (!filtered) return;
    
    uint8_t* prev = NULL;
    
    for(int y = 0; y < height; y++)
    {
        uint8_t* row = data + y * row_length;
        uint8_t* out_row = filtered + y * (1 + row_length);

        select_best_filter(out_row, row, prev, row_length, bytes_per_pixel);
        prev = row;
    }

    filtered_size = total_size;

    size_t max_compressed_size = ZSTD_compressBound(total_size);
    uint8_t* compressed_data = malloc(max_compressed_size);

    size_t compressed_size = ZSTD_compress((void*)compressed_data, max_compressed_size, (void*)filtered, total_size, compression_level);

    if (ZSTD_isError(compressed_size)) 
    {
        fprintf(stderr, "Compression failed: %s\n", ZSTD_getErrorName(compressed_size));
        free(filtered);
        free(compressed_data);
        return;
    }
    
    FILE* file = fopen(name, "wb");
    if (!file)
    {
        fprintf(stderr,"error, file did not create correctly");
        free(filtered);
        free(compressed_data);
        return;
    }

    fwrite((char[]){'M', 'T', 'S', 'S'}, 1, 4, file);

    uint32_t num_of_textures = 1;
    fwrite(&num_of_textures, sizeof(uint32_t), 1, file);

    fwrite(texture_name, 1, 32, file);
    
    fwrite(&width, sizeof(int), 1, file);
    fwrite(&height, sizeof(int), 1, file);
    for(int i = 0; i < 4; i++)
        fwrite((char[]){0}, 1, 1, file);
    uint8_t t_mip_count = (uint8_t)mip_count, t_array_size=(uint8_t)array_size, t_image_format=(uint8_t)image_format;
    
    fwrite(&t_mip_count, 1, 1, file);
    fwrite(&t_array_size, 1, 1, file);
    fwrite(&t_image_format, 1, 1, file);
    
    uint8_t channel = bytes_per_pixel;
    fwrite(&(channel), 1, 1, file);

    uint32_t final_size = (uint32_t)compressed_size;
    fwrite(&final_size, sizeof(uint32_t), 1, file);
    
    fwrite(compressed_data, 1, compressed_size, file);
    fclose(file);
    
    free(compressed_data);
    free(filtered);
}

int main() {
    int width = 0;
    int height = 0;
    int bpp = 3; // RGB
    int compression_level = 22;
    uint8_t* data = stbi_load("SamplePNGImage_30mbmb.png", &width, &height, &bpp, 0);
    size_t data_size = width * height * bpp;

    if (!data) {
        fprintf(stderr, "Failed to allocate image memory\n");
        return 1;
    }

    //generate_checkerboard(data, width, height, bpp);
    save_tfss_zstd("test.tfss", data, bpp, width, height, compression_level);
    stbi_image_free(data);

    printf("test.tfss saved successfully!\n");
    return 0;
}
