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

typedef struct rlGlyphPallet rlGlyphPallet;

/* fonts api only supports ascii characters at the moment and horizontal
	layouts */
typedef struct {
	rlGlyphPallet *pallet;
	int external_index;
	int utf32;
	short x0;
	short y0;
	short x1;
	short y1;
	short offset_x;
	short offset_y;
	float walking_x;
} rlFontGlyph;

// typedef struct
// {
// 	rlFontGlyph *glyph;

// 	rxvec2_t p0, p1, p2, p3;
// 	float u0, v0, u1, v1;
// } Emu_font_quad_t;

typedef struct
{
	rlGlyphPallet *pallet;

	short height;
  /* buckets allocate bitmap space horizontally */
	short cursor_x;

  /* each bucket needs to remember where they are in the pallet */
	short cursor_y;
} rlGlyphBucket;

typedef struct rlGlyphPallet
{
	rlTexture       *texture;
	rlImage storage;

	unsigned dirty: 1;

  /* buckets are sorted in ascending order, if you find a suitable
  	bucket go for it, it is the optimal size, maybe... if the bucket
  	is still too big you have to create a new one since all the other
  	ones are bigger */
	rlGlyphBucket **buckets;

  /* pallets allocate bitmap space vertically */
	short cursor_y;
} rlGlyphPallet;


/* #todo */
typedef struct
{ struct
	{ char const *file;
		void       *memory;
		int         length;
	} ttf;
	int         char_start;
	int         char_end;
	char const *char_string;

	int edge_value;
	int pixels_per_unit;
	int padding;
	int supports_subpixel;
	int supports_sdf;
} Emu_glyph_font_config_t;

/* everything is expressed in a y-upwards coordinate system */
typedef struct
{
	char const *fpath;

	rlFontGlyph **glyph_table;

	float char_height;
	float line_height;

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

	struct
	{
		FT_Face face;
	} freetype;

	struct
	{
		stbtt_fontinfo face;
	} stb;
} rlFont;

rlFont *
rlFont_loadFromFile(
char const *fpath, float char_height);

struct {

	rlFont **fonts;
	rlGlyphPallet **pallets;

} ccglobal emu_Font_Library;

typedef struct
{
	int offset;
	int length;

} Emu_text_line_t;

/* is either the font knows about the renderer or the renderer knows about the font
	or this becomes a separate file #pending */
typedef struct
{
	rlFont *font;
	float             x,y;

	int tab_size; /* in spaces */

	rxcolor_t color;

	rxcolor_t *color_table;

	int              length;
	char const     * string;
	unsigned char  * colors;

	/* should you choose to use .array to draw several lines,
		.string, .colors and .length must be large enough to accomodate all
		lines, set colors to null to use .color instead */
	Emu_text_line_t *line_array;

	int line_count;

	/* these are optional, not meant for subpixel fonts */
	float char_height;
	float line_height;
} Emu_font_text_config_t;

void
rlFont_drawText( Emu_font_text_config_t *config );
