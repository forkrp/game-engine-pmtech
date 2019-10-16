// renderer_definitions.h
// Copyright 2014 - 2019 Alex Dixon.
// License: https://github.com/polymonster/pmtech/blob/master/license.md

#ifndef _renderer_definitions_h
#define _renderer_definitions_h

#define PEN_RENDERER_OPENGL

#if __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define PEN_GLES3
#endif
#endif

#ifdef PEN_GLES3
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

// for portability with regular gl

// mark unsupported features null
#define GL_FILL 0x00               // gl fill is the only polygon mode on gles3
#define GL_LINE 0x00               // gl line (wireframe) usupported
#define GL_GEOMETRY_SHADER 0x00    // gl geometry shader unsupported
#define GL_TEXTURE_COMPRESSED 0x00 //
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x00
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x00
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x00

// remap unsupported stuff for rough equivalent
#define GL_CLAMP_TO_BORDER GL_CLAMP_TO_EDGE
#define GL_SRC1_COLOR GL_SRC_COLOR
#define GL_ONE_MINUS_SRC1_COLOR GL_ONE_MINUS_SRC_COLOR
#define GL_SRC1_ALPHA GL_SRC_ALPHA
#define GL_ONE_MINUS_SRC1_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define GL_TEXTURE_2D_MULTISAMPLE GL_TEXTURE_2D

#define glClearDepth glClearDepthf // gl es has these type suffixes

// gles does not support base vertex offset assert when b is > 0.. rethink how you are rendering stuff
#define glDrawElementsBaseVertex(p, i, f, o, b) glDrawElements(p, i, f, o)
#define glDrawElementsInstancedBaseVertex(p, i, f, o, c, b) glDrawElementsInstanced(p, i, f, o, c)
#define glDrawBuffer
#define glTexImage2DMultisample(a1, a2, a3, a4, a5, a6) PEN_ASSERT(0)

#else
#ifdef __linux__
#include "GL/glew.h"
#elif _WIN32
#define GLEW_BUILD
#include "GL/glew.h"
#include "GL/wglew.h"
#else // osx
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif
#endif

enum null_values
{
    PEN_NULL_DEPTH_BUFFER = -1,
    PEN_NULL_COLOUR_BUFFER = -1,
    PEN_NULL_PIXEL_SHADER = -1,
};

enum shader_type
{
    PEN_SHADER_TYPE_VS = GL_VERTEX_SHADER,
    PEN_SHADER_TYPE_PS = GL_FRAGMENT_SHADER,
    PEN_SHADER_TYPE_GS = GL_GEOMETRY_SHADER,
    PEN_SHADER_TYPE_SO,
    PEN_SHADER_TYPE_CS
};

enum fill_mode : s32
{
    PEN_FILL_SOLID = GL_FILL,
    PEN_FILL_WIREFRAME = GL_LINE
};

enum cull_mode : s32
{
    PEN_CULL_NONE = 0,
    PEN_CULL_FRONT = GL_FRONT,
    PEN_CULL_BACK = GL_BACK
};

enum default_targets : s32
{
    PEN_BACK_BUFFER_COLOUR = 0,
    PEN_BACK_BUFFER_DEPTH = 0
};

enum clear_bits : s32
{
    PEN_CLEAR_COLOUR_BUFFER = GL_COLOR_BUFFER_BIT,
    PEN_CLEAR_DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,
    PEN_CLEAR_STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT,
};

enum input_classification : s32
{
    PEN_INPUT_PER_VERTEX = 0,
    PEN_INPUT_PER_INSTANCE = 1
};

enum primitive_topology : s32
{
    PEN_PT_POINTLIST = GL_POINTS,
    PEN_PT_LINELIST = GL_LINES,
    PEN_PT_LINESTRIP = GL_LINE_STRIP,
    PEN_PT_TRIANGLELIST = GL_TRIANGLES,
    PEN_PT_TRIANGLESTRIP = GL_TRIANGLE_STRIP
};

