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

#ifndef _E_H
#define _E_H

# define _RX_STANDALONE
#include <rx/rx.c>

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4101)
#pragma warning(disable:4706) /* todo */
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#pragma warning(disable:4018)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#pragma warning(disable:4244)
#pragma warning(disable:4189)
#include <src/meow_hash_x64_aesni.h>

/* note: '/LIBPATH:<where>' tell the compiler where to find freetype */
#pragma comment(lib,"freetype")
#include <ft2build.h>
#include FT_FREETYPE_H

void Emu_imp_rect_sdf(rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include   <src/erect.c>
#include <src/esystem.c>
#include   <src/efile.c>
#include <src/ememory.c>
#include  <src/earray.c>
#include <src/estring.c>
#include <src/esyntax.h>
#include   <src/efont.h>
#include   <src/efont.c>
#include <src/ebuffer.h>
#include <src/ebuffer.c>
#include <src/eeditor.h>
#include <src/ewidget.h>
#include <src/esyntax.c>
#include <src/ewidget.c>
#include <src/eeditor.c>

void draw_single_text_line(rxFont *font, float x, float y, char const *string) {

	/* todo */
	int length = 0;
	while (*string != '\0' && *string != '\r' && *string != '\n') {
		length += 1;
		string += 1;
	}

	Emu_font_text_config_t config;
	ZeroMemory(&config,sizeof(config));
	config.font = font;
	config.x = x;
	config.y = y;
	config.char_height = font->char_height;
	config.line_height = font->line_height;
   config.tab_size = 2; /* in spaces */
	config.color = RX_RGBA_UNORM(8,36,36,255);
	config.color_table = NULL;
	config.length = length;
	config.string = string;
	config.colors = NULL;

	Emu_draw_text( &config );
}


#pragma warning(pop)

#endif