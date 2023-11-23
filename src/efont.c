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


// this whole thing is garbage, we need a static allocator, we should then
// create a font atlas and that's it ...

/* #todo */
lgi_API inline void
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

/* trying this out, see if I like it #pending */
#define each_in(decl_type,decl_name,arr)\
(decl_type decl_name = arr; decl_name < arrend(arr); decl_name += 1)

lui_FontGlyph *
rlFont_makeGlyph(lui_GlyphCol *pallet, int index, int utf32
,	short x0, short y0, short x1, short y1, short offset_x, short offset_y, float walking_x)
{
	lui_FontGlyph glyph;
	glyph.pallet = pallet;
	glyph.external_index = index;
	glyph.utf32 = utf32;
	glyph.x0 = x0;
	glyph.y0 = y0;
	glyph.x1 = x1;
	glyph.y1 = y1;
	glyph.offset_x = offset_x;
	glyph.offset_y = offset_y;
	glyph.walking_x = walking_x;

	lui_FontGlyph *result = lgi__allocate_typeof(lui_FontGlyph);
	*result = glyph;
	return result;
}

lui_GlyphRow *
rlFont_findOrMakeGlyphBucket(int width, int height, int *x, int *y)
{
	lui_GlyphRow *result = NULL;

	/* try to find a better pallet based on the glyph's face #todo */
	for each_in(lui_GlyphCol **, pallet_, lui.pallets) {

		lui_GlyphCol *pallet = *pallet_;

		lgi_Bitmap storage = pallet->storage;

		if(storage.size_y < height) {
			continue;
		}

		if(storage.size_x < width) {
			continue;
		}

		/* search each bucket in the pallet */
		for each_in(lui_GlyphRow **, bucket_, pallet->buckets) {

			lui_GlyphRow *bucket = *bucket_;

			if (bucket == NULL) {
				continue;
			}

			/* check if the bucket is too small or tall or if it ran
				out of space */
			if (bucket->height < height) {
				continue;
			}
			if (bucket->height > height * 1.3) {
				continue;
			}
			if (bucket->cursor_x + width > storage.size_x) {
				continue;
			}

			result = bucket;
			break;
		}

		/* the bucket was found, exit the loop */
		if(result != NULL) {
			break;
		}

		/* otherwise attempt to add a bucket if there's enough space */
		int leftover = storage.size_y - pallet->cursor_y;

		if(leftover < height) {
			continue;
		}

		/* add some extra space to spice things up if possible */
		height *= 1.25;

		if (height > leftover) {
			height = leftover;
		}

		/* maybe sort the array #todo #pending */
		result = lgi__allocate_typeof(lui_GlyphRow);

		if(result != NULL) {

			result->pallet = pallet;
			result->height = height;
			result->cursor_x = 0;
			result->cursor_y = pallet->cursor_y;

			*arradd(pallet->buckets,1) = result;
		}

		pallet->cursor_y += height;

		if(result != NULL) {
			break;
		}
	}

	if(result == NULL)
	{
		lui_GlyphCol *pallet = lgi__allocate_typeof(lui_GlyphCol);
		pallet->buckets = NULL;
		pallet->texture = NULL;
		pallet->storage = lgi_allocateTextureContents(1024,1024,lgi_Format_R8_UNORM);
		pallet->cursor_y = 0;

		*arradd(lui.pallets,1) = pallet;

		/* maybe sort the array #todo #pending */
		result = lgi__allocate_typeof(lui_GlyphRow);
		result->pallet = pallet;
		result->height = height;
		result->cursor_x = 0;
		result->cursor_y = 0;

		pallet->cursor_y += height;

		*arradd(pallet->buckets,1) = result;

		lgi_ensure(result != NULL);
	}

	if(result != NULL) {

		if(x != NULL) {
			*x = result->cursor_x;
		}

		if(y != NULL) {
			*y = result->cursor_y;
		}

		result->cursor_x += width;
	}

	return result;
}

