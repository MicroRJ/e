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


void
EMU_apply_pipeline_for_font( Glyph_Font *font )
{
	int mode = EMU_IMP_MODE_2D;

	if (font->is_subpixel) {
		mode = EMU_IMP_MODE_LCD_TEXT;
	} else
	if (font->is_sdf) {
		mode = EMU_IMP_MODE_SDF_TEXT;
	}

	EMU_imp_apply(mode,FALSE);

  rxpipset_texture(REG_PS_TEX_0,font->glyphAtlas,FALSE);
}

float
EMU_font_get_kerning(
  Glyph_Font *font, char prev_char, char curr_char )
{
  float result = .0;

#if 1
  if(FT_HAS_KERNING(font->freetype.face))
  {
    FT_Vector kerning2;
    FT_Get_Kerning(font->freetype.face,
    /* may need to store this #todo */
      FT_Get_Char_Index(font->freetype.face,prev_char),
      FT_Get_Char_Index(font->freetype.face,curr_char),
        FT_KERNING_DEFAULT, &kerning2);

    result += kerning2.x / 64.;
  }
#endif

  return result;
}
void
edraw_text( EMU_draw_text_config_t *config )
{
  if(config->length == -1)
  {
    config->length = strlen(config->string);
  }

  if(config->length == 0)
  {
    return;
  }

  EMU_apply_pipeline_for_font(&config->font);

  float scale = config->height / config->font.height;
  float unwiden = 1. / (config->font.is_subpixel ? 3. : 1.);

  for(int i=0;i<config->length;i+=1)
  {
    int chr = config->string[i];

    Glyph_Data d = config->font.glyphArray[chr - config->font.charset_start];

    /* some fonts support the whitespace char other not, either way
     is safer to just early out or get this from the font itself  #todo */
    if IS_WHITESPACE(chr) {
      goto skip_rendering;
    }

    if IS_NOT_NULL(config->color_table) {
      config->color = config->color_table[config->color_array[i]];
    }

    float x0 = config->x + d.offsetX * unwiden;
    float y0 = config->y + d.offsetY;

    if (i != 0) {
      x0 += EMU_font_get_kerning( &config->font, config->string[i-1], chr );
    }

    x0 *= scale;
    y0 *= scale;

    float x1 = x0 + d.imageWidth * scale * unwiden;
    float y1 = y0 + d.imageHeight * scale;

    float xnor = 1. / config->font.glyphAtlas->size_x;
    float ynor = 1. / config->font.glyphAtlas->size_y;

    EMU_imp_begin(6,4);
      rx.imp.attr.rgba = config->color;
      rxaddnidx(6, 0,1,2, 0,2,3);
      rxaddnvtx(4,
      	rxvtx_xyuv(x0,y0, d.x0 * xnor, d.y1 * ynor),
      	rxvtx_xyuv(x0,y1, d.x0 * xnor, d.y0 * ynor),
				rxvtx_xyuv(x1,y1, d.x1 * xnor, d.y0 * ynor),
				rxvtx_xyuv(x1,y0, d.x1 * xnor, d.y1 * ynor));
    EMU_imp_end();

skip_rendering:
    config->x += d.advanceWidth * scale;
  }
}


