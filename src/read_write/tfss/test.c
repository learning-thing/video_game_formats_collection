#include "tfss_read_write.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef TEST_READ
int main() {
    const char* tfss_file = "test.tfss";
    const char* reference_png = "SamplePNGImage_30mbmb.png";

    int w_tfss = 0, h_tfss = 0, bpp_tfss = 0;
    int w_ref = 0, h_ref = 0, bpp_ref = 0;

    // Load the reference image
    uint8_t* expected_data = stbi_load(reference_png, &w_ref, &h_ref, &bpp_ref, 0);
    if (!expected_data) {
        fprintf(stderr, "Failed to load reference PNG image\n");
        return 1;
    }

    printf("Reference image: %dx%d, bpp=%d\n", w_ref, h_ref, bpp_ref);

    // Allocate output buffer
    uint8_t* image_data = malloc(w_ref * h_ref * bpp_ref);
    if (!image_data) {
        fprintf(stderr, "Failed to allocate memory for TFSS image\n");
        stbi_image_free(expected_data);
        return 1;
    }

    // Load TFSS image
    load_tfss_index(tfss_file, image_data, &bpp_tfss, &w_tfss, &h_tfss, 0);

    printf("TFSS image: %dx%d, bpp=%d\n", w_tfss, h_tfss, bpp_tfss);

    // Sanity check
    if (w_tfss != w_ref || h_tfss != h_ref || bpp_tfss != bpp_ref) {
        fprintf(stderr, "Size or format mismatch between images\n");
    }

    // Compare data
    size_t size = w_ref * h_ref * bpp_ref;
    int same = memcmp(image_data, expected_data, size) == 0;
    printf("Pixel data %s\n", same ? "MATCHES" : "DIFFERS");

    if (!same) {
        for (size_t i = 0; i < size; ++i) {
            if (image_data[i] != expected_data[i]) {
                printf("Mismatch at byte %zu: got %d, expected %d\n", i, image_data[i], expected_data[i]);
                break;
            }
        }
    }

    // Cleanup
    free(image_data);
    stbi_image_free(expected_data);

    return 0;
}

#elif defined(TEST_WRITE)
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

#endif