lui_FontGlyph *
rlFont_makeGlyphFromIndex(lui_Font *lpFont, int glyph_index, int utf32) {

	FT_GlyphSlot glyph = NULL;
	int glyph_width  = 0;
	int glyph_height = 0;
	int glyph_stride = 0;
	int dest_x = 0;
	int dest_y = 0;
	lui_GlyphRow *lpBucket = NULL;
	lui_GlyphCol *lpPallet = NULL;
	FT_Face lpFace = lpFont->freetype.face;

	if (glyph_index == 0) {
		goto L_backdoor;
	}
	if (FT_Load_Glyph(lpFace,glyph_index,FT_LOAD_DEFAULT)) {
		goto L_backdoor;
	}
	if (FT_Render_Glyph(lpFace->glyph,FT_RENDER_MODE_LCD)) {
		goto L_backdoor;
	}

	glyph = lpFace->glyph;

	glyph_width = glyph->bitmap.width;
	glyph_height = glyph->bitmap.rows;
	glyph_stride = glyph->bitmap.pitch;
	unsigned char *glyph_memory = glyph->bitmap.buffer;


	if (glyph_memory != NULL) {
		lpBucket = rlFont_findOrMakeGlyphBucket(glyph_width,glyph_height,&dest_x,&dest_y);

		if (lpBucket != NULL) {
			lpPallet = lpBucket->pallet;

			lpBucket->cursor_x += glyph_width;

			Emu_memcpy2d_config_t cpy_cfg;
			cpy_cfg.dst.offset_x = dest_x;
			cpy_cfg.dst.offset_y = dest_y;
			cpy_cfg.dst.height   = lpPallet->storage.size_y;
			cpy_cfg.dst.stride   = lpPallet->storage.stride;
			cpy_cfg.dst.cursor   = lpPallet->storage.memory;

			cpy_cfg.src.offset_x = 0;
			cpy_cfg.src.offset_y = 0;
			cpy_cfg.src.height = glyph_height;
			cpy_cfg.src.stride = glyph_stride;
			cpy_cfg.src.cursor = glyph_memory;

			rlMem_copy2d(&cpy_cfg);

			lpPallet->dirty = TRUE;
		}
	}

	L_backdoor:

	lui_FontGlyph *lpGlyph;
	if (glyph == NULL) {
		lpGlyph = rlFont_makeGlyph(lpPallet,glyph_index,utf32,0,0,0,0,0,0,0);
	} else {
		lpGlyph = rlFont_makeGlyph(lpPallet,glyph_index,utf32
		,	dest_x,dest_y,dest_x+glyph_width,dest_y+glyph_height
		,	glyph->bitmap_left,glyph->bitmap_top-glyph_height
		, 	glyph->advance.x/64.);
	}

	*arradd(lpFont->glyph_table,1) = lpGlyph;

	return lpGlyph;
}

lui_FontGlyph *
rlFont_makeGlyphFromName (lui_Font *lpFont, char const *lpName, int utf32) {

	return rlFont_makeGlyphFromIndex(lpFont,FT_Get_Name_Index(lpFont->freetype.face,lpName),utf32);
}

lui_FontGlyph *
lui__findOrMakeGlyphByUnicode(lui_Font *lpFont, int utf32) {

	/* [[SPEED]] */
	for each_in(lui_FontGlyph **, it_, lpFont->glyph_table) {
		lui_FontGlyph *it = *it_;
		if (it != NULL) {
			if (it->utf32 == utf32) {
				return it;
			}
		}
	}

	int index = FT_Get_Char_Index(lpFont->freetype.face,utf32);
	if (index == 0) {
		printf("%s[%i] %s(): '%i': <%c> glyph not found\n"
		, __FILE__,__LINE__,__func__,utf32,utf32);
	}
	return rlFont_makeGlyphFromIndex(lpFont,index,utf32);
}

