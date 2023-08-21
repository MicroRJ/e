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

  rximp_apply();
  rxpipset_program(rx.imp.sha_vtx,rx.imp.sha_pxl_txt);
  rxpipset_texture(REG_PS_TEX_0,config->font.glyphAtlas);
  rxpipset_sampler(REG_PS_SAM_0,rx.linear_sampler);



  for(int i=0;i<config->length;i+=1)
  {
    int codepoint = *config->string ++;

    Glyph_Data d = config->font.glyphArray[codepoint - config->font.glyphCodeStart];
	  Glyph_Quad q = config->font.glyphQuads[codepoint - config->font.glyphCodeStart];

	  /* some fonts support the whitespace char other not, either way
	   is safer to just early out or get this from the font itself  #todo */
	  if( (codepoint ==  ' ') ||
	      (codepoint == '\t') ||
	      (codepoint == '\r') ||
	      (codepoint == '\n') ) goto skip;

    if(config->color_table != NULL)
    {
    	config->color = config->color_table[config->color_array[i]];
    }

    float x0 = config->x + d.offsetX;
  	float y0 = config->y + d.offsetY;

	  float x1 = x0 + d.imageWidth;
	  float y1 = y0 + d.imageHeight;

	  rximp_begin();
	    rx.imp.attr.rgba = config->color;
	    rxaddvtx(rxvtx_xyuv(x0,y0,q.x0,q.y1));
	    rxaddvtx(rxvtx_xyuv(x0,y1,q.x0,q.y0));
	    rxaddvtx(rxvtx_xyuv(x1,y1,q.x1,q.y0));
	    rxaddvtx(rxvtx_xyuv(x1,y0,q.x1,q.y1));
	    rxaddnidx(6, 0,1,2, 0,2,3);
	  rximp_end();

skip:
	  config->x += d.advanceWidth;
  }
}


Glyph_Font
Load_Glyph_Font(
  char const *filePath, float fontHeight)
{

#ifndef FONT_SDF_CHAR_PADDING
#define FONT_SDF_CHAR_PADDING         16
# endif
#ifndef FONT_SDF_ON_EDGE_VALUE
#define FONT_SDF_ON_EDGE_VALUE        128
# endif
#ifndef FONT_SDF_PIXEL_DIST_SCALE
#define FONT_SDF_PIXEL_DIST_SCALE     128.
# endif

  size_t fileLength;
  void *fileMemory = Load_File_Data(filePath,&fileLength);

  stbtt_fontinfo fontInfo;
  stbtt_InitFont(&fontInfo,fileMemory,0);

  float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo,fontHeight);

  int ascent,descent,lineGap;
  stbtt_GetFontVMetrics(&fontInfo,&ascent,&descent,&lineGap);
  float lineHeight = (ascent - descent + lineGap) * scaleFactor;

  int atlasWidth  = 1024;
  int atlasHeight = 1024;

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

    int advanceWidth = 0;
    int leftSideBearing = 0;
    stbtt_GetCodepointHMetrics(&fontInfo,codepoint,&advanceWidth,&leftSideBearing);

    int imageWidth = advanceWidth * scaleFactor;
    int imageHeight = fontHeight;
    unsigned char *imageData = NULL;
    int offsetX = 0;
    int offsetY = 0;

    /* check for all the other invisible characters too #todo,
      especially once the user is allowed to specify the range */
    if(codepoint != ' ')
    {
      imageData = stbtt_GetCodepointSDF(&fontInfo,scaleFactor,codepoint,
          FONT_SDF_CHAR_PADDING,FONT_SDF_ON_EDGE_VALUE,FONT_SDF_PIXEL_DIST_SCALE,
            &imageWidth,&imageHeight,&offsetX,&offsetY);

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
    data->advanceWidth = advanceWidth*scaleFactor;
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

  Glyph_Font font;
  font.filePath = filePath;
  font.glyphCodeStart = glyphStart;
  font.glyphCodeEnd = glyphEnd;
  font.glyphAtlas = glyphAtlas;
  font.glyphArray = glyphArray;
  font.glyphQuads = glyphQuads;
  font.ascent = ascent * scaleFactor;
  font.descent = descent * scaleFactor;
  font.lineGap = lineGap * scaleFactor;
  font.lineHeight = lineHeight;
  font.height = fontHeight;

  return font;
}


efont
efont_load(const char *, float);

void
efont_free(efont font);

/* todo: support for kerning */
ccinle float
efont_code_xadv(
  efont font, int code)
{
  if(code < 32)
  {
    return 0;
  }

  return font.glyphArray[code - 32].advanceWidth;
}

ccinle float
efont_code_xoff(
  efont font, int code)
{
  if(code < 32)
  {
    return 0;
  }

  return font.glyphArray[code - 32].offsetX;
}

ccinle float
efont_code_width(
  efont font, int code)
{
  if(code < 32)
  {
    return 0;
  }
  return font.glyphArray[code - 32].advanceWidth;
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
