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
  char codepoint;
  int imageWidth;
  int imageHeight;
  void *imageData;/* we don't want to allocate it this way #todo #temporary */

  float offsetX;
  float offsetY;
  float advanceWidth;
} Glyph_Data;

typedef struct
{
  int codepoint;
  float x0,y0,x1,y1;
} Glyph_Quad;

typedef struct
{
  char const *filePath;

  rxtexture_t  glyphAtlas;
  Glyph_Data  *glyphArray;
  Glyph_Quad  *glyphQuads;

  int glyphCodeStart;
  int glyphCodeEnd;

  float height;
  /* should probably round these #todo */
  float ascent;
  float descent;
  float lineGap;

  float lineHeight;
} Glyph_Font;

typedef Glyph_Font efont;