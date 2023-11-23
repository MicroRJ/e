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

/* note: '/LIBPATH:<where>' tell the compiler where to find freetype */
#pragma comment(lib,"freetype")
#include <ft2build.h>
#include FT_FREETYPE_H

#define TUI_LINE_HEIGHT ((lui.font->line_height*1.2f))

#define ELUI_COLOR_BACKGROUND   lgi_RGBA_U(11,53,62,255)
#define ELUI_COLOR_FOREGROUND   lgi_RGBA_U(9,63,73,255)
#define ELUI_COLOR_PRIMARY      lgi_RGBA_U(111,139,149,255)
#define ELUI_COLOR_SECONDARY 	  lgi_RGBA_U(111,139,149,255)
#define ELUI_COLOR_DANGER 		  lgi_RGBA_U(180+45,60+45,63+45,255)
#define ELUI_COLOR_HAPPY 		  lgi_RGBA_U(79,191,138,255)
#define ELUI_COLOR_ACCENT 		  lgi_RGBA_U(134,151,38,255)

#define ELUI_DEFAULT_ROUNDINESS 2.5


#ifndef _MSC_VER
typedef signed long long int __int64;
typedef signed int 			  __int32;
#endif

/* TODO: */
#ifndef lui__debugassert
#define lui__debugassert lgi_ensure
# endif
#ifndef lui__reallocaligned
#define lui__reallocaligned(length,memory) _aligned_realloc(memory,length,16)
# endif
#ifndef lui__freealigned
#define lui__freealigned(memory) _aligned_free(memory);
# endif
#ifndef lui_memmove
#define lui_memmove memmove
# endif
#ifndef lui_memcopy
#define lui_memcopy memcopy
# endif

// TODO: REMOVE BEGIN
#ifndef E_IS_ALPNUM
#define E_IS_ALPNUM(r) (isWithin(r,'a','z')||isWithin(r,'A','Z')||isWithin(r,'0','9')||(r)== '_')
# endif
#ifndef E_IS_BLANK
#define E_IS_BLANK(chr) ((chr ==  ' ')||(chr == '\t')||(chr == '\r')||(chr == '\n'))
# endif
#ifndef E_IS_WORD_DELI
#define E_IS_WORD_DELI(r) (!E_IS_ALPNUM(r))
# endif
# ifndef E_CURSOR_PHASE
# define E_CURSOR_PHASE(E,time_since_last_blink,blinks_per_second) (.5 + .5 * cos(lgi_PI * time_since_last_blink / blinks_per_second));
#include <math.h>
#  endif
#ifndef E_CURSOR_COLOR
#define E_CURSOR_COLOR(E) E_MK_COLOR_RGBA_UNORM(148,232,148,255)
# endif
#ifndef E_COLOR
#define E_COLOR lgi_Color
# endif
#ifndef E_MK_COLOR_RGBA_UNORM
#define E_MK_COLOR_RGBA_UNORM lgi_RGBA_U
# endif
// TODO: REMOVE END

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
} lui_EditorEvent;

typedef struct lui_GlyphCol lui_GlyphCol;

typedef struct {
	lui_GlyphCol *pallet;
	int external_index;
	int utf32;
	short x0,y0,x1,y1;
	short offset_x,offset_y;
	float walking_x;
} lui_FontGlyph;

// TODO:
// this is the shittiest packing algorithm ever
typedef struct lui_GlyphRow {
	lui_GlyphCol *pallet;
	short height,cursor_x,cursor_y;
} lui_GlyphRow;

typedef struct lui_GlyphCol {
	lgi_Texture *texture;
	lgi_Bitmap storage;
	unsigned dirty: 1;
	lui_GlyphRow **buckets;
	short cursor_y;
} lui_GlyphCol;

typedef struct lui_Font {
	char const *fpath;

	lui_FontGlyph **glyph_table;

	float char_height;
	float line_height;

	float spaceWidth;

	float ascent;
	float descent;
	float lineGap;

	int is_sdf;
	int is_subpixel;

	/* for underline effect, offset is relative to the baseline,
		height is whatever the designer thought looked nice for the
		underline's thickness */
	float underline_baseline_offset;
	float underline_thickness;

	struct {
		FT_Face face;
	} freetype;

	// struct {
	// 	stbtt_fontinfo face;
	// } stb;
} lui_Font;

typedef struct {
	/* [[TODO]]: only the offset is needed */
	int offset,length;
} lui_TextLine;