float
rlFont_getKerning(lui_Font *lpFont, int prev, int code) {
	float result = .0;
	if (FT_HAS_KERNING(lpFont->freetype.face)) {
		/* [[TODO]]: you already have the indexes stored */
		FT_Vector kerning2;
		FT_Get_Kerning(lpFont->freetype.face
		, FT_Get_Char_Index(lpFont->freetype.face,prev)
		, FT_Get_Char_Index(lpFont->freetype.face,code),FT_KERNING_DEFAULT,&kerning2);
		result += kerning2.x / 64.;
	}

	return result;
}

/* god help us, this will have to be auto generated cuz I can't waste my time doing this */
int
rlFont_readNextLigature(char const *s, int *u) {
	*u = *s;
	/* todo: this should call the find or make glyph function directly, if the glyph
		wasn't found, the function should mark the glyph as not found, and we should
		provide a fallback */
	return 1;
}


/* I don't want to do things this way, we should have a cache buffer, which holds a maximum
	number of glyphs, when rendering, we add the glyphs to this buffer, and we store the
	draw quad, when we run out of space we flush the buffer */
void
lgi_drawText( lui_Draw_Config *lpConfig ) {
	lui_Font *lpFont = lpConfig->lpFont;

	if (lpConfig->char_height == 0.) {
		lpConfig->char_height = lpFont->char_height;
	}
	if (lpConfig->line_height == 0.) {
		lpConfig->line_height = lpFont->line_height;
	}

	float scale = lpConfig->char_height / lpFont->char_height;

	float line_height = lpConfig->line_height;

	float unwiden = 1. / (lpFont->is_subpixel ? 3. : 1.);

	unsigned char *colors = lpConfig->colors;

	char const *string = lpConfig->string;

	lgi_Color color = lpConfig->color;

	lgi_Color *color_table = lpConfig->color_table;


	lui_TextLine single_line;
	single_line.offset = 0;
	single_line.length = lpConfig->length;
	if(lpConfig->line_array == NULL) {
		lpConfig->line_array = & single_line;
		lpConfig->line_count = 1;
	}

	int mode = lgi_IM__PRESET_2D;
	if (lpFont->is_subpixel) {
		mode = lgi_IM__PRESET_LCD_TEXT;
	} else
	if (lpFont->is_sdf) {
		mode = lgi_IM__PRESET_SDF_TEXT;
	}
	lgi_IM__applyPipelinePreset(mode,FALSE);

	lui_TextLine *line_array = lpConfig->line_array;
	int line_count = lpConfig->line_count;

	float y = lpConfig->y;

	lui_TextLine *it;
	for (it = line_array; it < line_array + line_count; it += 1) {
		__int64 offset = it->offset;
		__int64 length = it->length;
		if (length < 0) {
			break;
		}
		if (offset < 0) {
			break;
		}
		if (offset + length > lpConfig->length) {
			break;
		}
		float x = lpConfig->x;

		for(int iii = 0; iii < length;) {
			/* todo */

			/* todo: this won't work properly with colors */
			int xchar = iii;

			int utf32;
			iii += rlFont_readNextLigature(string+(offset+iii),&utf32);
			lui_FontGlyph *glyph = lui__findOrMakeGlyphByUnicode(lpFont,utf32);
			if (glyph == NULL) {
				goto L_skip_rendering;
			}
			if E_IS_BLANK(utf32) {
				goto L_skip_rendering;
			}
			if (glyph->walking_x == 0.) {
				goto L_skip_rendering;
			}

			lui_GlyphCol *pallet = glyph->pallet;
			if (pallet == NULL) {
				continue;
			}

			/* you should do this whole thing first, checking for all the glyphs that need
			  to be added and then finally update the texture #todo #urgent */
			if (pallet->dirty) {
				pallet->dirty = FALSE;
				if(pallet->texture == NULL) {
					pallet->texture = lgi_uploadTextureContents(pallet->storage);
				} else {
					lgi_Texture__updateContents(pallet->texture,pallet->storage);
				}
			}

			if (pallet->texture == NULL) {
				continue;
			}

			lgi_flushAndBindTexture(rxPIPREG_kPS_TEX_0,pallet->texture,FALSE);

			if (color_table != NULL) {
				color = color_table[colors[it->offset + xchar]];
			}

			float x0 = x + glyph->offset_x * unwiden;
			float y0 = y + glyph->offset_y;
			x0 *= scale;
			y0 *= scale;
			float x1 = x0 + (glyph->x1 - glyph->x0) * scale * unwiden;
			float y1 = y0 + (glyph->y1 - glyph->y0) * scale;

			float xnor = 1. / pallet->texture->size_x;
			float ynor = 1. / pallet->texture->size_y;

			Emu_imp_begin(6,4);
			rx.imp.attr.rgba = color;
			rxaddnidx(6, 0,1,2, 0,2,3);
			rxaddnvtx(4,
			rxvtx_xyuv(x0,y0, glyph->x0 * xnor, glyph->y1 * ynor),
			rxvtx_xyuv(x0,y1, glyph->x0 * xnor, glyph->y0 * ynor),
			rxvtx_xyuv(x1,y1, glyph->x1 * xnor, glyph->y0 * ynor),
			rxvtx_xyuv(x1,y0, glyph->x1 * xnor, glyph->y1 * ynor));
			Emu_imp_end();

			L_skip_rendering:

			if (glyph == NULL || glyph->walking_x == 0.) {
				glyph = lui__findOrMakeGlyphByUnicode(lpFont,'_');
			}
			float walking_x = glyph->walking_x;

			if (utf32 == '\t') {
				walking_x *= 3;
			}

			x += walking_x * scale;
		}

		y -= line_height;
	}
}

