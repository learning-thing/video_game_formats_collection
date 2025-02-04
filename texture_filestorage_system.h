typedef enum 
{
	MFSS_UNKNOWN 	  = 0,
	MFSS_COMPRESS_ZSTD = 1,
	MFSS_COMPRESS_BC7  = 2
} TFSS_flags;


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
	
	uint32_t texture_count;
	texture_info* textures
	
} TFSS;
