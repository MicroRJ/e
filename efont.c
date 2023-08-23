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

/* dynamic glyph allocator #todo */

void *
Load_File_Data(char const *filePath, size_t *lpSize)
{
  void *file = ccopenfile(filePath,"r");
  ccu32_t length = 0;
  void *memory = ccpullfile(file,0,&length);

  if(lpSize) *lpSize = length;
  return memory;
}

edraw_text_config_t
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
  edraw_text_config_t config;
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
edraw_text( edraw_text_config_t *config );

void
edraw_text_easy( efont font, float height, rxcolor_t color,
  float x, float y, int length, char const *string)
{
  edraw_text_config_t config =
    draw_text_config_init(font,height,x,y,color,NULL,NULL,length,string);

  edraw_text( &config );
}

void
edraw_text( edraw_text_config_t *config )
{
  if(config->length == -1)
  {
    config->length = strlen(config->string);
  }

  if(config->length == 0)
  {
    return;
  }

  float scale = config->height / config->font.height;

  rx2d();
  rxpipset_varying(REG_VS_BLC_0,rx.imp.varying);
  rxpipset_varying(REG_PS_BLC_0,rx.imp.varying);
  rxpipset_program(rx.imp.sha_vtx,rx.imp.sha_pxl_txt);
  rxpipset_texture(REG_PS_TEX_0,config->font.glyphAtlas);
  rxpipset_sampler(REG_PS_SAM_0,rx.point_sampler);

  // if(config->font.isSubpixelReady)
  // {
  // } else
  // {
  // rxpipset_sampler(REG_PS_SAM_0,rx.linear_sampler);
  // }

  float unwiden = 1. / (config->font.isWidened ? 3. : 1.);

  for(int i=0;i<config->length;i+=1)
  {
    int chr = config->string[i];

    Glyph_Data d = config->font.glyphArray[chr - config->font.glyphCodeStart];
    Glyph_Quad q = config->font.glyphQuads[chr - config->font.glyphCodeStart];

    /* some fonts support the whitespace char other not, either way
     is safer to just early out or get this from the font itself  #todo */
    if( (chr ==  ' ') ||
        (chr == '\t') ||
        (chr == '\r') ||
        (chr == '\n') ) goto skip;

    if(config->color_table != NULL)
    {
      config->color = config->color_table[config->color_array[i]];
    }

    float x0 = config->x + d.offsetX * scale * unwiden;
    float y0 = config->y + d.offsetY * scale;


    if(i != 0)
    {
      if(FT_HAS_KERNING(config->font.freetype.face))
      {
        FT_Vector kerning2;

        int prev_glyph_index = FT_Get_Char_Index(config->font.freetype.face,config->string[i-1]);
        int glyph_index = FT_Get_Char_Index(config->font.freetype.face,chr);

        FT_Get_Kerning(config->font.freetype.face,
          prev_glyph_index,glyph_index,FT_KERNING_DEFAULT,&kerning2);

        x0 += kerning2.x >> 6;
      }
    }


    float x1 = x0 + d.imageWidth * scale * unwiden;
    float y1 = y0 + d.imageHeight * scale;

    rximp_begin();
      rx.imp.attr.rgba = config->color;
      rxaddvtx(rxvtx_xyuv(x0,y0,q.x0,q.y1));
      rxaddvtx(rxvtx_xyuv(x0,y1,q.x0,q.y0));
      rxaddvtx(rxvtx_xyuv(x1,y1,q.x1,q.y0));
      rxaddvtx(rxvtx_xyuv(x1,y0,q.x1,q.y1));
      rxaddnidx(6, 0,1,2, 0,2,3);
    rximp_end();

skip:
    config->x += d.advanceWidth * scale;
  }
}


typedef struct
{ struct
  { char const *file;
    void       *memory;
    int         length;
  } ttf;
  int         char_start;
  int         char_end;
  char const *char_string;
  int is_static;
  int edge_value;
  int sample_size;
  int padding;
  int supports_subpixel;
  int supports_sdf;
} efont_config_t;

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

} rxmemory_copy_config_t;