#if 0
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
	 lgi_logError("failed to load file");
	 goto leave;
  }

  /* store this for the dynamically adding glyphs and such */
  stbtt_fontinfo fontInfo;
  if(stbtt_InitFont(&fontInfo,fileMemory,0) == 0) {
	 lgi_logError("failed to init font");
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
  glyphAtlas = lgi_GPU__createTextureSimply(atlasWidth,atlasHeight,lgi_Format_R8_UNORM);
  atlasMemory = rxGPU_borrow_texture(glyphAtlas);

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

  int rune;
  for ( rune  = glyphStart;
		  rune  < glyphEnd;    rune += 1 )
  {
	 /* we have to add the glyph even if the character is invisible #todo */
	 lui_FontGlyph *data = arradd(glyphArray,1);

	 int advanceWidthEm = 0;
	 int leftSideBearingEm = 0;
	 stbtt_GetCodepointHMetrics(&fontInfo,rune,&advanceWidthEm,&leftSideBearingEm);

	 float walking_x = advanceWidthEm * emToPixels;
	 float leftSideBearing = leftSideBearingEm * emToPixels;

	 int imageWidth = .5 + walking_x * wideningFactor;
	 int imageHeight = .5 + requestedHeight;
	 unsigned char *imageData = NULL;
	 int offset_x = 0;
	 int offset_y = 0;

	 /* check for all the other invisible characters too #todo,
		especially once the user is allowed to specify the range */
	 if(codepoint != ' ')
	 {
		if(0)
		{
		  imageData = stbtt_GetGlyphBitmapSubpixel(
			 &fontInfo,emToPixels * wideningFactor,emToPixels,-.5,-.5,
				stbtt_FindGlyphIndex(&fontInfo,codepoint),&imageWidth,&imageHeight,&offset_x,&offset_y);
		} else
		{
		  imageData = stbtt_GetGlyphSDF(&fontInfo,emToPixels * wideningFactor,emToPixels,
			 /* cache this #todo */
			 stbtt_FindGlyphIndex(&fontInfo,codepoint),
				FONT_SDF_CHAR_PADDING,FONT_SDF_ON_EDGE_VALUE,FONT_SDF_PIXEL_DIST_SCALE,
				  &imageWidth,&imageHeight,&offset_x,&offset_y);
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
	 data->walking_x = walking_x;
	 data->offset_x = offset_x;
	 data->offset_y = - offset_y - imageHeight ;
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