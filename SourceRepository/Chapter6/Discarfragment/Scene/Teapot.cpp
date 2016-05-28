#include "Teapot.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
//#include "Transform.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"CubeFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/CubeFragment.glsl"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
GLuint vbo_teapot_vertices, vbo_teapot_colors, ibo_teapot_elements,
  vbo_teapot_cp_vertices, vbo_teapot_cp_colors, ibo_teapot_cp_elements;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;
struct vertex { GLfloat x, y, z; };
struct vertex teapot_cp_vertices[] = {
  // 1
  {  1.4   ,   0.0   ,  2.4     },
  {  1.4   ,  -0.784 ,  2.4     },
  {  0.784 ,  -1.4   ,  2.4     },
  {  0.0   ,  -1.4   ,  2.4     },
  {  1.3375,   0.0   ,  2.53125 },
  {  1.3375,  -0.749 ,  2.53125 },
  {  0.749 ,  -1.3375,  2.53125 },
  {  0.0   ,  -1.3375,  2.53125 },
  {  1.4375,    0.0  ,  2.53125 },
  {  1.4375,  -0.805 ,  2.53125 },
  // 11
  {  0.805 ,  -1.4375,  2.53125 },
  {  0.0   ,  -1.4375,  2.53125 },
  {  1.5   ,   0.0   ,  2.4     },
  {  1.5   ,  -0.84  ,  2.4     },
  {  0.84  ,  -1.5   ,  2.4     },
  {  0.0   ,  -1.5   ,  2.4     },
  { -0.784 ,  -1.4   ,  2.4     },
  { -1.4   ,  -0.784 ,  2.4     },
  { -1.4   ,   0.0   ,  2.4     },
  { -0.749 ,  -1.3375,  2.53125 },
  // 21
  { -1.3375,  -0.749 ,  2.53125 },
  { -1.3375,   0.0   ,  2.53125 },
  { -0.805 ,  -1.4375,  2.53125 },
  { -1.4375,  -0.805 ,  2.53125 },
  { -1.4375,   0.0   ,  2.53125 },
  { -0.84  ,  -1.5   ,  2.4     },
  { -1.5   ,  -0.84  ,  2.4     },
  { -1.5   ,   0.0   ,  2.4     },
  { -1.4   ,   0.784 ,  2.4     },
  { -0.784 ,   1.4   ,  2.4     },
  // 31
  {  0.0   ,   1.4   ,  2.4     },
  { -1.3375,   0.749 ,  2.53125 },
  { -0.749 ,   1.3375,  2.53125 },
  {  0.0   ,   1.3375,  2.53125 },
  { -1.4375,   0.805 ,  2.53125 },
  { -0.805 ,   1.4375,  2.53125 },
  {  0.0   ,   1.4375,  2.53125 },
  { -1.5   ,   0.84  ,  2.4     },
  { -0.84  ,   1.5   ,  2.4     },
  {  0.0   ,   1.5   ,  2.4     },
  // 41
  {  0.784 ,   1.4   ,  2.4     },
  {  1.4   ,   0.784 ,  2.4     },
  {  0.749 ,   1.3375,  2.53125 },
  {  1.3375,   0.749 ,  2.53125 },
  {  0.805 ,   1.4375,  2.53125 },
  {  1.4375,   0.805 ,  2.53125 },
  {  0.84  ,   1.5   ,  2.4     },
  {  1.5   ,   0.84  ,  2.4     },
  {  1.75  ,   0.0   ,  1.875   },
  {  1.75  ,  -0.98  ,  1.875   },
  // 51
  {  0.98  ,  -1.75  ,  1.875   },
  {  0.0   ,  -1.75  ,  1.875   },
  {  2.0   ,   0.0   ,  1.35    },
  {  2.0   ,  -1.12  ,  1.35    },
  {  1.12  ,  -2.0   ,  1.35    },
  {  0.0   ,  -2.0   ,  1.35    },
  {  2.0   ,   0.0   ,  0.9     },
  {  2.0   ,  -1.12  ,  0.9     },
  {  1.12  ,  -2.0   ,  0.9     },
  {  0.0   ,  -2.0   ,  0.9     },
  // 61
  { -0.98  ,  -1.75  ,  1.875   },
  { -1.75  ,  -0.98  ,  1.875   },
  { -1.75  ,   0.0   ,  1.875   },
  { -1.12  ,  -2.0   ,  1.35    },
  { -2.0   ,  -1.12  ,  1.35    },
  { -2.0   ,   0.0   ,  1.35    },
  { -1.12  ,  -2.0   ,  0.9     },
  { -2.0   ,  -1.12  ,  0.9     },
  { -2.0   ,   0.0   ,  0.9     },
  { -1.75  ,   0.98  ,  1.875   },
  // 71
  { -0.98  ,   1.75  ,  1.875   },
  {  0.0   ,   1.75  ,  1.875   },
  { -2.0   ,   1.12  ,  1.35    },
  { -1.12  ,   2.0   ,  1.35    },
  {  0.0   ,   2.0   ,  1.35    },
  { -2.0   ,   1.12  ,  0.9     },
  { -1.12  ,   2.0   ,  0.9     },
  {  0.0   ,   2.0   ,  0.9     },
  {  0.98  ,   1.75  ,  1.875   },
  {  1.75  ,   0.98  ,  1.875   },
  // 81
  {  1.12  ,   2.0   ,  1.35    },
  {  2.0   ,   1.12  ,  1.35    },
  {  1.12  ,   2.0   ,  0.9     },
  {  2.0   ,   1.12  ,  0.9     },
  {  2.0   ,   0.0   ,  0.45    },
  {  2.0   ,  -1.12  ,  0.45    },
  {  1.12  ,  -2.0   ,  0.45    },
  {  0.0   ,  -2.0   ,  0.45    },
  {  1.5   ,   0.0   ,  0.225   },
  {  1.5   ,  -0.84  ,  0.225   },
  // 91
  {  0.84  ,  -1.5   ,  0.225   },
  {  0.0   ,  -1.5   ,  0.225   },
  {  1.5   ,   0.0   ,  0.15    },
  {  1.5   ,  -0.84  ,  0.15    },
  {  0.84  ,  -1.5   ,  0.15    },
  {  0.0   ,  -1.5   ,  0.15    },
  { -1.12  ,  -2.0   ,  0.45    },
  { -2.0   ,  -1.12  ,  0.45    },
  { -2.0   ,   0.0   ,  0.45    },
  { -0.84  ,  -1.5   ,  0.225   },
  // 101
  { -1.5   ,  -0.84  ,  0.225   },
  { -1.5   ,   0.0   ,  0.225   },
  { -0.84  ,  -1.5   ,  0.15    },
  { -1.5   ,  -0.84  ,  0.15    },
  { -1.5   ,   0.0   ,  0.15    },
  { -2.0   ,   1.12  ,  0.45    },
  { -1.12  ,   2.0   ,  0.45    },
  {  0.0   ,   2.0   ,  0.45    },
  { -1.5   ,   0.84  ,  0.225   },
  { -0.84  ,   1.5   ,  0.225   },
  // 111
  {  0.0   ,   1.5   ,  0.225   },
  { -1.5   ,   0.84  ,  0.15    },
  { -0.84  ,   1.5   ,  0.15    },
  {  0.0   ,   1.5   ,  0.15    },
  {  1.12  ,   2.0   ,  0.45    },
  {  2.0   ,   1.12  ,  0.45    },
  {  0.84  ,   1.5   ,  0.225   },
  {  1.5   ,   0.84  ,  0.225   },
  {  0.84  ,   1.5   ,  0.15    },
  {  1.5   ,   0.84  ,  0.15    },
  // 121
  { -1.6   ,   0.0   ,  2.025   },
  { -1.6   ,  -0.3   ,  2.025   },
  { -1.5   ,  -0.3   ,  2.25    },
  { -1.5   ,   0.0   ,  2.25    },
  { -2.3   ,   0.0   ,  2.025   },
  { -2.3   ,  -0.3   ,  2.025   },
  { -2.5   ,  -0.3   ,  2.25    },
  { -2.5   ,   0.0   ,  2.25    },
  { -2.7   ,   0.0   ,  2.025   },
  { -2.7   ,  -0.3   ,  2.025   },
  // 131
  { -3.0   ,  -0.3   ,  2.25    },
  { -3.0   ,   0.0   ,  2.25    },
  { -2.7   ,   0.0   ,  1.8     },
  { -2.7   ,  -0.3   ,  1.8     },
  { -3.0   ,  -0.3   ,  1.8     },
  { -3.0   ,   0.0   ,  1.8     },
  { -1.5   ,   0.3   ,  2.25    },
  { -1.6   ,   0.3   ,  2.025   },
  { -2.5   ,   0.3   ,  2.25    },
  { -2.3   ,   0.3   ,  2.025   },
  // 141
  { -3.0   ,   0.3   ,  2.25    },
  { -2.7   ,   0.3   ,  2.025   },
  { -3.0   ,   0.3   ,  1.8     },
  { -2.7   ,   0.3   ,  1.8     },
  { -2.7   ,   0.0   ,  1.575   },
  { -2.7   ,  -0.3   ,  1.575   },
  { -3.0   ,  -0.3   ,  1.35    },
  { -3.0   ,   0.0   ,  1.35    },
  { -2.5   ,   0.0   ,  1.125   },
  { -2.5   ,  -0.3   ,  1.125   },
  // 151
  { -2.65  ,  -0.3   ,  0.9375  },
  { -2.65  ,   0.0   ,  0.9375  },
  { -2.0   ,  -0.3   ,  0.9     },
  { -1.9   ,  -0.3   ,  0.6     },
  { -1.9   ,   0.0   ,  0.6     },
  { -3.0   ,   0.3   ,  1.35    },
  { -2.7   ,   0.3   ,  1.575   },
  { -2.65  ,   0.3   ,  0.9375  },
  { -2.5   ,   0.3   ,  1.1255  },
  { -1.9   ,   0.3   ,  0.6     },
  // 161
  { -2.0   ,   0.3   ,  0.9     },
  {  1.7   ,   0.0   ,  1.425   },
  {  1.7   ,  -0.66  ,  1.425   },
  {  1.7   ,  -0.66  ,  0.6     },
  {  1.7   ,   0.0   ,  0.6     },
  {  2.6   ,   0.0   ,  1.425   },
  {  2.6   ,  -0.66  ,  1.425   },
  {  3.1   ,  -0.66  ,  0.825   },
  {  3.1   ,   0.0   ,  0.825   },
  {  2.3   ,   0.0   ,  2.1     },
  // 171
  {  2.3   ,  -0.25  ,  2.1     },
  {  2.4   ,  -0.25  ,  2.025   },
  {  2.4   ,   0.0   ,  2.025   },
  {  2.7   ,   0.0   ,  2.4     },
  {  2.7   ,  -0.25  ,  2.4     },
  {  3.3   ,  -0.25  ,  2.4     },
  {  3.3   ,   0.0   ,  2.4     },
  {  1.7   ,   0.66  ,  0.6     },
  {  1.7   ,   0.66  ,  1.425   },
  {  3.1   ,   0.66  ,  0.825   },
  // 181
  {  2.6   ,   0.66  ,  1.425   },
  {  2.4   ,   0.25  ,  2.025   },
  {  2.3   ,   0.25  ,  2.1     },
  {  3.3   ,   0.25  ,  2.4     },
  {  2.7   ,   0.25  ,  2.4     },
  {  2.8   ,   0.0   ,  2.475   },
  {  2.8   ,  -0.25  ,  2.475   },
  {  3.525 ,  -0.25  ,  2.49375 },
  {  3.525 ,   0.0   ,  2.49375 },
  {  2.9   ,   0.0   ,  2.475   },
  // 191
  {  2.9   ,  -0.15  ,  2.475   },
  {  3.45  ,  -0.15  ,  2.5125  },
  {  3.45  ,   0.0   ,  2.5125  },
  {  2.8   ,   0.0   ,  2.4     },
  {  2.8   ,  -0.15  ,  2.4     },
  {  3.2   ,  -0.15  ,  2.4     },
  {  3.2   ,   0.0   ,  2.4     },
  {  3.525 ,   0.25  ,  2.49375 },
  {  2.8   ,   0.25  ,  2.475   },
  {  3.45  ,   0.15  ,  2.5125  },
  // 201
  {  2.9   ,   0.15  ,  2.475   },
  {  3.2   ,   0.15  ,  2.4     },
  {  2.8   ,   0.15  ,  2.4     },
  {  0.0   ,   0.0   ,  3.15    },
  {  0.0   ,  -0.002 ,  3.15    },
  {  0.002 ,   0.0   ,  3.15    },
  {  0.8   ,   0.0   ,  3.15    },
  {  0.8   ,  -0.45  ,  3.15    },
  {  0.45  ,  -0.8   ,  3.15    },
  {  0.0   ,  -0.8   ,  3.15    },
  // 211
  {  0.0   ,   0.0   ,  2.85    },
  {  0.2   ,   0.0   ,  2.7     },
  {  0.2   ,  -0.112 ,  2.7     },
  {  0.112 ,  -0.2   ,  2.7     },
  {  0.0   ,  -0.2   ,  2.7     },
  { -0.002 ,   0.0   ,  3.15    },
  { -0.45  ,  -0.8   ,  3.15    },
  { -0.8   ,  -0.45  ,  3.15    },
  { -0.8   ,   0.0   ,  3.15    },
  { -0.112 ,  -0.2   ,  2.7     },
  // 221
  { -0.2   ,  -0.112 ,  2.7     },
  { -0.2   ,   0.0   ,  2.7     },
  {  0.0   ,   0.002 ,  3.15    },
  { -0.8   ,   0.45  ,  3.15    },
  { -0.45  ,   0.8   ,  3.15    },
  {  0.0   ,   0.8   ,  3.15    },
  { -0.2   ,   0.112 ,  2.7     },
  { -0.112 ,   0.2   ,  2.7     },
  {  0.0   ,   0.2   ,  2.7     },
  {  0.45  ,   0.8   ,  3.15    },
  // 231
  {  0.8   ,   0.45  ,  3.15    },
  {  0.112 ,   0.2   ,  2.7     },
  {  0.2   ,   0.112 ,  2.7     },
  {  0.4   ,   0.0   ,  2.55    },
  {  0.4   ,  -0.224 ,  2.55    },
  {  0.224 ,  -0.4   ,  2.55    },
  {  0.0   ,  -0.4   ,  2.55    },
  {  1.3   ,   0.0   ,  2.55    },
  {  1.3   ,  -0.728 ,  2.55    },
  {  0.728 ,  -1.3   ,  2.55    },
  // 241
  {  0.0   ,  -1.3   ,  2.55    },
  {  1.3   ,   0.0   ,  2.4     },
  {  1.3   ,  -0.728 ,  2.4     },
  {  0.728 ,  -1.3   ,  2.4     },
  {  0.0   ,  -1.3   ,  2.4     },
  { -0.224 ,  -0.4   ,  2.55    },
  { -0.4   ,  -0.224 ,  2.55    },
  { -0.4   ,   0.0   ,  2.55    },
  { -0.728 ,  -1.3   ,  2.55    },
  { -1.3   ,  -0.728 ,  2.55    },
  // 251
  { -1.3   ,   0.0   ,  2.55    },
  { -0.728 ,  -1.3   ,  2.4     },
  { -1.3   ,  -0.728 ,  2.4     },
  { -1.3   ,   0.0   ,  2.4     },
  { -0.4   ,   0.224 ,  2.55    },
  { -0.224 ,   0.4   ,  2.55    },
  {  0.0   ,   0.4   ,  2.55    },
  { -1.3   ,   0.728 ,  2.55    },
  { -0.728 ,   1.3   ,  2.55    },
  {  0.0   ,   1.3   ,  2.55    },
  // 261
  { -1.3   ,   0.728 ,  2.4     },
  { -0.728 ,   1.3   ,  2.4     },
  {  0.0   ,   1.3   ,  2.4     },
  {  0.224 ,   0.4   ,  2.55    },
  {  0.4   ,   0.224 ,  2.55    },
  {  0.728 ,   1.3   ,  2.55    },
  {  1.3   ,   0.728 ,  2.55    },
  {  0.728 ,   1.3   ,  2.4     },
  {  1.3   ,   0.728 ,  2.4     },
};
#define TEAPOT_NB_PATCHES 28
#define ORDER 3
GLushort teapot_patches[][ORDER+1][ORDER+1] = {
  // rim
  { {   1,   2,   3,   4 }, {   5,   6,   7,   8 }, {   9,  10,  11,  12 }, {  13,  14,  15,  16, } },
  { {   4,  17,  18,  19 }, {   8,  20,  21,  22 }, {  12,  23,  24,  25 }, {  16,  26,  27,  28, } },
  { {  19,  29,  30,  31 }, {  22,  32,  33,  34 }, {  25,  35,  36,  37 }, {  28,  38,  39,  40, } },
  { {  31,  41,  42,   1 }, {  34,  43,  44,   5 }, {  37,  45,  46,   9 }, {  40,  47,  48,  13, } },
  // body
  { {  13,  14,  15,  16 }, {  49,  50,  51,  52 }, {  53,  54,  55,  56 }, {  57,  58,  59,  60, } },
  { {  16,  26,  27,  28 }, {  52,  61,  62,  63 }, {  56,  64,  65,  66 }, {  60,  67,  68,  69, } },
  { {  28,  38,  39,  40 }, {  63,  70,  71,  72 }, {  66,  73,  74,  75 }, {  69,  76,  77,  78, } },
  { {  40,  47,  48,  13 }, {  72,  79,  80,  49 }, {  75,  81,  82,  53 }, {  78,  83,  84,  57, } },
  { {  57,  58,  59,  60 }, {  85,  86,  87,  88 }, {  89,  90,  91,  92 }, {  93,  94,  95,  96, } },
  { {  60,  67,  68,  69 }, {  88,  97,  98,  99 }, {  92, 100, 101, 102 }, {  96, 103, 104, 105, } },
  { {  69,  76,  77,  78 }, {  99, 106, 107, 108 }, { 102, 109, 110, 111 }, { 105, 112, 113, 114, } },
  { {  78,  83,  84,  57 }, { 108, 115, 116,  85 }, { 111, 117, 118,  89 }, { 114, 119, 120,  93, } },
  // handle
  { { 121, 122, 123, 124 }, { 125, 126, 127, 128 }, { 129, 130, 131, 132 }, { 133, 134, 135, 136, } },
  { { 124, 137, 138, 121 }, { 128, 139, 140, 125 }, { 132, 141, 142, 129 }, { 136, 143, 144, 133, } },
  { { 133, 134, 135, 136 }, { 145, 146, 147, 148 }, { 149, 150, 151, 152 }, {  69, 153, 154, 155, } },
  { { 136, 143, 144, 133 }, { 148, 156, 157, 145 }, { 152, 158, 159, 149 }, { 155, 160, 161,  69, } },
  // spout
  { { 162, 163, 164, 165 }, { 166, 167, 168, 169 }, { 170, 171, 172, 173 }, { 174, 175, 176, 177, } },
  { { 165, 178, 179, 162 }, { 169, 180, 181, 166 }, { 173, 182, 183, 170 }, { 177, 184, 185, 174, } },
  { { 174, 175, 176, 177 }, { 186, 187, 188, 189 }, { 190, 191, 192, 193 }, { 194, 195, 196, 197, } },
  { { 177, 184, 185, 174 }, { 189, 198, 199, 186 }, { 193, 200, 201, 190 }, { 197, 202, 203, 194, } },
  // lid
  { { 204, 204, 204, 204 }, { 207, 208, 209, 210 }, { 211, 211, 211, 211 }, { 212, 213, 214, 215, } },
  { { 204, 204, 204, 204 }, { 210, 217, 218, 219 }, { 211, 211, 211, 211 }, { 215, 220, 221, 222, } },
  { { 204, 204, 204, 204 }, { 219, 224, 225, 226 }, { 211, 211, 211, 211 }, { 222, 227, 228, 229, } },
  { { 204, 204, 204, 204 }, { 226, 230, 231, 207 }, { 211, 211, 211, 211 }, { 229, 232, 233, 212, } },
  { { 212, 213, 214, 215 }, { 234, 235, 236, 237 }, { 238, 239, 240, 241 }, { 242, 243, 244, 245, } },
  { { 215, 220, 221, 222 }, { 237, 246, 247, 248 }, { 241, 249, 250, 251 }, { 245, 252, 253, 254, } },
  { { 222, 227, 228, 229 }, { 248, 255, 256, 257 }, { 251, 258, 259, 260 }, { 254, 261, 262, 263, } },
  { { 229, 232, 233, 212 }, { 257, 264, 265, 234 }, { 260, 266, 267, 238 }, { 263, 268, 269, 242, } },
  // no bottom!
};
#define RESU 10
#define RESV 10
struct vertex teapot_vertices[TEAPOT_NB_PATCHES * RESU*RESV];
GLfloat teapot_colors[TEAPOT_NB_PATCHES * RESU*RESV * 3];
GLushort teapot_elements[TEAPOT_NB_PATCHES * (RESU-1)*(RESV-1) * 2*3];

