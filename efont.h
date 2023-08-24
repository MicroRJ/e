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

/* fonts api only supports ascii characters at the moment */

typedef struct
{
	short    offset_x;
	short    offset_y;
	float   walking_x;
	short bbox_size_x;
	short bbox_size_y;
} EMU_bitmap_glyph_t;

typedef struct
{
  char  codepoint;
  int   imageWidth;
  int   imageHeight;

  short offsetX;
  short offsetY;
  float advanceWidth;

  short x0,y0,x1,y1;
} Glyph_Data;

typedef Glyph_Data Glyph_Quad;

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
} EMU_font_config_t;

/* everything is expressed in a y-upwards coordinate system */
typedef struct
{
  char const *filePath;

  Emu_texture_t *glyphAtlas;
  Glyph_Data  	*glyphArray;

  int charset_start;
  int charset_end;

  float height;

  float lineHeight;
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
} Glyph_Font;

typedef Glyph_Font efont;

/* is either the font knows about the renderer or the renderer knows about the font
	or this becomes a separate file #pending */
typedef struct
{
	efont            font;
	float            height;
	float            x,y;
	rxcolor_t        color;/* color is used if the color_array is null */
	rxcolor_t     *  color_table;
	unsigned char *  color_array;
	int              length;
	char const     * string;
} EMU_draw_text_config_t;

ccinle EMU_draw_text_config_t
draw_text_config_init(
  efont            font,
  float            height,
  float            x,
  float            y,
  rxcolor_t        color,
  rxcolor_t     *  color_table,
  unsigned char *  color_array,
  int              length,
  char const     * string )
{
  EMU_draw_text_config_t config;
  config.font        = font;
  config.height      = height;
  config.x           = x;
  config.y           = y;
  config.color       = color;
  config.color_table = color_table;
  config.color_array = color_array;
  config.length      = length;
  config.string      = string;
  return config;
}
void
edraw_text( EMU_draw_text_config_t *config );