#pragma warning(disable:4701)
#pragma warning(disable:4703)
Glyph_Font
EMU_load_glyph_font(
  char const *filePath, float requestedHeight)
{
  Glyph_Font font;

  int glyphStart = 32;
  int glyphEnd = 126;

  float ascentInPixels;
  float descentInPixels;
  float lineGapInPixels;
  float fontHeightInPixels;
  float lineHeightInPixels;
  Glyph_Data *glyphArray;
  Emu_texture_t *glyphAtlas;



  int is_subpixel;
  int is_sdf;

#if 1
  FT_Library library_ft;
  if(FT_Init_FreeType(&library_ft)) {
    cctraceerr("failed to init FreeType");
    goto leave;
  }

  FT_Face face_ft;
  if(FT_New_Face(library_ft,filePath,0,&face_ft)) {
    cctraceerr("failed to load file path");
    goto leave;
  }

  if(FT_Set_Pixel_Sizes(face_ft,0,requestedHeight)) {
    cctraceerr("invalid pixel size");
    goto leave;
  }

  is_subpixel = TRUE;
  is_sdf = FALSE;

  /* 16.16 to 24.6 to 32. */
  float units_to_pixels = face_ft->size->metrics.y_scale / 65536. / 64.;
  float lineGap = face_ft->height - (face_ft->ascender - face_ft->descender);

  ascentInPixels = face_ft->ascender * units_to_pixels;
  descentInPixels = face_ft->descender * units_to_pixels;
  lineGapInPixels = lineGap * units_to_pixels;
  lineHeightInPixels = requestedHeight;
  fontHeightInPixels = requestedHeight;

  float tallestGlyph = 0;

  int atlasWidth = 1024;
  int atlasHeight = 1024;
  glyphAtlas = Emu_texture_create_simple(atlasWidth,atlasHeight,EMU_FORMAT_R8_UNORM,0,NULL);

  int atlasStride;
  unsigned char *atlasMemory = Emu_texture_borrow(glyphAtlas,&atlasStride);

  glyphArray = NULL;

  Emu_memcpy2d_config_t atlas_copy_config;
  atlas_copy_config.dst.offset_x = 0;
  atlas_copy_config.dst.offset_y = 0;
  atlas_copy_config.dst.height   = atlasHeight;
  atlas_copy_config.dst.stride   = atlasStride;
  atlas_copy_config.dst.cursor   = atlasMemory;

  int codepoint;
  for ( codepoint  = glyphStart;
        codepoint  < glyphEnd;    codepoint += 1 )
  {
    Glyph_Data *data = earray_add(glyphArray,1);
    memset(data,0,sizeof(*data));

    data->codepoint = codepoint;

    if(FT_Load_Char(face_ft,codepoint,FT_LOAD_DEFAULT))
    {
      cctracewar("failed to load glyph %c",codepoint);
      continue;
    }

    // FT_RENDER_MODE_LCD
    // FT_RENDER_MODE_NORMAL

    if(FT_Render_Glyph(face_ft->glyph,FT_RENDER_MODE_LCD)) {
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
      EMU_memcpy2d(&atlas_copy_config);
    }

    data->x0 = atlas_copy_config.dst.offset_x;
    data->y0 = atlas_copy_config.dst.offset_y;
    data->x1 = data->x0 + imageWidth;
    data->y1 = data->y0 + imageHeight;

    data->advanceWidth = face_ft->glyph->advance.x / 64.;
    data->offsetX = face_ft->glyph->bitmap_left;
    data->offsetY = face_ft->glyph->bitmap_top - imageHeight;
    data->imageWidth = imageWidth;
    data->imageHeight = imageHeight;

    atlas_copy_config.dst.offset_x += imageWidth;
  }

leave:
  // FT_Done_Library(library_ft);
  font.freetype.face = face_ft;
#else
  is_subpixel = FALSE;
  is_sdf = TRUE;

// #define FONT_SDF_PIXEL_DIST_SCALE 32
// #define FONT_SDF_CHAR_PADDING     8
// #define FONT_SDF_ON_EDGE_VALUE    128
#ifndef FONT_SDF_CHAR_PADDING
#define FONT_SDF_CHAR_PADDING         8
# endif
#ifndef FONT_SDF_ON_EDGE_VALUE
#define FONT_SDF_ON_EDGE_VALUE        128
# endif
#ifndef FONT_SDF_PIXEL_DIST_SCALE
#define FONT_SDF_PIXEL_DIST_SCALE     32
# endif
  size_t fileLength;
  void *fileMemory = EMU_load_file_data(filePath,&fileLength);

  if IS_NULL(fileMemory) {
    cctraceerr("failed to load file");
    goto leave;
  }

  /* store this for the dynamically adding glyphs and such */
  stbtt_fontinfo fontInfo;
  if(stbtt_InitFont(&fontInfo,fileMemory,0) == 0) {
    cctraceerr("failed to init font");
    goto leave;
  }

  float emToPixels = stbtt_ScaleForPixelHeight(&fontInfo,requestedHeight);

  int ascentEm,descentEm,lineGapEm;
  stbtt_GetFontVMetrics(&fontInfo,&ascentEm,&descentEm,&lineGapEm);

  int atlasWidth  = 1024*2;
  int atlasHeight = 512;

  fontHeightInPixels = requestedHeight;
  lineHeightInPixels = (ascentEm - descentEm + lineGapEm) * emToPixels;
  ascentInPixels =  ascentEm * emToPixels,
  descentInPixels = descentEm * emToPixels,
  lineGapInPixels = lineGapEm * emToPixels;
  glyphArray = NULL;
  glyphAtlas = Emu_texture_create_simple(atlasWidth,atlasHeight,EMU_FORMAT_R8_UNORM);
  atlasMemory = Emu_texture_borrow(glyphAtlas);

  /* widen the glyphs to prepare for sub-pixel rendering, subsequent renderers
    must take this into account #noteworthy we could also get rid of it by simply
    encoding this larger image into a 3/4 component image, it would take the same
    space but it would be more friendly #pending */
  float wideningFactor = 1.;
  int tallestGlyph = 0;

  /* generate the atlas, should let the user pick the size since it is
    used for caching #todo, though you shouldn't worry too much about
    the initial size, this changes dynamically anyways, glyphs are baked in and out fifo style */

  int atlasY = 0, atlasX = 0;

  int codepoint;
  for ( codepoint  = glyphStart;
        codepoint  < glyphEnd;    codepoint += 1 )
  {
    /* we have to add the glyph even if the character is invisible #todo */
    Glyph_Data *data = earray_add(glyphArray,1);

    int advanceWidthEm = 0;
    int leftSideBearingEm = 0;
    stbtt_GetCodepointHMetrics(&fontInfo,codepoint,&advanceWidthEm,&leftSideBearingEm);

    float advanceWidth = advanceWidthEm * emToPixels;
    float leftSideBearing = leftSideBearingEm * emToPixels;

    int imageWidth = .5 + advanceWidth * wideningFactor;
    int imageHeight = .5 + requestedHeight;
    unsigned char *imageData = NULL;
    int offsetX = 0;
    int offsetY = 0;

    /* check for all the other invisible characters too #todo,
      especially once the user is allowed to specify the range */
    if(codepoint != ' ')
    {
      if(0)
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

    data->codepoint = codepoint;
    data->x0 = atlasX;
    data->y0 = atlasY;
    data->x1 = atlasX + imageWidth;
    data->y1 = atlasY + imageHeight;
    data->codepoint = codepoint;
    data->advanceWidth = advanceWidth;
    data->offsetX = offsetX;
    data->offsetY = - offsetY - imageHeight ;
    data->imageWidth = imageWidth;
    data->imageHeight = imageHeight;

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

leave:
  font.stb.face = fontInfo;
#endif

  font.is_subpixel = is_subpixel;
  font.is_sdf = is_sdf;
  font.filePath = filePath;
  font.charset_start = glyphStart;
  font.charset_end = glyphEnd;
  font.glyphAtlas = glyphAtlas;
  font.glyphArray = glyphArray;
  font.ascent = ascentInPixels;
  font.descent = descentInPixels;
  font.lineGap = lineGapInPixels;
  font.lineHeight = lineHeightInPixels;
  font.height = fontHeightInPixels;


  Emu_texture_return(glyphAtlas);


  return font;
}

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
