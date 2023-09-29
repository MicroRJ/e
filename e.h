/*
**
** Copyright(c) 2023 - Dayan Rodriguez - Dyr
**
** -+- e -+-
**
*/

// [ TIMENTE MORTEM, OMNIA TIMEO ]

// See section [[API]]

#ifndef _E_H
#define _E_H

#ifndef _MSC_VER
typedef signed long long int __int64;
typedef signed int 			  __int32;
#endif

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
#define E_ASSERT rx_assert
# endif

#ifndef E_IS_ALPNUM
#define E_IS_ALPNUM(r) (isWithin(r,'a','z')||isWithin(r,'A','Z')||isWithin(r,'0','9')||(r)== '_')
# endif

#ifndef E_IS_BLANK
#define E_IS_BLANK(chr) ((chr ==  ' ')||(chr == '\t')||(chr == '\r')||(chr == '\n'))
# endif

#ifndef E_IS_WORD_DELI
#define E_IS_WORD_DELI(r) (!E_IS_ALPNUM(r))
# endif

/* [[FONT]] */
#ifndef E_FONT
#define E_FONT_GET_LINE_HEIGHT(E)
#define E_FONT_GET_CODE_DIMENS(E,UTF32,LPWIDTH,LPHEIGHT)
#define E_FONT_GET_CODE_ADVANCE(E,UTF32A,UTF32B)
#define E_FONT_GET_NAME(E)
# endif

/* [[THEME]] */
# ifndef E_CURSOR_PHASE
# define E_CURSOR_PHASE(E,time_since_last_blink,blinks_per_second) (.5 + .5 * cos(rxPI_F * time_since_last_blink / blinks_per_second));
#include <math.h>
#  endif

#ifndef E_CURSOR_COLOR
#define E_CURSOR_COLOR(E) E_MK_COLOR_RGBA_UNORM(148,232,148,255)
# endif

/* [[DRAWING INTERFACE]] */

#ifndef E_COLOR
#define E_COLOR rlColor
# endif
#ifndef E_MK_COLOR_RGBA_UNORM
#define E_MK_COLOR_RGBA_UNORM rxColor_RGBA_U
# endif

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
} E_KID;

char const *E_KID_toStringMap[] = {
	"NONE","MOVE_LEFT","MOVE_UP","MOVE_RIGHT","MOVE_DOWN","MOVE_WORD_LEFT","MOVE_WORD_RIGHT","MOVE_LINE_LEFT","MOVE_LINE_RIGHT","MOVE_PAGE_START","MOVE_PAGE_END","DELETE_BACK","DELETE_HERE","CUT","COPY","PASTE","LINE","CHAR",
};

typedef struct {
	/* .mod: any applied modifiers
		.kid: the key id
		.cur: target cursor, (-1) for all
		.num: (optional) the number of times to do it, (1) default
		.xpt: (optional) x screen space coordinates (for things like click events)
		.ypt: (optional) y screen space coordinates (for things like click events)
		.chr: (optional) utf32 code */
	E_MOD mod;
	E_KID kid;
	int cur;
	int num,chr;
	float xpt,ypt;
	void *lpUser;
} E_KEY;

/* [[TODO]]: REMOVE */
#include  <src/earray.c>
#include   <src/efont.h>
#include   <src/efont.c>

typedef rlFont_Line emarker_t;

typedef struct {
	char name[MAX_PATH];
	char fileName[MAX_PATH];
   /* [[todo]]: this could be replaced with single ints, the length of the line
   	isn't necessary, at least not explicitly, it can be calculated by using the
   	offset of the next line instead  */
	emarker_t      *lcache;
	union  {
		void        *memory;
		char        *string;
	};
	meow_u128     checksum;

	/* [[REMOVE]]: we may want to open a file multiple encodings and or formattings */
	unsigned char  *colors;
	__int32 modified;
	__int64 extent;
	__int64 length;
	__int32 isReadonly;
} EBuffer;

void EBuffer_setName(EBuffer *, char const *);
void EBuffer_setFileName(EBuffer *, char const *);
/* [[todo]]: alloc and insert could be replaced with a single really nifty function */
char *EBuffer_allocSize(EBuffer *, __int64 reserve, __int64 commit);
char *EBuffer_insertSize(EBuffer *, __int64 offset, __int64 length);

typedef struct {
	__int32 xchar,yline;
} ECursor;

typedef struct {
	E_MOD mod;
	ECursor *cursor;
	EBuffer buffer;
	rlFont_Face *font;
	float cursor_blink_speed_in_seconds;
	float cursor_blink_timer;
	__int32 lyview;
	__int32 isReadonly;
} EEditor;

/* [[API]] */
char *egetptr(EEditor *, __int32);
__int32 enumcur(EEditor *);
__int32 eaddcur(EEditor *, ECursor cur);
void esetcur(EEditor *, __int32, ECursor cur);
ECursor egetcur(EEditor *, __int32);
__int32 egetcurx(EEditor *, __int32);
__int32 egetcury(EEditor *, __int32);
__int32 ecurloc(EEditor *, __int32);
char ecurchr(EEditor *, __int32, __int32 off);
char *ecurptr(EEditor *, __int32);

__int32 eputchar(EEditor *, __int32 index, __int32 chr);
void edelchar(EEditor *, __int32 index);
void eaddline(EEditor *, __int32 offset, __int32 number);
void eremline(EEditor *, __int32 offset, __int32 number);

void Editor_testKeys(EEditor *);
void Editor_keyOne(EEditor *, E_KEY key);
void Editor_keyAll(EEditor *, E_KEY key);

#include <src/boxthing.c>
#include <src/ebuffer.h>
#include <src/ebuffer.c>
#include <src/eeditor.h>
#include <src/eeditor.c>

void draw_single_text_line(rlFont_Face *font, float x, float y, char const *string) {

	/* todo */
	int length = 0;
	while (*string != '\0' && *string != '\r' && *string != '\n') {
		length += 1;
		string += 1;
	}

	rxTTF_DRAW config;
	ZeroMemory(&config,sizeof(config));
	config.font = font;
	config.x = x;
	config.y = y;
	config.char_height = font->char_height;
	config.line_height = font->line_height;
   config.tab_size = 2; /* in spaces */
	config.color = rxColor_RGBA_U(8,36,36,255);
	config.color_table = NULL;
	config.length = length;
	config.string = string;
	config.colors = NULL;

	rx_drawText( &config );
}


#pragma warning(pop)

#endif