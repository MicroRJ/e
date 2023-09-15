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

/* trying this out, see if I like it #pending */
#define each_in(decl_type,decl_name,arr)\
(decl_type decl_name = arr; decl_name < ccarrend(arr); decl_name += 1)


ccinle void
Emu_atlas_glyph_init(
Emu_glyph_t *glyph,
Emu_glyph_pallet_t *pallet,
int external_index,
int utf32,
short x0,
short y0,
short x1,
short y1,
short offset_x,
short offset_y,
float walking_x )
{
	glyph->pallet = pallet;
	glyph->external_index = external_index;
	glyph->utf32 = utf32;
	glyph->x0 = x0;
	glyph->y0 = y0;
	glyph->x1 = x1;
	glyph->y1 = y1;
	glyph->offset_x = offset_x;
	glyph->offset_y = offset_y;
	glyph->walking_x = walking_x;
}

Emu_glyph_bucket_t *
Emu_search_or_create_glyph_bucket( int width, int height, int *x, int *y )
{
	Emu_glyph_bucket_t *result = NULL;

	/* try to find a better pallet based on the glyph's face #todo */
	for each_in(Emu_glyph_pallet_t **, pallet_, emu_Font_Library.pallets) {

		Emu_glyph_pallet_t *pallet = *pallet_;

		Emu_texture_memory_t storage = pallet->storage;

		if(storage.size_y < height) {
			continue;
		}

		if(storage.size_x < width) {
			continue;
		}

		/* search each bucket in the pallet */
		for each_in(Emu_glyph_bucket_t **, bucket_, pallet->buckets) {

			Emu_glyph_bucket_t *bucket = *bucket_;

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
		result = ccmalloc_T(Emu_glyph_bucket_t);

		if(result != NULL) {

			result->pallet = pallet;
			result->height = height;
			result->cursor_x = 0;
			result->cursor_y = pallet->cursor_y;

			*earray_add(pallet->buckets,1) = result;
		}

		pallet->cursor_y += height;

		if(result != NULL) {
			break;
		}
	}

	if(result == NULL)
	{
		Emu_glyph_pallet_t *pallet = ccmalloc_T(Emu_glyph_pallet_t);
		pallet->buckets = NULL;
		pallet->texture = NULL;
		pallet->storage = Emu_texture_memory_create(1024,1024,EMU_FORMAT_R8_UNORM);
		pallet->cursor_y = 0;

		*ccarradd(emu_Font_Library.pallets,1) = pallet;

		/* maybe sort the array #todo #pending */
		result = ccmalloc_T(Emu_glyph_bucket_t);
		result->pallet = pallet;
		result->height = height;
		result->cursor_x = 0;
		result->cursor_y = 0;

		pallet->cursor_y += height;

		*earray_add(pallet->buckets,1) = result;

		ccassert(result != NULL);
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

Emu_glyph_t *
Emu_search_or_create_glyph( rxFont *font, int utf32 )
{
	/* todo */
	for each_in(Emu_glyph_t **, it_, font->glyph_table) {

		Emu_glyph_t *it = *it_;

		if(it != NULL) {

			if(it->utf32 == utf32) {
				return it;
			}
		}
	}

	FT_Face face = font->freetype.face;

	int glyph_index = FT_Get_Char_Index(face,utf32);

	if(FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT)) {

		return NULL;
	}

	if(FT_Render_Glyph(face->glyph,FT_RENDER_MODE_LCD)) {

		return NULL;
	}

	FT_GlyphSlot glyph = face->glyph;

	int glyph_width = glyph->bitmap.width;
	int glyph_height = glyph->bitmap.rows;
	int glyph_stride = glyph->bitmap.pitch;
	unsigned char *glyph_memory = glyph->bitmap.buffer;

	Emu_glyph_bucket_t *bucket = NULL;

	int dest_x = 0;
	int dest_y = 0;

	Emu_glyph_pallet_t *pallet = NULL;

	if(glyph_memory != NULL)
	{
		bucket = Emu_search_or_create_glyph_bucket(glyph_width,glyph_height,&dest_x,&dest_y);

		if(bucket != NULL)
		{
			pallet = bucket->pallet;

			bucket->cursor_x += glyph_width;

			Emu_memcpy2d_config_t cpy_cfg;
			cpy_cfg.dst.offset_x = dest_x;
			cpy_cfg.dst.offset_y = dest_y;
			cpy_cfg.dst.height   = pallet->storage.size_y;
			cpy_cfg.dst.stride   = pallet->storage.stride;
			cpy_cfg.dst.cursor   = pallet->storage.memory;

			cpy_cfg.src.offset_x = 0;
			cpy_cfg.src.offset_y = 0;
			cpy_cfg.src.height = glyph_height;
			cpy_cfg.src.stride = glyph_stride;
			cpy_cfg.src.cursor = glyph_memory;

			Emu_memcpy2d(&cpy_cfg);

			pallet->dirty = TRUE;
		}
	}

	Emu_glyph_t *result = ccmalloc_T(Emu_glyph_t);

	if(result != NULL) {

		Emu_atlas_glyph_init(
		result,pallet,glyph_index,utf32,
		dest_x,dest_y,dest_x+glyph_width,dest_y+glyph_height,
		glyph->bitmap_left,glyph->bitmap_top-glyph_height,
		glyph->advance.x / 64. );

		Emu_glyph_t **result_ = ccarradd(font->glyph_table,1);

		if(result_ != NULL) {

			*result_ = result;
		}
	}

	return result;
}


float
EMU_font_get_kerning(
rxFont *font, char prev_char, char curr_char )
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
Emu_draw_text( Emu_font_text_config_t *config )
{
	rxFont *font = config->font;

	/* setup emu::imp */
	int mode = EMU_IMP_MODE_2D;

	if (font->is_subpixel) {
		mode = EMU_IMP_MODE_LCD_TEXT;
	} else
	if (font->is_sdf) {
		mode = EMU_IMP_MODE_SDF_TEXT;
	}

	float scale = config->char_height / font->char_height;

	float line_height = config->line_height;

	float unwiden = 1. / (font->is_subpixel ? 3. : 1.);

	unsigned char *colors = config->colors;

	char const *string = config->string;

	int length = config->length;

	rxcolor_t color = config->color;

	rxcolor_t *color_table = config->color_table;

	float y = config->y;

	Emu_imp_apply(mode,FALSE);

	Emu_text_line_t single_line;
	single_line.offset = 0;
	single_line.length = config->length;
	if(config->line_array == NULL) {
		config->line_array = & single_line;
		config->line_count = 1;
	}

	Emu_text_line_t *it;
	for ( it = config->line_array;
	it < config->line_array + config->line_count; it += 1 )
	{
		if(it->offset + it->length > config->length) {
			continue;
		}

		float x = config->x;

		for(int xchar = 0; xchar < it->length; xchar += 1)
		{
			/* todo */
			int utf32 = string[it->offset + xchar];

			Emu_glyph_t *glyph = Emu_search_or_create_glyph(font,utf32);

			if (glyph == NULL) {
				continue;
			}

			if IS_WHITESPACE(utf32) {
				goto skip_rendering;
			}

			Emu_glyph_pallet_t *pallet = glyph->pallet;

			if (pallet == NULL) {
				continue;
			}

			/* you should do this whole thing first, checking for all the glyphs that need
			  to be added and then finally update the texture #todo #urgent */
			if (pallet->dirty) {
				pallet->dirty = FALSE;

				if(pallet->texture == NULL) {
					pallet->texture = Emu_texture_memory_upload(pallet->storage);
				} else {
					Emu_texture_update(pallet->texture,pallet->storage);
				}
			}

			if (pallet->texture == NULL) {
				continue;
			}

			rxpipset_texture(REG_PS_TEX_0,pallet->texture,FALSE);

			if (color_table != NULL) {
				color = color_table[colors[it->offset + xchar]];
			}

			float x0 = x + glyph->offset_x * unwiden;
			float y0 = y + glyph->offset_y;

		// if (i != 0) {
		//   x0 += EMU_font_get_kerning( &font, config->string[i-1], chr );
		// }

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

			skip_rendering:

			float walking_x = glyph->walking_x;
			if (utf32 == '\t') {
				walking_x *= 3;
			}

			x += walking_x * scale;
		}

		y -= line_height;
	}
}

rxFont *
rxLoadFontFile(
char const *fpath, float height)
{

	FT_Library library_ft;
	if(FT_Init_FreeType(&library_ft)) {
		cctraceerr("failed to init FreeType");
	}

	FT_Face face_ft;
	if(FT_New_Face(library_ft,fpath,0,&face_ft)) {
		cctraceerr("failed to load file path");
	}

	if(FT_Set_Pixel_Sizes(face_ft,0,height)) {
		cctraceerr("invalid pixel size");
	}

	/* 16.16 to 24.6 to 32. */
	float units_to_pixels = face_ft->size->metrics.y_scale / 65536. / 64.;
	float lineGap = face_ft->height - (face_ft->ascender - face_ft->descender);

	rxFont *font = ccmalloc_T(rxFont);

	font->is_subpixel = TRUE;
	font->is_sdf = FALSE;
	font->fpath = fpath;
	font->glyph_table = NULL;
	font->ascent = face_ft->ascender * units_to_pixels;
	font->descent = face_ft->descender * units_to_pixels;
	font->lineGap = lineGap * units_to_pixels;
	font->line_height = height;
	font->char_height = height;
	font->freetype.face = face_ft;

	return font;
}

/* todo: support for kerning */
ccinle float
efont_code_xadv(
rxFont *font, int code)
{
	// return 0;
	Emu_glyph_t *glyph = Emu_search_or_create_glyph(font,code);

	if(glyph != NULL) {

		return glyph->walking_x;
	}

	return 0;
}

ccinle float
efont_code_width(
rxFont *font, int code)
{
	// return 0;
	Emu_glyph_t *glyph = Emu_search_or_create_glyph(font,code);

	if(glyph != NULL) {

		return glyph->walking_x;
	}

	return 0;
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

  int rune;
  for ( rune  = glyphStart;
		  rune  < glyphEnd;    rune += 1 )
  {
	 /* we have to add the glyph even if the character is invisible #todo */
	 Emu_glyph_t *data = earray_add(glyphArray,1);

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