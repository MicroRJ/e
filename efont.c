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

  rxtexture_t atlas = rxtexture_create(512,512,rxRGB8);
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
