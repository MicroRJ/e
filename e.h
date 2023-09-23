/*
**
**                      -+- e -+-
**
**                      An Editor?
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**
**            [ TIMENTE MORTEM, OMNIA TIMEO ]
**
**
**                 github.com/MicroRJ/e
**
**
**                 USE AT YOUR OWN RISK
**
*/

#ifndef _E_H
#define _E_H

/* TODO: REMOVE */
# define _RX_STANDALONE
#include <rx/rx.c>

#ifndef E_REALLOC_ALIGNED
#define E_REALLOC_ALIGNED(length,memory) _aligned_realloc(memory,length,16)
# endif

#ifndef E_FREE_ALIGNED
#define E_FREE_ALIGNED(memory) _aligned_free(memory);
# endif

#ifndef E_MEMMOVE
#define E_MEMMOVE memmove
# endif

#ifndef E_MEMCOPY
#define E_MEMCOPY memcopy
# endif

/* TODO: */
#ifndef E_ASSERT
#define E_ASSERT ccassert
# endif

# ifndef E_PHASE
# define E_PHASE(E,time_since_last_blink,blinks_per_second) (.5 + .5 * cos(rxPI_F * time_since_last_blink / blinks_per_second));
#include <math.h>
#  endif

/* [[FONT]] */
#ifndef E_FONT
#define E_FONT_GET_LINE_HEIGHT(E)
#define E_FONT_GET_CODE_DIMENS(E,UTF32,LPWIDTH,LPHEIGHT)
#define E_FONT_GET_CODE_ADVANCE(E,UTF32A,UTF32B)
#define E_FONT_GET_NAME(E)
# endif

/* [[THEME]] */
#ifndef E_CURSOR_COLOR
#define E_CURSOR_COLOR(E) E_MK_COLOR_RGBA_UNORM(148,232,148,255)
# endif

#ifndef E_COLOR
#define E_COLOR rxcolor_t
# endif
#ifndef E_MK_COLOR_RGBA_UNORM
#define E_MK_COLOR_RGBA_UNORM RX_RGBA_UNORM
# endif

/* [[DRAWING PROCEDURES]] */
#ifndef E_DRAW
#define E_DRAW_AABB(color,roundness,x,y,w,h)
# endif

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4101)
#pragma warning(disable:4706) /* todo */
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#pragma warning(disable:4018)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#pragma warning(disable:4244)
#pragma warning(disable:4189)
#include <src/meow_hash_x64_aesni.h>

/* note: '/LIBPATH:<where>' tell the compiler where to find freetype */
#pragma comment(lib,"freetype")
#include <ft2build.h>
#include FT_FREETYPE_H

void Emu_imp_rect_sdf(rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include <src/boxthing.c>
#include <src/esystem.c>
#include  <src/earray.c>
#include <src/estring.c>
#include   <src/efont.h>
#include   <src/efont.c>
#include <src/ebuffer.h>
#include <src/ebuffer.c>

typedef enum {
	E_MOD_NONE,
	E_MOD_SHIFT_BIT = 1 << 0,
	E_MOD_CTRL_BIT = 1 << 1,
	E_MOD_ALT_BIT = 1 << 2,
	E_MOD_CUSTOM_0_BIT = 1 << 3,
	E_MOD_CUSTOM_1_BIT = 1 << 4,
	E_MOD_CUSTOM_2_BIT = 1 << 5,
} E_MOD;


typedef enum {
	E_kNONE = 0,
	E_kMOVE_LEFT,
	E_kMOVE_UP,
	E_kMOVE_RIGHT,
	E_kMOVE_DOWN,
	E_kMOVE_WORD_LEFT,
	E_kMOVE_WORD_RIGHT,
	E_kMOVE_LINE_LEFT,
	E_kMOVE_LINE_RIGHT,
	E_kMOVE_PAGE_START,
	E_kMOVE_PAGE_END,
	E_kDELETE_BACK,
	E_kDELETE_HERE,
	E_kCUT,
	E_kCOPY,
	E_kPASTE,
	E_kLINE,
	E_kCHAR,
} E_KEY_NAME;

typedef struct {
	/* mod: any applied modifiers
		key: the key
		num: (optional) the number of times to do it, (1) default
		chr: (optional) utf32 code
		cur: target cursor, (-1) for all */
	E_MOD mod;
	E_KEY_NAME key;
	int cur;
	int num,chr;

	void *lpUser;
} E_KEY;


#include <src/eeditor.h>
#include <src/eeditor.c>

void draw_single_text_line(rlFont *font, float x, float y, char const *string) {

	/* todo */
	int length = 0;
	while (*string != '\0' && *string != '\r' && *string != '\n') {
		length += 1;
		string += 1;
	}

	rlFont_Draw_Config config;
	ZeroMemory(&config,sizeof(config));
	config.font = font;
	config.x = x;
	config.y = y;
	config.char_height = font->char_height;
	config.line_height = font->line_height;
   config.tab_size = 2; /* in spaces */
	config.color = RX_RGBA_UNORM(8,36,36,255);
	config.color_table = NULL;
	config.length = length;
	config.string = string;
	config.colors = NULL;

	rlFont_drawText( &config );
}


#pragma warning(pop)

#endif