#define PACK_GL_FORMAT(DATA_TYPE, NUM_ELEMENTS) ((NUM_ELEMENTS) | (DATA_TYPE << 4))
#define UNPACK_FORMAT(PF) PF >> 4
#define UNPACK_NUM_ELEMENTS(PF) PF&(0x07)

#define PACK_GL_TEXTURE_FORMAT(DATA_TYPE, NUM_ELEMENTS) ((NUM_ELEMENTS) | (DATA_TYPE << 4))
#define UNPACK_TEXTURE_FORMAT(PF) PF >> 4
#define UNPACK_DATA_TYPE(PF) PF&(0x07)

enum vertex_format : s32
{
    PEN_VERTEX_FORMAT_FLOAT1 = PACK_GL_FORMAT(GL_FLOAT, 1),
    PEN_VERTEX_FORMAT_FLOAT2 = PACK_GL_FORMAT(GL_FLOAT, 2),
    PEN_VERTEX_FORMAT_FLOAT3 = PACK_GL_FORMAT(GL_FLOAT, 3),
    PEN_VERTEX_FORMAT_FLOAT4 = PACK_GL_FORMAT(GL_FLOAT, 4),
    PEN_VERTEX_FORMAT_UNORM4 = PACK_GL_FORMAT(GL_UNSIGNED_BYTE, 4),
    PEN_VERTEX_FORMAT_UNORM2 = PACK_GL_FORMAT(GL_UNSIGNED_BYTE, 2),
    PEN_VERTEX_FORMAT_UNORM1 = PACK_GL_FORMAT(GL_UNSIGNED_BYTE, 1)
};

enum index_buffer_format : s32
{
    PEN_FORMAT_R16_UINT = GL_UNSIGNED_SHORT,
    PEN_FORMAT_R32_UINT = GL_UNSIGNED_INT
};

enum texture_format : s32
{
    // integer
    PEN_TEX_FORMAT_BGRA8_UNORM,
    PEN_TEX_FORMAT_RGBA8_UNORM,
    PEN_TEX_FORMAT_D24_UNORM_S8_UINT,

    // floating point
    PEN_TEX_FORMAT_R32G32B32A32_FLOAT,
    PEN_TEX_FORMAT_R32_FLOAT,
    PEN_TEX_FORMAT_R16G16B16A16_FLOAT,
    PEN_TEX_FORMAT_R16_FLOAT,
    PEN_TEX_FORMAT_R32_UINT,
    PEN_TEX_FORMAT_R8_UNORM,
    PEN_TEX_FORMAT_R32G32_FLOAT,

    // bc compressed
    PEN_TEX_FORMAT_BC1_UNORM,
    PEN_TEX_FORMAT_BC2_UNORM,
    PEN_TEX_FORMAT_BC3_UNORM,
    PEN_TEX_FORMAT_BC4_UNORM,
    PEN_TEX_FORMAT_BC5_UNORM
};

enum usage : s32
{
    PEN_USAGE_DEFAULT = GL_STATIC_DRAW,   // gpu read and write, d3d can updatesubresource with usage default
    PEN_USAGE_IMMUTABLE = GL_STATIC_DRAW, // gpu read only
    PEN_USAGE_DYNAMIC = GL_DYNAMIC_DRAW,  // dynamic
    PEN_USAGE_STAGING = GL_DYNAMIC_DRAW,  // cpu access
};

enum bind_flags : s32
{
    PEN_BIND_SHADER_RESOURCE = 0,

    PEN_BIND_VERTEX_BUFFER = GL_ARRAY_BUFFER,
    PEN_BIND_INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
    PEN_BIND_CONSTANT_BUFFER = GL_UNIFORM_BUFFER,
    PEN_STREAM_OUT_VERTEX_BUFFER = GL_ARRAY_BUFFER,
    PEN_BIND_RENDER_TARGET = GL_FRAMEBUFFER,
    PEN_BIND_DEPTH_STENCIL,
    PEN_BIND_SHADER_WRITE
};

enum cpu_access_flags : s32
{
    PEN_CPU_ACCESS_WRITE = GL_MAP_WRITE_BIT,
    PEN_CPU_ACCESS_READ = GL_MAP_READ_BIT
};

