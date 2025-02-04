#ifndef TFSS_H
#define TFSS_H

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

    // BPTC (Vulkan, OpenGL)
    GPU_TEXTURE_COMPRESSED_BPTC_RGB,   // BC7
    GPU_TEXTURE_COMPRESSED_BPTC_FLOAT, // BC6H

    // RGTC (Vulkan, OpenGL)
    GPU_TEXTURE_COMPRESSED_RGTC1,  // BC4
    GPU_TEXTURE_COMPRESSED_RGTC2,  // BC5

    // ASTC (Vulkan, OpenGL, mobile)
    GPU_TEXTURE_COMPRESSED_ASTC_4x4,
    GPU_TEXTURE_COMPRESSED_ASTC_5x5,
    GPU_TEXTURE_COMPRESSED_ASTC_6x6,
    GPU_TEXTURE_COMPRESSED_ASTC_8x8,
    GPU_TEXTURE_COMPRESSED_ASTC_10x10,
    GPU_TEXTURE_COMPRESSED_ASTC_12x12,

    // ETC (Embedded GPUs, OpenGL ES, Vulkan)
    GPU_TEXTURE_COMPRESSED_ETC1_RGB8,
    GPU_TEXTURE_COMPRESSED_ETC2_RGB8,
    GPU_TEXTURE_COMPRESSED_ETC2_RGBA8,
    GPU_TEXTURE_COMPRESSED_ETC2_RGB8A1,

    // PVRTC (PowerVR, Apple devices)
    GPU_TEXTURE_COMPRESSED_PVRTC1_2BPP,
    GPU_TEXTURE_COMPRESSED_PVRTC1_4BPP,
    GPU_TEXTURE_COMPRESSED_PVRTC2_2BPP,
    GPU_TEXTURE_COMPRESSED_PVRTC2_4BPP,

    // ATC (Adreno GPUs)
    GPU_TEXTURE_COMPRESSED_ATC_RGB,
    GPU_TEXTURE_COMPRESSED_ATC_RGBA_EXPLICIT,
    GPU_TEXTURE_COMPRESSED_ATC_RGBA_INTERPOLATED,

    // FXT1 (3dfx legacy)
    GPU_TEXTURE_COMPRESSED_FXT1_RGB,

    // EAC (OpenGL ES, Vulkan)
    GPU_TEXTURE_COMPRESSED_EAC_R11,
    GPU_TEXTURE_COMPRESSED_EAC_RG11,

    GPU_TEXTURE_COMPRESSED_MAX // Sentinel value
} texture_compression_formats;


typedef struct 
{
    char name[32];             // Texture name (optional)
	
    uint32_t width;            // Texture width
    uint32_t height;           // Texture height
    uint32_t depth;            // 3D texture depth (optional, 0 if 2D)
	
    uint8_t mip_count;         // Number of mipmap levels
    uint16_t array_size;       // Number of layers (for texture arrays), if set to zero then it is only a single texture
    uint8_t format;            // Texture format (e.g., RGBA8, BC7)
	
    uint32_t size;             // Total size of the texture data in bytes
} texture_info;

// material file storage system
typedef struct
{
	char magic[4]; 	// MTSS
	
	uint8_t flags;
	uint8_t lossy_compresion_type;
	
	uint32_t texture_count;
	texture_info* textures
	
} TFSS;

#endif
