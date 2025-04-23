#ifndef TFSS_H
#define TFSS_H

#include <stdint.h>

typedef enum 
{
	MFSS_UNKNOWN 	  = 0,
	MFSS_COMPRESS_ZSTD = 1,
	MFSS_COMPRESS_WITH_LOSSY  = 2
} TFSS_flags;

typedef enum {
    // S3TC / DXT (DirectX, OpenGL, Vulkan)
    GPU_TEXTURE_COMPRESSED_DXT1,   // BC1
    GPU_TEXTURE_COMPRESSED_DXT3,   // BC2
    GPU_TEXTURE_COMPRESSED_DXT5,   // BC3

    // RGTC (Vulkan, OpenGL)
    GPU_TEXTURE_COMPRESSED_RGTC1,  // BC4
    GPU_TEXTURE_COMPRESSED_RGTC2,  // BC5

    // BPTC (Vulkan, OpenGL)
    GPU_TEXTURE_COMPRESSED_BPTC_RGB,   // BC7
    GPU_TEXTURE_COMPRESSED_BPTC_FLOAT, // BC6H

    GPU_TEXTURE_COMPRESSED_MAX // Sentinel value
} texture_compression_formats;

typedef enum
{
    COLOR_R = 1 << 3,
    COLOR_G = 1 << 2,
    COLOR_B = 1 << 1,
    COLOR_A = 1 << 0
} texture_color_properties;

typedef enum
{
    COLOR_FORMAT_INT,
    COLOR_FORMAT_UINT,
    COLOR_FORMAT_FLOAT,
    COLOR_FORMAT_FLOAT16,
} texture_color_format;


typedef struct 
{
    char name[32];             // Texture name (optional)
	
    uint32_t width;            // Texture width
    uint32_t height;           // Texture height
    uint32_t depth;            // 3D texture depth (optional, 0 if 2D)
	
    uint8_t mip_count;         // Number of mipmap levels
    uint8_t array_size;        // Number of layers (for texture arrays), if set to zero then it is only a single texture
    uint8_t image_format;       // Texture format (e.g., RGBA8, BC7)
    uint8_t color_format;      //  color format so number of colors

    uint32_t size;             // Total size of the texture data in bytes
} texture_info;

// material file storage system
typedef struct
{
	char magic[4]; 	// MTSS
	
	uint8_t flags;
	
	uint32_t texture_count;
	texture_info* textures;
	
} TFSS;

#endif