void
rxmemory_copy(
	rxmemory_copy_config_t *config )
{
  for (int y = 0; y < config->src.height; y += 1)
  {
    memcpy(
      config->dst.cursor + config->dst.stride * (y + config->dst.offset_y) + config->dst.offset_x,
      config->src.cursor + config->src.stride * (y + config->src.offset_y) + config->src.offset_x, config->src.stride );
  }
}

Glyph_Font
Load_Glyph_Font(
  char const *filePath, float requestedHeight)
{
	FT_Library library_ft;
	if(FT_Init_FreeType(&library_ft))
	{
		ccbreak();
	}

	FT_Face face_ft;
	if(FT_New_Face(library_ft,filePath,0,&face_ft))
	{
		ccbreak();
	}
	if(FT_Set_Pixel_Sizes(face_ft,0,requestedHeight))
	{
		ccbreak();
	}

	/* 16.16 to 24.6 to 32. */
  float units_to_pixels = face_ft->size->metrics.y_scale / 65536. / 64.;
  float lineGap = face_ft->height - (face_ft->ascender - face_ft->descender);

  float ascentInPixels = face_ft->ascender * units_to_pixels;
  float descentInPixels = face_ft->descender * units_to_pixels;
  float lineGapInPixels = lineGap * units_to_pixels;
  float lineHeightInPixels = requestedHeight;
  float fontHeightInPixels = requestedHeight;

	float tallestGlyph = 0;

	int glyphStart = 32;
  int glyphEnd   = 126;
  Glyph_Data *glyphArray = NULL;
  Glyph_Quad *glyphQuads = NULL;

  int atlasWidth = 1024;
  int atlasHeight = 1024;
  rxtexture_t glyphAtlas = rxtexture_create(atlasWidth,atlasHeight,rxRGB8);
  rxborrowed_t atlasMemory = rxtexture_borrow(glyphAtlas);

  rxmemory_copy_config_t atlas_copy_config;
  atlas_copy_config.dst.offset_x = 0;
  atlas_copy_config.dst.offset_y = 0;
  atlas_copy_config.dst.height   = atlasHeight;
  atlas_copy_config.dst.stride   = atlasMemory.stride;
  atlas_copy_config.dst.cursor   = atlasMemory.cursor;

  int codepoint;
  for ( codepoint  = glyphStart;
        codepoint  < glyphEnd;    codepoint += 1 )
  {
  	Glyph_Data *data = earray_add(glyphArray,1);
    Glyph_Quad *quad = earray_add(glyphQuads,1);
    memset(data,0,sizeof(*data));
    memset(quad,0,sizeof(*quad));

		quad->codepoint = codepoint;
    data->codepoint = codepoint;

		if(FT_Load_Char(face_ft,codepoint,FT_LOAD_DEFAULT))
		{
			cctracewar("failed to load glyph %c",codepoint);
			continue;
		}

	  if(FT_Render_Glyph(face_ft->glyph,FT_RENDER_MODE_LCD))
	  {
			cctracewar("failed to render glyph %c",codepoint);
			continue;
	  }

	  int imageWidth = face_ft->glyph->bitmap.width;
	  int imageHeight = face_ft->glyph->bitmap.rows;
	  unsigned char *imageData = face_ft->glyph->bitmap.buffer;

	  if(imageHeight > tallestGlyph)
    {
      tallestGlyph = imageHeight;
    }

    if(atlas_copy_config.dst.offset_x + imageWidth > atlasWidth)
    {
      atlas_copy_config.dst.offset_x = 0;
      atlas_copy_config.dst.offset_y += tallestGlyph; /* #todo */
    }

    if(imageData != NULL)
    {
		  atlas_copy_config.src.offset_x = 0;
		  atlas_copy_config.src.offset_y = 0;
		  atlas_copy_config.src.height = face_ft->glyph->bitmap.rows;
		  atlas_copy_config.src.stride = face_ft->glyph->bitmap.pitch;
		  atlas_copy_config.src.cursor = face_ft->glyph->bitmap.buffer;
			rxmemory_copy(&atlas_copy_config);
    }

    quad->x0 = atlas_copy_config.dst.offset_x / (float) atlasWidth;
    quad->y0 = atlas_copy_config.dst.offset_y / (float) atlasHeight;
    quad->x1 = quad->x0 + imageWidth / (float) atlasWidth;
    quad->y1 = quad->y0 + imageHeight / (float) atlasHeight;

    data->advanceWidth = face_ft->glyph->advance.x >> 6;
    data->offsetX = face_ft->glyph->bitmap_left;
    data->offsetY = face_ft->glyph->bitmap_top - imageHeight;
    data->imageWidth = imageWidth;
    data->imageHeight = imageHeight;

    atlas_copy_config.dst.offset_x += imageWidth;
  }

  rxreturn(atlasMemory);

#if 0

// #define FONT_SDF_PIXEL_DIST_SCALE 32
// #define FONT_SDF_CHAR_PADDING     8
// #define FONT_SDF_ON_EDGE_VALUE    128

#ifndef FONT_SDF_CHAR_PADDING
#define FONT_SDF_CHAR_PADDING         8
# endif
#ifndef FONT_SDF_ON_EDGE_VALUE
#define FONT_SDF_ON_EDGE_VALUE        0xff
# endif
#ifndef FONT_SDF_PIXEL_DIST_SCALE
#define FONT_SDF_PIXEL_DIST_SCALE     256-16
# endif

  size_t fileLength;
  void *fileMemory = Load_File_Data(filePath,&fileLength);

  /* store this for the dynamically adding glyphs and such */
  stbtt_fontinfo fontInfo;
  stbtt_InitFont(&fontInfo,fileMemory,0);

  int ascentEm,descentEm,lineGapEm;
  stbtt_GetFontVMetrics(&fontInfo,&ascentEm,&descentEm,&lineGapEm);

  float emToPixels = stbtt_ScaleForPixelHeight(&fontInfo,fontHeight);

  /* widen the glyphs to prepare for sub-pixel rendering, subsequent renderers
    must take this into account #noteworthy we could also get rid of it by simply
    encoding this larger image into a 3/4 component image, it would take the same
    space but it would be more friendly #pending */
  float wideningFactor = 3.;

  float lineHeight = (ascentEm - descentEm + lineGapEm) * emToPixels;

  float  ascent =  ascentEm * emToPixels,
        descent = descentEm * emToPixels,
        lineGap = lineGapEm * emToPixels;

  int atlasWidth  = 1024*8;
  int atlasHeight = 512;

  int tallestGlyph = 0;

  /* generate the atlas, should let the user pick the size since it is
    used for caching #todo, though you shouldn't worry too much about
    the initial size, this changes dynamically anyways, glyphs are baked in and out fifo style */
  Glyph_Data *glyphArray = NULL;
  Glyph_Quad *glyphQuads = NULL;

  rxtexture_t glyphAtlas = rxtexture_create(atlasWidth,atlasHeight,rxRGB8);
  rxborrowed_t atlasMemory = rxtexture_borrow(glyphAtlas);

  int atlasY = 0, atlasX = 0;

  int glyphStart = 32;
  int glyphEnd   = 126;
  int codepoint;
  for ( codepoint  = glyphStart;
        codepoint  < glyphEnd;    codepoint += 1 )
  {
    /* we have to add the glyph even if the character is invisible #todo */
    Glyph_Data *data = earray_add(glyphArray,1);
    Glyph_Quad *quad = earray_add(glyphQuads,1);

    int advanceWidthEm = 0;
    int leftSideBearingEm = 0;
    stbtt_GetCodepointHMetrics(&fontInfo,codepoint,&advanceWidthEm,&leftSideBearingEm);

    float advanceWidth = advanceWidthEm * emToPixels;
    float leftSideBearing = leftSideBearingEm * emToPixels;

    int imageWidth = .5 + advanceWidth * wideningFactor;
    int imageHeight = .5 + fontHeight;
    unsigned char *imageData = NULL;
    int offsetX = 0;
    int offsetY = 0;

    /* check for all the other invisible characters too #todo,
      especially once the user is allowed to specify the range */
    if(codepoint != ' ')
    {
    	if(1)
    	{
	   		imageData = stbtt_GetGlyphBitmapSubpixel(
	   			&fontInfo,emToPixels * wideningFactor,emToPixels,-.5,-.5,
	   				stbtt_FindGlyphIndex(&fontInfo,codepoint),&imageWidth,&imageHeight,&offsetX,&offsetY);
    	} else
    	{
	      imageData = stbtt_GetGlyphSDF(&fontInfo,emToPixels * wideningFactor,emToPixels,
	        /* cache this #todo */
	        stbtt_FindGlyphIndex(&fontInfo,codepoint),
	          FONT_SDF_CHAR_PADDING,FONT_SDF_ON_EDGE_VALUE,FONT_SDF_PIXEL_DIST_SCALE,
	            &imageWidth,&imageHeight,&offsetX,&offsetY);
    	}
    }

    if(imageHeight > tallestGlyph)
    {
      tallestGlyph = imageHeight;
    }

    if(atlasX + imageWidth > atlasWidth)
    {
      atlasX = 0;
      atlasY += tallestGlyph; /* #todo */
    }

    quad->codepoint = codepoint;
    quad->x0 = atlasX / (float)atlasWidth;
    quad->y0 = atlasY / (float)atlasHeight;
    quad->x1 = (atlasX + imageWidth) / (float)atlasWidth;
    quad->y1 = (atlasY + imageHeight) / (float)atlasHeight;
    data->codepoint = codepoint;
    data->advanceWidth = advanceWidth;
    data->offsetX = offsetX;
    data->offsetY = (lineHeight - imageHeight - offsetY)  - lineHeight;
    data->imageWidth = imageWidth;
    data->imageHeight = imageHeight;
    data->imageData = imageData;

    if(imageData != NULL)
    {
      for (int imageY = 0; imageY < imageHeight; imageY += 1)
      {
        memcpy(
          atlasMemory.cursor +
          atlasMemory.stride * (imageY + atlasY) + atlasX,
          imageData  +
          imageWidth * imageY, imageWidth);
      }

      atlasX += imageWidth;
    }
  }

  rxreturn(atlasMemory);
#endif

leave:
  Glyph_Font font;
  font.isWidened = TRUE;
  font.isSubpixelReady = TRUE;
  font.isSDF = TRUE;
  font.filePath = filePath;
  font.glyphCodeStart = glyphStart;
  font.glyphCodeEnd = glyphEnd;
  font.glyphAtlas = glyphAtlas;
  font.glyphArray = glyphArray;
  font.glyphQuads = glyphQuads;
  font.ascent = ascentInPixels;
  font.descent = descentInPixels;
  font.lineGap = lineGapInPixels;
  font.lineHeight = lineHeightInPixels;
  font.height = fontHeightInPixels;
  font.freetype.face = face_ft;
  return font;
}


efont
efont_load(const char *, float);

void
efont_free(efont font);

/* todo: support for kerning */
ccinle float
efont_code_xadv(
  efont font, float height, int code)
{
  if(code < 32)
  {
    return 0;
  }

  return font.glyphArray[code - 32].advanceWidth * (height / font.height);
}

ccinle float
efont_code_width(
  efont font, float height, int code)
{
  if(code < 32)
  {
    return 0;
  }
  return font.glyphArray[code - 32].advanceWidth * (height / font.height);
}

void
efont_free(efont font)
{
  // ccfree(font.array);
  // rxtexture_delete(font.atlas);
}

efont
efont_load(const char *fpath, float vsize)
{
  return Load_Glyph_Font(fpath,vsize);
}