GLfloat teapot_cp_colors[269*3];
GLushort teapot_cp_elements[TEAPOT_NB_PATCHES][ORDER+1][ORDER+1];

void build_control_points_k(int p, struct vertex control_points_k[][ORDER+1]);
struct vertex compute_position(struct vertex control_points_k[][ORDER+1], float u, float v);
float bernstein_polynomial(int i, int n, float u);
float binomial_coefficient(int i, int n);
int factorial(int n);

void build_teapot() {
  // Vertices
  for (int p = 0; p < TEAPOT_NB_PATCHES; p++) {
    struct vertex control_points_k[ORDER+1][ORDER+1];
    build_control_points_k(p, control_points_k);
    for (int ru = 0; ru <= RESU-1; ru++) {
      float u = 1.0 * ru / (RESU-1);
      for (int rv = 0; rv <= RESV-1; rv++) {
	float v = 1.0 * rv / (RESV-1);
	teapot_vertices[p*RESU*RESV + ru*RESV + rv] = compute_position(control_points_k, u, v);
	teapot_colors[p*RESU*RESV*3 + ru*RESV*3 + rv*3 + 0] = 1.0 * p / TEAPOT_NB_PATCHES;
	teapot_colors[p*RESU*RESV*3 + ru*RESV*3 + rv*3 + 1] = 1.0 * p / TEAPOT_NB_PATCHES;
	teapot_colors[p*RESU*RESV*3 + ru*RESV*3 + rv*3 + 2] = 0.8;
      }
    }
  }

  // Elements
  int n = 0;
  for (int p = 0; p < TEAPOT_NB_PATCHES; p++)
    for (int ru = 0; ru < RESU-1; ru++)
      for (int rv = 0; rv < RESV-1; rv++) {
	// 1 square ABCD = 2 triangles ABC + CDA
	// ABC
	teapot_elements[n] = p*RESU*RESV +  ru   *RESV +  rv   ; n++;
	teapot_elements[n] = p*RESU*RESV +  ru   *RESV + (rv+1); n++;
	teapot_elements[n] = p*RESU*RESV + (ru+1)*RESV + (rv+1); n++;
	// CDA
	teapot_elements[n] = p*RESU*RESV + (ru+1)*RESV + (rv+1); n++;
	teapot_elements[n] = p*RESU*RESV + (ru+1)*RESV +  rv   ; n++;
	teapot_elements[n] = p*RESU*RESV +  ru   *RESV +  rv   ; n++;
      }

  // Control points elements for debugging
  memset(teapot_cp_colors, 0, sizeof(teapot_cp_colors)); // black
  for (int p = 0; p < TEAPOT_NB_PATCHES; p++)
    for (int i = 0; i < (ORDER+1); i++)
      for (int j = 0; j < (ORDER+1); j++)
	teapot_cp_elements[p][i][j] = teapot_patches[p][i][j] - 1;
}