enum texture_address_mode : s32
{
    PEN_TEXTURE_ADDRESS_WRAP = GL_REPEAT,
    PEN_TEXTURE_ADDRESS_MIRROR = GL_MIRRORED_REPEAT,
    PEN_TEXTURE_ADDRESS_CLAMP = GL_CLAMP_TO_EDGE,
    PEN_TEXTURE_ADDRESS_BORDER = GL_CLAMP_TO_BORDER,
#if GL_EXT_texture_mirror_clamp
    PEN_TEXTURE_ADDRESS_MIRROR_ONCE = GL_MIRROR_CLAMP_EXT
#else
    PEN_TEXTURE_ADDRESS_MIRROR_ONCE = GL_MIRRORED_REPEAT
#endif
};

enum comparison : s32
{
    PEN_COMPARISON_NEVER = GL_NEVER,
    PEN_COMPARISON_LESS = GL_LESS,
    PEN_COMPARISON_EQUAL = GL_EQUAL,
    PEN_COMPARISON_LESS_EQUAL = GL_LEQUAL,
    PEN_COMPARISON_GREATER = GL_GREATER,
    PEN_COMPARISON_NOT_EQUAL = GL_NOTEQUAL,
    PEN_COMPARISON_GREATER_EQUAL = GL_GEQUAL,
    PEN_COMPARISON_ALWAYS = GL_ALWAYS
};

enum filter_mode : s32
{
    PEN_FILTER_MIN_MAG_MIP_LINEAR = 0,
    PEN_FILTER_MIN_MAG_MIP_POINT = 1,
    PEN_FILTER_LINEAR = 2,
    PEN_FILTER_POINT = 3
};

enum blending_factor : s32
{
    PEN_BLEND_ZERO = GL_ZERO,
    PEN_BLEND_ONE = GL_ONE,
    PEN_BLEND_SRC_COLOR = GL_SRC_COLOR,
    PEN_BLEND_INV_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    PEN_BLEND_SRC_ALPHA = GL_SRC_ALPHA,
    PEN_BLEND_INV_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    PEN_BLEND_DEST_ALPHA = GL_DST_ALPHA,
    PEN_BLEND_INV_DEST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
    PEN_BLEND_DEST_COLOR = GL_DST_COLOR,
    PEN_BLEND_INV_DEST_COLOR = GL_ONE_MINUS_DST_COLOR,
    PEN_BLEND_SRC_ALPHA_SAT = GL_SRC_ALPHA_SATURATE,
    PEN_BLEND_BLEND_FACTOR = GL_CONSTANT_COLOR,
    PEN_BLEND_INV_BLEND_FACTOR = GL_ONE_MINUS_CONSTANT_COLOR,
    PEN_BLEND_SRC1_COLOR = GL_SRC1_COLOR,
    PEN_BLEND_INV_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
    PEN_BLEND_SRC1_ALPHA = GL_SRC1_ALPHA,
    PEN_BLEND_INV_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA
};

enum blend_op : s32
{
    PEN_BLEND_OP_ADD = GL_FUNC_ADD,
    PEN_BLEND_OP_SUBTRACT = GL_FUNC_SUBTRACT,
    PEN_BLEND_OP_REV_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
    PEN_BLEND_OP_MIN = GL_MIN,
    PEN_BLEND_OP_MAX = GL_MAX
};

enum stencil_op : s32
{
    PEN_STENCIL_OP_KEEP = GL_KEEP,
    PEN_STENCIL_OP_REPLACE = GL_REPLACE,
    PEN_STENCIL_OP_ZERO = GL_ZERO,
    PEN_STENCIL_OP_INCR_SAT = GL_INCR,
    PEN_STENCIL_OP_DECR_SAT = GL_DECR,
    PEN_STENCIL_OP_INVERT = GL_INVERT,
    PEN_STENCIL_OP_INCR = GL_INCR_WRAP,
    PEN_STENCIL_OP_DECR = GL_DECR_WRAP
};

