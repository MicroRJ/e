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


void *Load_File_Data(char const *, size_t *);

typedef struct
{
  int codepoint;
  int imageWidth;
  int imageHeight;
  char *imageData;
  float offsetX;
  float offsetY;
  float advanceWidth;
} Glyph_Data;

typedef struct
{
  char const *filePath;
  Glyph_Data *glyphData;
  float height;
  /* should probably round these :todo */
  float ascent;
  float descent;
  float lineGap;
} Glyph_Font;

void *Load_File_Data(char const *filePath, size_t *lpSize)
{
  void *file = ccopenfile(filePath,"r");
  ccu32_t length = 0;
  void *memory = ccpullfile(file,0,&length);

  if(lpSize) *lpSize = length;
  return memory;
}

Glyph_Font
Load_Glyph_Font(
  char const *filePath, float fontHeight)
{

#ifndef FONT_SDF_CHAR_PADDING
#define FONT_SDF_CHAR_PADDING         4
# endif
#ifndef FONT_SDF_ON_EDGE_VALUE
#define FONT_SDF_ON_EDGE_VALUE        128
# endif
#ifndef FONT_SDF_PIXEL_DIST_SCALE
#define FONT_SDF_PIXEL_DIST_SCALE     64.
# endif
#ifndef FONT_BITMAP_ALPHA_THRESHOLD
#define FONT_BITMAP_ALPHA_THRESHOLD   80
# endif

  size_t fileLength;
  void *fileMemory = Load_File_Data(filePath,&fileLength);

  stbtt_fontinfo fontInfo;
  stbtt_InitFont(&fontInfo,fileMemory,0);

  int ascent,descent,lineGap;
  stbtt_GetFontVMetrics(&fontInfo,&ascent,&descent,&lineGap);

  float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo,fontHeight);

  Glyph_Data *glyphArray = NULL;

  int glyphStart = 32;
  int glyphCount = 96;
  int codepoint;
  for ( codepoint  = glyphStart;
        codepoint  < glyphStart + glyphCount;
        codepoint += 1 )
  {
    Glyph_Data *glyph = earray_add(glyphArray,1);

    int advanceWidth;
    int leftSideBearing;
    stbtt_GetCodepointHMetrics(&fontInfo,codepoint,&advanceWidth,&leftSideBearing);

    int imageWidth,imageHeight;
    int offsetX,offsetY;
    void *imageData;

    imageData = stbtt_GetCodepointSDF(&fontInfo,scaleFactor,codepoint,
        FONT_SDF_CHAR_PADDING,FONT_SDF_ON_EDGE_VALUE,FONT_SDF_PIXEL_DIST_SCALE,
          &imageWidth,&imageHeight,&offsetX,&offsetY);

    glyph->codepoint = codepoint;
    glyph->advanceWidth = advanceWidth*scaleFactor;
    glyph->offsetX = offsetX;
    glyph->offsetY = offsetY + ascent*scaleFactor;
    glyph->imageWidth = imageWidth;
    glyph->imageHeight = imageHeight;
    glyph->imageData = imageData;
  }

  Glyph_Font font;
  font.filePath = filePath;
  font.glyphData = glyphArray;
  font.height = fontHeight;
  font.ascent = ascent * scaleFactor;
  font.descent = descent * scaleFactor;
  font.lineGap = lineGap * scaleFactor;

  return font;
}

typedef struct
{
  char const      *fpath;

  rxtexture_t      atlas;
  stbtt_bakedchar *array;

  float            vsize;
  int              vline;
} efont;

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

  stbtt_bakedchar c = font.array[code - 32];
  return c.xadvance;
}

ccinle float
efont_code_xoff(
  efont font, int code)
{
  if(code < 32)
  {
    return 0;
  }

  stbtt_bakedchar c = font.array[code - 32];
  int round_x = STBTT_ifloor(c.xoff + .5);
  return round_x;
}

ccinle float
efont_code_width(
  efont font, int code)
{
  if(code < 32)
  {
    return 0;
  }

  stbtt_bakedchar c = font.array[code - 32];

  int round_x = STBTT_ifloor(c.xoff + .5);
  return round_x + c.x1 - c.x0;
}

void
efont_free(efont font)
{
  ccfree(font.array);
  rxtexture_delete(font.atlas);
}

efont
efont_load(const char *fpath, float vsize)
{
  void *file = ccopenfile(fpath,"r");

  ccu32_t length = 0;
  unsigned char *memory = ccpullfile(file,0,&length);

  rxtexture_t atlas = rxtexture_create(1024,512,rxRGB8);
  rxborrowed_t borrow = rxtexture_borrow(atlas);

  stbtt_bakedchar *array = ccmalloc(sizeof(stbtt_bakedchar) * 0x80);
  stbtt_BakeFontBitmap(memory,0,vsize,borrow.memory,atlas.size_x,atlas.size_y,32,96,array);

  ccclosefile(file);
  rxreturn(borrow);
  ccfree(memory);

  efont font;
  font.fpath = fpath;
  font.vsize = vsize;
  font.vline = vsize;
  font.atlas = atlas;
  font.array = array;

  return font;
}



void
efont_get_quad(
  efont font, float vsize,
    stbtt_aligned_quad *q, int index,
      float *xpos, float *ypos )
{
  float subpixel_bias = -.5;

  float scale = vsize / font.vsize;

  stbtt_bakedchar *b = font.array + index;

  q->x0 = subpixel_bias + STBTT_ifloor((*xpos + b->xoff * scale) + .5);
  q->y0 = subpixel_bias + STBTT_ifloor((*ypos + b->yoff * scale) + .5);

  q->x1 = q->x0 + (b->x1 - b->x0) * scale;
  q->y1 = q->y0 + (b->y1 - b->y0) * scale;

  float ipw = 1. / font.atlas.size_x;
  float iph = 1. / font.atlas.size_y;
  q->s0 = b->x0 * ipw;
  q->t0 = b->y0 * iph;
  q->s1 = b->x1 * ipw;
  q->t1 = b->y1 * iph;

  *xpos += b->xadvance * scale;
}


/* todo: this has to become more advanced */
void
edraw_text(
  efont font, float vsize, rxcolor_t color,
    float x, float y, int length, char const *string)
{
  if(length == -1)
  {
    length = strlen(string);
  }

  if(length == 0)
  {
    return;
  }

  float scale = vsize / font.vsize;

  rximp_apply();
  rxpipset_program(rx.imp.sha_vtx,rx.imp.sha_pxl_txt);
  rxpipset_texture(REG_PS_TEX_0,font.atlas);
  rxpipset_sampler(REG_PS_SAM_0,rx.linear_sampler);

  for(int i=0;i<length;i+=1)
  {
    int code = *string ++;

    if(code < 32)
      continue;

    stbtt_bakedchar c = font.array[code-32];

    stbtt_aligned_quad q;
    efont_get_quad(font,vsize,&q,code-32,&x,&y);

    float yyy = q.y1 - q.y0;
    float xxx = q.x1 - q.x0;

    q.y0 -= 2*c.yoff*scale + yyy;
    q.y1 -= 2*c.yoff*scale + yyy;

    q.x1 = q.x0 + xxx;
    q.y1 = q.y0 + yyy;

    rxbgn();
      rx.imp.attr.rgba = color;
      rxaddvtx(rxvtx_xyuv(q.x0,q.y0,q.s0,q.t1));
      rxaddvtx(rxvtx_xyuv(q.x0,q.y1,q.s0,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y1,q.s1,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y0,q.s1,q.t1));
      rxaddnidx(6, 0,1,2, 0,2,3);
    rxend();

  }
}