void build_control_points_k(int p, struct vertex control_points_k[][ORDER+1]) {
  for (int i = 0; i <= ORDER; i++)
    for (int j = 0; j <= ORDER; j++)
      control_points_k[i][j] = teapot_cp_vertices[teapot_patches[p][i][j] - 1];
}

struct vertex compute_position(struct vertex control_points_k[][ORDER+1], float u, float v) {
  struct vertex result = { 0.0, 0.0, 0.0 };
  for (int i = 0; i <= ORDER; i++) {
    float poly_i = bernstein_polynomial(i, ORDER, u);
    for (int j = 0; j <= ORDER; j++) {
      float poly_j = bernstein_polynomial(j, ORDER, v);
      result.x += poly_i * poly_j * control_points_k[i][j].x;
      result.y += poly_i * poly_j * control_points_k[i][j].y;
      result.z += poly_i * poly_j * control_points_k[i][j].z;
    }
  }
  return result;
}

float bernstein_polynomial(int i, int n, float u) {
  return binomial_coefficient(i, n) * powf(u, i) * powf(1-u, n-i);
}

float binomial_coefficient(int i, int n) {
  assert(i >= 0); assert(n >= 0);
  return 1.0f * factorial(n) / (factorial(i) * factorial(n-i));
}
int factorial(int n) {
  assert(n >= 0);
  int result = 1;
  for (int i = n; i > 1; i--)
    result *= i;
  return result;
}