// [[[TODO REMOVE]]]
/* is either the font knows about the renderer or the renderer knows about the font
	or this becomes a separate file #pending */
typedef struct lui_Draw_Config {
	union {
		lui_Font *font;
		lui_Font *lpFont;
	};
	float             x,y;

	int tab_size; /* in spaces */

	lgi_Color color;

	lgi_Color *color_table;

	int              length;
	char const     * string;
	unsigned char  * colors;

	/* should you choose to use .array to draw several lines,
		.string, .colors and .length must be large enough to accomodate all
		lines, set colors or null to use .color instead */
	lui_TextLine *line_array;

	int line_count;

	float line_height;

	/* optional, not meant for subpixel fonts */
	float char_height;
} lui_Draw_Config;



/* [[ REMOVE ]] */
typedef struct
{
	struct
	{
		int offset_x, offset_y;
		int stride, height;
		unsigned char *cursor;
	} dst;

	struct
	{
		int offset_x, offset_y;
		int stride, height;
		unsigned char const *cursor;
	} src;

} Emu_memcpy2d_config_t;

enum { lui_left, lui_right, lui_top, lui_bottom };

typedef struct lui_Box {
	float x0,y0,x1,y1;
} lui_Box;

typedef struct {
	void *key;
	elBool toggled;
} lui_State;

typedef struct {
	char name[MAX_PATH];
	char fileName[MAX_PATH];
   /* [[todo]]: this could be replaced with single ints, the length of the line
   	isn't necessary, at least not explicitly, it can be calculated by using the
   	offset of the next line instead  */
	lui_TextLine      *lcache;
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
} lui_Buffer;

typedef struct {
	__int32 xchar,yline;
} lui_Cursor;

typedef struct {
	E_MOD mod;
	lui_Cursor *cursor;
	lui_Buffer buffer;
	lui_Font *font;
	float cursor_blink_speed_in_seconds;
	float cursor_blink_timer;
	__int32 lyview;
	__int32 isReadonly;
} lui_Editor;

struct {
	lui_Font *font;
	float textHeight;
	lgi_Color textColor;
	lgi_Color *colorTable;
	unsigned char *charColors;
	lui_Box boxstack[0x20];
	lui_Box *box;
	int boxcount;
	lui_Font **fonts;
	lui_GlyphCol **pallets;
} lgi_Global lui;

void lui_buffer__setName(lui_Buffer *, char const *);
void lui_buffer__setFileName(lui_Buffer *, char const *);
/* [[todo]]: alloc and insert could be replaced with a single really nifty function */
char *EBuffer_allocSize(lui_Buffer *, __int64 reserve, __int64 commit);
char *EBuffer_insertSize(lui_Buffer *, __int64 offset, __int64 length);
void lui_buffer__initSized(lui_Buffer *, char const *, __int64 length);
lui_TextLine Emu_buffer_get_line_at_index(lui_Buffer *buffer, int index);
int ebuffer_get_line_length(lui_Buffer *buffer, int yline);
int ebuffer_get_line_offset(lui_Buffer *buffer, int yline);

/* [[API]] */
char *egetptr(lui_Editor *, __int32);
__int32 enumcur(lui_Editor *);
__int32 lui_editor__addCursor(lui_Editor *, lui_Cursor cur);
void esetcur(lui_Editor *, __int32, lui_Cursor cur);
lui_Cursor egetcur(lui_Editor *, __int32);
__int32 egetcurx(lui_Editor *, __int32);
__int32 egetcury(lui_Editor *, __int32);
__int32 ecurloc(lui_Editor *, __int32);
char ecurchr(lui_Editor *, __int32, __int32 off);
char *ecurptr(lui_Editor *, __int32);

__int32 eputchar(lui_Editor *, __int32 index, __int32 chr);
void edelchar(lui_Editor *, __int32 index);
void eaddline(lui_Editor *, __int32 offset, __int32 number);
void eremline(lui_Editor *, __int32 offset, __int32 number);

void lui_editor__testKeys(lui_Editor *);
void Editor_keyOne(lui_Editor *, lui_EditorEvent key);
void Editor_keyAll(lui_Editor *, lui_EditorEvent key);


/* [[TODO]] */
#define lui_logError(fmt,...)


lui_Font *lui_loadFont(char const *fileName, float height);
lui_FontGlyph *lui__findOrMakeGlyphByUnicode(lui_Font *lpFont, int utf32);

void
lgi_drawText( lui_Draw_Config *config );