enum misc_flags : s32
{
    PEN_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
    PEN_RESOURCE_MISC_SHARED = 0x2L,
    PEN_RESOURCE_MISC_TEXTURECUBE = 0x4L,
    PEN_RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
    PEN_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20,
    PEN_RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
    PEN_RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
    PEN_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
    PEN_RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
    PEN_RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
    PEN_RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
    PEN_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
    PEN_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
    PEN_RESOURCE_MISC_GUARDED = 0x8000L,
    PEN_RESOURCE_MISC_TILE_POOL = 0x20000L,
    PEN_RESOURCE_MISC_TILED = 0x40000L
};

/*

enum format : s32
{
    PEN_FORMAT_UNKNOWN = DXGI_FORMAT_UNKNOWN,
    PEN_FORMAT_R32G32B32A32_TYPELESS = DXGI_FORMAT_R32G32B32A32_TYPELESS,
    PEN_FORMAT_R32G32B32A32_FLOAT = DXGI_FORMAT_R32G32B32A32_FLOAT,
    PEN_FORMAT_R32G32B32A32_UINT = DXGI_FORMAT_R32G32B32A32_UINT,
    PEN_FORMAT_R32G32B32A32_SINT = DXGI_FORMAT_R32G32B32A32_SINT,
    PEN_FORMAT_R32G32B32_TYPELESS = DXGI_FORMAT_R32G32B32_TYPELESS,
    PEN_FORMAT_R32G32B32_FLOAT = DXGI_FORMAT_R32G32B32_FLOAT,
    PEN_FORMAT_R32G32B32_UINT = DXGI_FORMAT_R32G32B32_UINT,
    PEN_FORMAT_R32G32B32_SINT = DXGI_FORMAT_R32G32B32_SINT,
    PEN_FORMAT_R16G16B16A16_TYPELESS = DXGI_FORMAT_R16G16B16A16_TYPELESS,
    PEN_FORMAT_R16G16B16A16_FLOAT = DXGI_FORMAT_R16G16B16A16_FLOAT,
    PEN_FORMAT_R16G16B16A16_UNORM = DXGI_FORMAT_R16G16B16A16_UNORM,
    PEN_FORMAT_R16G16B16A16_UINT = DXGI_FORMAT_R16G16B16A16_UINT,
    PEN_FORMAT_R16G16B16A16_SNORM = DXGI_FORMAT_R16G16B16A16_SNORM,
    PEN_FORMAT_R16G16B16A16_SINT = DXGI_FORMAT_R16G16B16A16_SINT,
    PEN_FORMAT_R32G32_TYPELESS = DXGI_FORMAT_R32G32_TYPELESS,
    PEN_FORMAT_R32G32_FLOAT = DXGI_FORMAT_R32G32_FLOAT,
    PEN_FORMAT_R32G32_UINT = DXGI_FORMAT_R32G32_UINT,
    PEN_FORMAT_R32G32_SINT = DXGI_FORMAT_R32G32_SINT,
    PEN_FORMAT_R32G8X24_TYPELESS = DXGI_FORMAT_R32G8X24_TYPELESS,
    PEN_FORMAT_D32_FLOAT_S8X24_UINT = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
    PEN_FORMAT_R32_FLOAT_X8X24_TYPELESS = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
    PEN_FORMAT_X32_TYPELESS_G8X24_UINT = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
    PEN_FORMAT_R10G10B10A2_TYPELESS = DXGI_FORMAT_R10G10B10A2_TYPELESS,
    PEN_FORMAT_R10G10B10A2_UNORM = DXGI_FORMAT_R10G10B10A2_UNORM,
    PEN_FORMAT_R10G10B10A2_UINT = DXGI_FORMAT_R10G10B10A2_UINT,
    PEN_FORMAT_R11G11B10_FLOAT = DXGI_FORMAT_R11G11B10_FLOAT,
    PEN_FORMAT_R8G8B8A8_TYPELESS = DXGI_FORMAT_R8G8B8A8_TYPELESS,
    PEN_FORMAT_R8G8B8A8_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
    PEN_FORMAT_R8G8B8A8_UNORM_SRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
    PEN_FORMAT_R8G8B8A8_UINT = DXGI_FORMAT_R8G8B8A8_UINT,
    PEN_FORMAT_R8G8B8A8_SNORM = DXGI_FORMAT_R8G8B8A8_SNORM,
    PEN_FORMAT_R8G8B8A8_SINT = DXGI_FORMAT_R8G8B8A8_SINT,
    PEN_FORMAT_R16G16_TYPELESS = DXGI_FORMAT_R16G16_TYPELESS,
    PEN_FORMAT_R16G16_FLOAT = DXGI_FORMAT_R16G16_FLOAT,
    PEN_FORMAT_R16G16_UNORM = DXGI_FORMAT_R16G16_UNORM,
    PEN_FORMAT_R16G16_UINT = DXGI_FORMAT_R16G16_UINT,
    PEN_FORMAT_R16G16_SNORM = DXGI_FORMAT_R16G16_SNORM,
    PEN_FORMAT_R16G16_SINT = DXGI_FORMAT_R16G16_SINT,
    PEN_FORMAT_R32_TYPELESS = DXGI_FORMAT_R32_TYPELESS,
    PEN_FORMAT_D32_FLOAT = DXGI_FORMAT_D32_FLOAT,
    PEN_FORMAT_R32_FLOAT = DXGI_FORMAT_R32_FLOAT,
    PEN_FORMAT_R32_UINT = DXGI_FORMAT_R32_UINT,
    PEN_FORMAT_R32_SINT = DXGI_FORMAT_R32_SINT,
    PEN_FORMAT_R24G8_TYPELESS = DXGI_FORMAT_R24G8_TYPELESS,
    PEN_FORMAT_D24_UNORM_S8_UINT = DXGI_FORMAT_D24_UNORM_S8_UINT,
    PEN_FORMAT_R24_UNORM_X8_TYPELESS = DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
    PEN_FORMAT_X24_TYPELESS_G8_UINT = DXGI_FORMAT_X24_TYPELESS_G8_UINT,
    PEN_FORMAT_R8G8_TYPELESS = DXGI_FORMAT_R8G8_TYPELESS,
    PEN_FORMAT_R8G8_UNORM = DXGI_FORMAT_R8G8_UNORM,
    PEN_FORMAT_R8G8_UINT = DXGI_FORMAT_R8G8_UINT,
    PEN_FORMAT_R8G8_SNORM = DXGI_FORMAT_R8G8_SNORM,
    PEN_FORMAT_R8G8_SINT = DXGI_FORMAT_R8G8_SINT,
    PEN_FORMAT_R16_TYPELESS = DXGI_FORMAT_R16_TYPELESS,
    PEN_FORMAT_R16_FLOAT = DXGI_FORMAT_R16_FLOAT,
    PEN_FORMAT_D16_UNORM = DXGI_FORMAT_D16_UNORM,
    PEN_FORMAT_R16_UNORM = DXGI_FORMAT_R16_UNORM,
    PEN_FORMAT_R16_UINT = DXGI_FORMAT_R16_UINT,
    PEN_FORMAT_R16_SNORM = DXGI_FORMAT_R16_SNORM,
    PEN_FORMAT_R16_SINT = DXGI_FORMAT_R16_SINT,
    PEN_FORMAT_R8_TYPELESS = DXGI_FORMAT_R8_TYPELESS,
    PEN_FORMAT_R8_UNORM = DXGI_FORMAT_R8_UNORM,
    PEN_FORMAT_R8_UINT = DXGI_FORMAT_R8_UINT,
    PEN_FORMAT_R8_SNORM = DXGI_FORMAT_R8_SNORM,
    PEN_FORMAT_R8_SINT = DXGI_FORMAT_R8_SINT,
    PEN_FORMAT_A8_UNORM = DXGI_FORMAT_A8_UNORM,
    PEN_FORMAT_R1_UNORM = DXGI_FORMAT_R1_UNORM,
    PEN_FORMAT_R9G9B9E5_SHAREDEXP = DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
    PEN_FORMAT_R8G8_B8G8_UNORM = DXGI_FORMAT_R8G8_B8G8_UNORM,
    PEN_FORMAT_G8R8_G8B8_UNORM = DXGI_FORMAT_G8R8_G8B8_UNORM,
    PEN_FORMAT_BC1_TYPELESS = DXGI_FORMAT_BC1_TYPELESS,
    PEN_FORMAT_BC1_UNORM = DXGI_FORMAT_BC1_UNORM,
    PEN_FORMAT_BC1_UNORM_SRGB = DXGI_FORMAT_BC1_UNORM_SRGB,
    PEN_FORMAT_BC2_TYPELESS = DXGI_FORMAT_BC2_TYPELESS,
    PEN_FORMAT_BC2_UNORM = DXGI_FORMAT_BC2_UNORM,
    PEN_FORMAT_BC2_UNORM_SRGB = DXGI_FORMAT_BC2_UNORM_SRGB,
    PEN_FORMAT_BC3_TYPELESS = DXGI_FORMAT_BC3_TYPELESS,
    PEN_FORMAT_BC3_UNORM = DXGI_FORMAT_BC3_UNORM,
    PEN_FORMAT_BC3_UNORM_SRGB = DXGI_FORMAT_BC3_UNORM_SRGB,
    PEN_FORMAT_BC4_TYPELESS = DXGI_FORMAT_BC4_TYPELESS,
    PEN_FORMAT_BC4_UNORM = DXGI_FORMAT_BC4_UNORM,
    PEN_FORMAT_BC4_SNORM = DXGI_FORMAT_BC4_SNORM,
    PEN_FORMAT_BC5_TYPELESS = DXGI_FORMAT_BC5_TYPELESS,
    PEN_FORMAT_BC5_UNORM = DXGI_FORMAT_BC5_UNORM,
    PEN_FORMAT_BC5_SNORM = DXGI_FORMAT_BC5_SNORM,
    PEN_FORMAT_B5G6R5_UNORM = DXGI_FORMAT_B5G6R5_UNORM,
    PEN_FORMAT_B5G5R5A1_UNORM = DXGI_FORMAT_B5G5R5A1_UNORM,
    PEN_FORMAT_B8G8R8A8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM,
    PEN_FORMAT_B8G8R8X8_UNORM = DXGI_FORMAT_B8G8R8X8_UNORM,
    PEN_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
    PEN_FORMAT_B8G8R8A8_TYPELESS = DXGI_FORMAT_B8G8R8A8_TYPELESS,
    PEN_FORMAT_B8G8R8A8_UNORM_SRGB = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
    PEN_FORMAT_B8G8R8X8_TYPELESS = DXGI_FORMAT_B8G8R8X8_TYPELESS,
    PEN_FORMAT_B8G8R8X8_UNORM_SRGB = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
    PEN_FORMAT_BC6H_TYPELESS = DXGI_FORMAT_BC6H_TYPELESS,
    PEN_FORMAT_BC6H_UF16 = DXGI_FORMAT_BC6H_UF16,
    PEN_FORMAT_BC6H_SF16 = DXGI_FORMAT_BC6H_SF16,
    PEN_FORMAT_BC7_TYPELESS = DXGI_FORMAT_BC7_TYPELESS,
    PEN_FORMAT_BC7_UNORM = DXGI_FORMAT_BC7_UNORM,
    PEN_FORMAT_BC7_UNORM_SRGB = DXGI_FORMAT_BC7_UNORM_SRGB,
    PEN_FORMAT_AYUV = DXGI_FORMAT_AYUV,
    PEN_FORMAT_Y410 = DXGI_FORMAT_Y410,
    PEN_FORMAT_Y416 = DXGI_FORMAT_Y416,
    PEN_FORMAT_NV12 = DXGI_FORMAT_NV12,
    PEN_FORMAT_P010 = DXGI_FORMAT_P010,
    PEN_FORMAT_P016 = DXGI_FORMAT_P016,
    PEN_FORMAT_420_OPAQUE = DXGI_FORMAT_420_OPAQUE,
    PEN_FORMAT_YUY2 = DXGI_FORMAT_YUY2,
    PEN_FORMAT_Y210 = DXGI_FORMAT_Y210,
    PEN_FORMAT_Y216 = DXGI_FORMAT_Y216,
    PEN_FORMAT_NV11 = DXGI_FORMAT_NV11,
    PEN_FORMAT_AI44 = DXGI_FORMAT_AI44,
    PEN_FORMAT_IA44 = DXGI_FORMAT_IA44,
    PEN_FORMAT_P8 = DXGI_FORMAT_P8,
    PEN_FORMAT_A8P8 = DXGI_FORMAT_A8P8,
    PEN_FORMAT_B4G4R4A4_UNORM = DXGI_FORMAT_B4G4R4A4_UNORM,
    PEN_FORMAT_FORCE_UINT = DXGI_FORMAT_FORCE_UINT
};

enum sampler_filters : s32
{
    PEN_FILTER_MIN_MAG_MIP_POINT = D3D11_FILTER_MIN_MAG_MIP_POINT,
    PEN_FILTER_MIN_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MIN_POINT_MAG_MIP_LINEAR = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
    PEN_FILTER_MIN_LINEAR_MAG_MIP_POINT = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
    PEN_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MIN_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MIN_MAG_MIP_LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
    PEN_FILTER_ANISOTROPIC = D3D11_FILTER_ANISOTROPIC,
    PEN_FILTER_COMPARISON_MIN_MAG_MIP_POINT = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
    PEN_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
    PEN_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
    PEN_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
    PEN_FILTER_COMPARISON_ANISOTROPIC = D3D11_FILTER_COMPARISON_ANISOTROPIC,
    PEN_FILTER_MINIMUM_MIN_MAG_MIP_POINT = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
    PEN_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
    PEN_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
    PEN_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,
    PEN_FILTER_MINIMUM_ANISOTROPIC = D3D11_FILTER_MINIMUM_ANISOTROPIC,
    PEN_FILTER_MAXIMUM_MIN_MAG_MIP_POINT = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
    PEN_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
    PEN_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
    PEN_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    PEN_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
    PEN_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
    PEN_FILTER_MAXIMUM_ANISOTROPIC = D3D11_FILTER_MAXIMUM_ANISOTROPIC
};

enum query_marker : s32
{
    PEN_QUERY_BEGIN = 1,
    PEN_QUERY_END = 2
};

enum query_type : s32
{
    PEN_QUERY_EVENT = 0,
    PEN_QUERY_OCCLUSION = ( PEN_QUERY_EVENT + 1 ),
    PEN_QUERY_TIMESTAMP = ( PEN_QUERY_OCCLUSION + 1 ),
    PEN_QUERY_TIMESTAMP_DISJOINT = ( PEN_QUERY_TIMESTAMP + 1 ),
    PEN_QUERY_PIPELINE_STATISTICS = ( PEN_QUERY_TIMESTAMP_DISJOINT + 1 ),
    PEN_QUERY_OCCLUSION_PREDICATE = ( PEN_QUERY_PIPELINE_STATISTICS + 1 ),
    PEN_QUERY_SO_STATISTICS = ( PEN_QUERY_OCCLUSION_PREDICATE + 1 ),
    PEN_QUERY_SO_OVERFLOW_PREDICATE = ( PEN_QUERY_SO_STATISTICS + 1 ),
    PEN_QUERY_SO_STATISTICS_STREAM0 = ( PEN_QUERY_SO_OVERFLOW_PREDICATE + 1 ),
    PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 = ( PEN_QUERY_SO_STATISTICS_STREAM0 + 1 ),
    PEN_QUERY_SO_STATISTICS_STREAM1 = ( PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 + 1 ),
    PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM1 = ( PEN_QUERY_SO_STATISTICS_STREAM1 + 1 ),
    PEN_QUERY_SO_STATISTICS_STREAM2 = ( PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM1 + 1 ),
    PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM2 = ( PEN_QUERY_SO_STATISTICS_STREAM2 + 1 ),
    PEN_QUERY_SO_STATISTICS_STREAM3 = ( PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM2 + 1 ),
    PEN_QUERY_SO_OVERFLOW_PREDICATE_STREAM3 = ( PEN_QUERY_SO_STATISTICS_STREAM3 + 1 )
};


*/

#endif