int init_resources()
{
  build_teapot();

  glGenBuffers(1, &vbo_teapot_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_vertices), teapot_vertices, GL_STATIC_DRAW);
  
  glGenBuffers(1, &vbo_teapot_colors);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_colors), teapot_colors, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo_teapot_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_teapot_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapot_elements), teapot_elements, GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_teapot_cp_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_cp_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_cp_vertices), teapot_cp_vertices, GL_STATIC_DRAW);
  
  glGenBuffers(1, &vbo_teapot_cp_colors);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_cp_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_cp_colors), teapot_cp_colors, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo_teapot_cp_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_teapot_cp_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapot_cp_elements), teapot_cp_elements, GL_STATIC_DRAW);

  //GLint link_ok = GL_FALSE;

  //GLuint vs, fs;
  //if ((vs = create_shader("teapot.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  //if ((fs = create_shader("teapot.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  //program = glCreateProgram();
  //glAttachShader(program, vs);
  //glAttachShader(program, fs);
  //glLinkProgram(program);
  //glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  //if (!link_ok) {
  //  fprintf(stderr, "glLinkProgram:");
  //  print_log(program);
  //  return 0;
  //}

  //const char* attribute_name;
  //attribute_name = "coord3d";
  //attribute_coord3d = glGetAttribLocation(program, attribute_name);
  //if (attribute_coord3d == -1) {
  //  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
  //  return 0;
  //}
  //attribute_name = "v_color";
  //attribute_v_color = glGetAttribLocation(program, attribute_name);
  //if (attribute_v_color == -1) {
  //  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
  //  return 0;
  //}
  //const char* uniform_name;
  //uniform_name = "mvp";
  //uniform_mvp = glGetUniformLocation(program, uniform_name);
  //if (uniform_mvp == -1) {
  //  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
  //  return 0;
  //}

  return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace used
using std::ifstream;
using std::ostringstream;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Teapot::Teapot( Renderer* parent )
{
	if (!parent)
		return;

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= TeapotType;

}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
Teapot::~Teapot()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"Cube" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Teapot::InitModel()
{
	program                             = NULL;
	ProgramManager* ProgramManagerObj   = MapRenderHandler->RendererProgramManager();
	Transform*	TransformObj            = MapRenderHandler->RendererTransform();


	if (! ( program = ProgramManagerObj->Program( ( char * )"Cube" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Cube" );
		ProgramManagerObj->AddProgram( program );
	}

	program->VertexShader		= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);

	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );

	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
	}
	m = mclose( m );

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
	}

	m = mclose( m );

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Teapot::Render()
{

}

void Teapot::RenderCubeVBO(){
}


void Teapot::TouchEventDown( float x, float y )
{
}

void Teapot::TouchEventMove( float x, float y )
{
}

void Teapot::TouchEventRelease( float x, float y )
{
}