lui_Font *lui_loadFont(char const *fileName, float height) {
	FT_Library library_ft;
	if (FT_Init_FreeType(&library_ft)) {
		lui_logError("failed to init FreeType");
	}
	FT_Face face_ft;
	if (FT_New_Face(library_ft,fileName,0,&face_ft)) {
		lui_logError("failed to load file path");
	}
	if (FT_Set_Pixel_Sizes(face_ft,0,height)) {
		lui_logError("invalid pixel size");
	}

	/* 16.16 to 24.6 to ..32.. */
	float units_to_pixels = face_ft->size->metrics.y_scale / 65536. / 64.;
	float lineGap = face_ft->height - (face_ft->ascender - face_ft->descender);

	lui_Font *font = lgi__allocate_typeof(lui_Font);

	font->is_subpixel = TRUE;
	font->is_sdf = FALSE;
	font->fpath = fileName;
	font->glyph_table = NULL;
	font->ascent = face_ft->ascender * units_to_pixels;
	font->descent = face_ft->descender * units_to_pixels;
	font->lineGap = lineGap * units_to_pixels;
	font->line_height = height;
	font->char_height = height;
	font->freetype.face = face_ft;

	lui_FontGlyph *underscoreGlyph = lui__findOrMakeGlyphByUnicode(font,'_');
	lui_FontGlyph *spaceGlyph = lui__findOrMakeGlyphByUnicode(font,' ');
	if (spaceGlyph == NULL) {
		spaceGlyph = underscoreGlyph;
	}
	font->spaceWidth = spaceGlyph->walking_x;
	return font;
}


float
lui_getTextWidth(lui_Font *font, char const *lpStr, int length) {
	float result = 0.;
	float spaceW = font->spaceWidth;
	for (int i=0; i<length; i+=1) {
		int chr = lpStr[i];
		if E_IS_BLANK(chr) {
			if (chr == '\t') {
				result += spaceW * 3;
			} else {
				result += spaceW;
			}
		} else {
			lui_FontGlyph *lpGlyph = lui__findOrMakeGlyphByUnicode(font,chr);
			result += lpGlyph->walking_x;
		}
	}
	return result;
}

#include   <src/efont.c>
#include "lui.c"
#include <src/ebuffer.c>


void
Editor_keyAll(lui_Editor *lp, lui_EditorEvent key) {
	for (key.cur=enumcur(lp)-1; key.cur>=0; key.cur-=1) {
		Editor_keyOne(lp,key);
	}
}

void
Editor_handleKey(lui_Editor *lp, E_KID kid, int num, int chr) {

	// printf("handle_key kid := %s num := %i chr := %i\n"
	// , E_KID_toStringMap[kid],num,chr);

	lui_EditorEvent key;
	key.mod = lp->mod;
	key.kid = kid;
	key.num = num;
	key.chr = chr;
	key.cur =  -1;
	Editor_keyAll(lp,key);
}

int
enumcur(lui_Editor *editor) {
	return arrlen(editor->cursor);
}

#include <src/eeditor.c>


void
lui__drawBox(lui_Box rect, lgi_Color color) {
	lgi_drawQuad(color,rect.x0,rect.y0,(rect.x1-rect.x0),(rect.y1-rect.y0));
}

void
lui_draw_round_box(lui_Box box, lgi_Color color, float cornerRadius) {
	Emu_imp_rect_sdf(
	/* */rxvec2_xy(
	/* */box.x0 + (box.x1 - box.x0) * .5f,
	/* */box.y0 + (box.y1 - box.y0) * .5f),
	/* */rxvec2_xy(
	/* */.5f*(box.x1-box.x0),
	/* */.5f*(box.y1-box.y0)), color, cornerRadius, 1.);
}

void
lui__drawText(lui_Box box, char const *string) {

	int length = 0;
	while isNeitherOf3(string[length],'\0','\r','\n') {
		length += 1;
	}

	lui_Draw_Config config;
	ZeroMemory(&config,sizeof(config));
	config.font = lui.font;
	config.x = box.x0;
	config.y = box.y1 - lui.font->line_height * 1.2f;
	config.char_height = lui.font->char_height;
	config.line_height = lui.font->line_height;
	config.tab_size = 2; /* in spaces */
	config.color = lui.textColor;
	config.color_table = lui.colorTable;
	config.length = length;
	config.string = string;
	config.colors = lui.charColors;

	lgi_drawText( &config );
}

#pragma warning(pop)

#endif