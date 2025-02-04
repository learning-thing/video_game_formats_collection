typedef enum 
{
    // Vertex attribute formats (e.g., position, normal, etc.)
    FORMAT_UNKNOWN = 0,           // Unknown or uninitialized format
    
    FORMAT_FLOAT_1,               // 1-component float (e.g., scalar values)
    FORMAT_FLOAT_2,               // 2-component float (e.g., vec2)
    FORMAT_FLOAT_3,               // 3-component float (e.g., vec3)
    FORMAT_FLOAT_4,               // 4-component float (e.g., vec4)
    
    FORMAT_INT_1,                 // 1-component int (e.g., integer values)
    FORMAT_INT_2,                 // 2-component int
    FORMAT_INT_3,                 // 3-component int
    FORMAT_INT_4,                 // 4-component int
    
    FORMAT_UINT_1,                // 1-component unsigned int
    FORMAT_UINT_2,                // 2-component unsigned int
    FORMAT_UINT_3,                // 3-component unsigned int
    FORMAT_UINT_4,                // 4-component unsigned int
    
    FORMAT_BYTE_4,                // 4-component byte (e.g., color in RGBA)
    FORMAT_BYTE_3,                // 3-component byte (e.g., color in RGB)
    FORMAT_BYTE_2,                // 2-component byte (e.g., color in grayscale)
    FORMAT_BYTE_1,                // 1-component byte
    
    FORMAT_HALF_2,                // 2-component half float (16-bit float)
    FORMAT_HALF_3,                // 3-component half float
    FORMAT_HALF_4,                // 4-component half float
    
    FORMAT_DOUBLE_1,              // 1-component double precision float
    FORMAT_DOUBLE_2,              // 2-component double precision float
    FORMAT_DOUBLE_3,              // 3-component double precision float
    FORMAT_DOUBLE_4,              // 4-component double precision float
    
    FORMAT_UINT8_NORM,            // 1-component unsigned int with normalized value [0, 1] (e.g., color channels)
    FORMAT_INT8_NORM,             // 1-component signed int with normalized value [-1, 1]
    FORMAT_UINT16_NORM,           // 1-component unsigned short with normalized value [0, 1]
    FORMAT_INT16_NORM,            // 1-component signed short with normalized value [-1, 1]
    
    FORMAT_MAX                    // Maximum value, not used for actual data
} VFSS_format;

typedef struct 
{
	char name[32];         // Optional buffer name
	uint32_t vertex_size;  // Size of each vertex in bytes

	uint32_t index_size;   // Total size of index buffer
} VFSS_buffer_info;


//vertex filestream system
typedef struct 
{
	char magic[4];  				// "VFSS"
	uint8_t flags;  				// Flags for compression, endianess, etc.

	uint8_t vertex_attr_count; 		// Number of attributes (max 16)
	struct {
		uint8_t attr_type;   		// Position, Normal, TexCoord, etc.
		uint8_t format;      		// Float3, Byte4, etc.
		uint8_t offset;      		// Offset in each vertex
		uint8_t padding;     		// Alignment padding
	} attributes[16];

	uint8_t index_type; 			// 0 = uint16, 1 = uint32

	char mesh_section[4];  			// "VFBS"
	uint32_t mesh_count;   			// Number of meshes
	vertex_buffer_info buffers[];  	// Variable array of mesh buffers
} VFSS;
