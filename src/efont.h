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

/* note: '/LIBPATH:<where>' tell the compiler where to find freetype */
#pragma comment(lib,"freetype")
#include <ft2build.h>
#include FT_FREETYPE_H

/* [[TODO]] */
#define rlFONT_LOG_ERROR(fmt,...)

typedef struct rlFont_Pallet rlFont_Pallet;

/* fonts api only supports ascii characters at the moment and horizontal
	layouts */
typedef struct {
	rlFont_Pallet *pallet;
	int external_index;
	int utf32;
	short x0;
	short y0;
	short x1;
	short y1;
	short offset_x;
	short offset_y;
	float walking_x;
} rxFont_Glyph;

/* [[NOTE]]: glyphs are laid out horizontally within buckets */
typedef struct {
	rlFont_Pallet *pallet;
	short height;
	short cursor_x;
	short cursor_y;
} rlFont_Bucket;

/* [[NOTE]]: buckets are laid out vertically within pallets */
typedef struct rlFont_Pallet {

	/* [[TODO]]: there's probably a better way to do this */
	rxGPU_Texture *texture;
	rx_Image 	  storage;

	unsigned dirty: 1;

  	/* [[TODO]]: sort buckets? */
	rlFont_Bucket **buckets;
	short cursor_y;
} rlFont_Pallet;

/* everything is expressed in a y-upwards coordinate system */
typedef struct {
	char const *fpath;

	rxFont_Glyph **glyph_table;

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
} rlFont_Face;

rlFont_Face *rx_loadFontFromFile(char const *fileName, float height);
rxFont_Glyph *rlFont_findOrMakeGlyphByUnicode(rlFont_Face *lpFont, int utf32);

struct {

	rlFont_Face   **fonts;
	rlFont_Pallet **pallets;

} rxGlobal emu_Font_Library;

typedef struct {
	/* [[TODO]]: only the offset is needed */
	__int32 offset;
	__int32 length;
} rlFont_Line;

/* is either the font knows about the renderer or the renderer knows about the font
	or this becomes a separate file #pending */
typedef struct {
	union {
		rlFont_Face *font;
		rlFont_Face *lpFont;
	};
	float             x,y;

	int tab_size; /* in spaces */

	rlColor color;

	rlColor *color_table;

	int              length;
	char const     * string;
	unsigned char  * colors;

	/* should you choose to use .array to draw several lines,
		.string, .colors and .length must be large enough to accomodate all
		lines, set colors or null to use .color instead */
	rlFont_Line *line_array;

	int line_count;

	float line_height;

	/* optional, not meant for subpixel fonts */
	float char_height;
} rxTTF_DRAW;

void
rx_drawText( rxTTF_DRAW *config );



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

/* #todo */
rxAPI inline void
rlMem_copy2d( Emu_memcpy2d_config_t *config )
{
	for (int y = 0; y < config->src.height; y += 1)
	{
		memcpy(
		config->dst.cursor + config->dst.stride * (y + config->dst.offset_y) + config->dst.offset_x,
		config->src.cursor + config->src.stride * (y + config->src.offset_y) + config->src.offset_x,

		config->src.stride );
	}
}


/* [[IMPLEMENTATION]] */

rlFont_Face *
rx_loadFontFromFile(char const *fileName, float height) {

	FT_Library library_ft;
	if (FT_Init_FreeType(&library_ft)) {
		rlFONT_LOG_ERROR("failed to init FreeType");
	}

	FT_Face face_ft;
	if (FT_New_Face(library_ft,fileName,0,&face_ft)) {
		rlFONT_LOG_ERROR("failed to load file path");
	}

	if (FT_Set_Pixel_Sizes(face_ft,0,height)) {
		rlFONT_LOG_ERROR("invalid pixel size");
	}

	/* 16.16 to 24.6 to ..32.. */
	float units_to_pixels = face_ft->size->metrics.y_scale / 65536. / 64.;
	float lineGap = face_ft->height - (face_ft->ascender - face_ft->descender);

	rlFont_Face *font = EMU_ALLOC_TYPE(rlFont_Face);

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

	rxFont_Glyph *underscoreGlyph = rlFont_findOrMakeGlyphByUnicode(font,'_');
	rxFont_Glyph *spaceGlyph = rlFont_findOrMakeGlyphByUnicode(font,' ');
	if (spaceGlyph == NULL) {
		spaceGlyph = underscoreGlyph;
	}
	font->spaceWidth = spaceGlyph->walking_x;
	return font;
}


float
rlFont_getWidth(rlFont_Face *lpFont, char const *lpStr, int length) {
	float result = 0.;
	float spaceW = lpFont->spaceWidth;
	for (int i=0; i<length; i+=1) {
		int chr = lpStr[i];
		if E_IS_BLANK(chr) {
			if (chr == '\t') {
				result += spaceW * 3;
			} else {
				result += spaceW;
			}
		} else {
			rxFont_Glyph *lpGlyph = rlFont_findOrMakeGlyphByUnicode(lpFont,chr);
			result += lpGlyph->walking_x;
		}
	}
	return result;
}
