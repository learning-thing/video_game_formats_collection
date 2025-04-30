#ifndef TFSS_H
#define TFSS_H

#include <stdint.h>

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

    GPU_TEXTURE_UNCOMPRESSED_I_RGB8888,     // rgb with alpha
    GPU_TEXTURE_UNCOMPRESSED_I_RGB888,      // rgb with alpha
    GPU_TEXTURE_UNCOMPRESSED_I_RGB16161616, // integer hdr
    GPU_TEXTURE_UNCOMPRESSED_I_GS8,         // 8 bit grayscale 
    GPU_TEXTURE_UNCOMPRESSED_I_GSA88,        // 8 bit grayscale with alpha
    GPU_TEXTURE_UNCOMPRESSED_I_GSA81,        // 8 bit grayscale with one bit alpha 
    GPU_TEXTURE_UNCOMPRESSED_I_ILU8,         // lumanance texture no alpha
    GPU_TEXTURE_UNCOMPRESSED_I_ILUA88,       // lumanance texture with 8 bit alpha
    GPU_TEXTURE_UNCOMPRESSED_I_ILUA81,       // lumanance texture with 1 bit alpha

    // note if uncompressed, always assume 16 bit float
    GPU_TEXTURE_UNCOMPRESSED_F_RGB,         // floating point hdr
    GPU_TEXTURE_UNCOMRPESSED_F_GS,          // floating point grayscale no alpha
    GPU_TEXTURE_UNCOMRPESSED_F_GSA,         // floating point grayscale wiht alpha   
    GPU_TEXTURE_UNCOMRPESSED_F_ILU,         // floating point grayscale wiht alpha   
    GPU_TEXTURE_UNCOMRPESSED_F_ILUA,         // floating point grayscale wiht alpha   
    
    
    GPU_TEXTURE_COMPRESSED_MAX // Sentinel value
} texture_formats;


typedef enum {
    TEXTURE_FLAG_IS_PIXELART       = 1 << 0,  
    TEXTURE_FLAG_TRILINEAR         = 1 << 1,  
    TEXTURE_FLAG_CLAMP_S           = 1 << 2,  
    TEXTURE_FLAG_CLAMP_T           = 1 << 3,  
    TEXTURE_FLAG_ANISOTROPIC       = 1 << 4,  
    TEXTURE_FLAG_IS_COMPRESSED     = 1 << 5,  // checking for if using bcx compression 
    TEXTURE_FLAG_IS_NORMAL_MAP     = 1 << 6,  
    TEXTURE_FLAG_NO_MIPMAP         = 1 << 7,  // disables mipmapping
    TEXTURE_FLAG_PREGEN_MIPMAP     = 1 << 8,  // pregenerated mip maps are stored
    TEXTURE_FLAG_NO_LOD            = 1 << 9,  
    TEXTURE_FLAG_SRGB              = 1 << 10, 
    TEXTURE_FLAG_PRESRGB           = 1 << 11, 
    TEXTURE_FLAG_IS_ZIMG_PRECOMP   = 1 << 12, 
    TEXTURE_FLAG_IS_TEXTURE_ARRAY  = 1 << 13,

    // Depth/Stencil/Framebuffer modes (occupy bits 14-15)
    TEXTURE_FLAG_DEPTHMODE_NONE          = 0 << 14,
    TEXTURE_FLAG_DEPTHMODE_DEPTH         = 1 << 14,
    TEXTURE_FLAG_DEPTHMODE_STENCIL       = 2 << 14,
    TEXTURE_FLAG_DEPTHMODE_DEPTH_STENCIL = 3 << 14,

    TEXTURE_FLAG_DEPTHMODE_MASK          = 3 << 14, // to extract the mode
} texture_flags;


typedef struct 
{
    char name[32];             // Texture name (optional)
	
    uint32_t width;            // Texture width
    uint32_t height;           // Texture height
    uint32_t depth;            // 3D texture depth (optional, 0 if 2D)
	
    uint32_t flags;
    
    uint8_t mip_count;         // Number of mipmap levels
    uint8_t image_format;       // Texture format (e.g., RGBA8, BC7)
    uint16_t image_count;

    uint32_t size;             // Total size of the texture data in bytes in order to access the next image, after decompression, the 
} texture_info;

// material file storage system
typedef struct
{
	char magic[4]; 	// MTSS
	
	uint32_t texture_count;
	texture_info* textures;
	
} TFSS;

#endif
