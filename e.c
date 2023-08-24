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
# define _RX_STANDALONE
# define _RX_NO_TRUNCATION_WARNING
# define _RX_NO_CONVERSION_WARNING
# define _RX_DEFAULT_WINDOW_SIZE_X 1080 + 256
# define _RX_DEFAULT_WINDOW_SIZE_Y 720
# define _RX_REFRESH_RATE          30
#include "rx\\rx.c"

#pragma warning(disable:4324)
#include "meow_hash_x64_aesni.h"

#pragma comment(lib,"freetype")
#include <ft2build.h>
#include FT_FREETYPE_H

void
EMU_imp_rect_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include   "erect.c"
#include "esystem.c"
#include   "efile.c"
#include "ememory.c"
#include  "earray.c"
#include "estring.c"
#include "esyntax.h"
#include "ebuffer.h"
#include "ebuffer.c"
#include  "emem2d.h"
#include   "efont.h"
efont default_font;/* todo */
#include "ealloc2.c"
#include   "efont.c"
#include "ewidget.h"
#include "eeditor.h"
#include "esyntax.c"
#include "ewidget.c"
#include "eeditor.c"
#include "econfig.c"

unsigned int RAND_seed = 5735;

unsigned int
xorshift32(unsigned int *p)
{
  unsigned int q = *p;
  q = q ^ q << 13;
  q = q ^ q >> 17;
  q = q ^ q << 5;
  return *p=q;
}

int
randi(
  int min, int max)
{
	int off = max - min;

	return off <= 0 ? 0 : min + xorshift32(&RAND_seed) % off;
}

int main(int argc, char **argv)
{
  rxinit(L"e");

  default_font = EMU_load_glyph_font( "assets\\Hack\\Hack_v3_003\\Hack-Regular.ttf", 22);

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));
  eaddcur(&editor,(ecursor_t){0,0});
  editor.font = default_font;
  editor.text_size = default_font.height;
  editor.line_height = default_font.height * 1.4;

  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  Emu_texture_t *glyphTexture = editor.font.glyphAtlas;

  int debug_overlay = 0;

  do
  {

#if 1
    if(rxtstkey(rx_kKEY_F5))
    {
      RunCommandString("build.msvc.template.bat e TRUE");
    } else
    if(rxtstkey(rx_kKEY_F1))
    {
      eeditor_unload(&editor,editor.buffer.tag);
    } else
    if(rxtstkey(rx_kKEY_F7))
    { debug_overlay = !debug_overlay;
    } else
    if(rxtstkey(rx_kKEY_F9))
    {
    } else
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_UP))
    {
      editor.text_size += 2;
    } else
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_DOWN))
    {
      editor.text_size -= 2;
    } else
    if(rxisctrl() && rxtstkey('P'))
    { // cmd.widget.focused = !cmd.widget.focused;
      // editor.widget.focused = !cmd.widget.focused;
    } else
    if(rxisctrl() && rxtstkey('O'))
    { eeditor_load(&editor,fdlg());
    } else
    if(rxisctrl() && rxtstkey('S'))
    { eeditor_unload(&editor,fdlg());
    } else
    {
    	erect_t r = get_window_client_rect();

      /* draw the background, should be a clear command instead #todo */
      draw_rect(r,RX_RGBA_UNORM(8,36,36,255));

      if(debug_overlay)
      {
        EMU_imp_rect_uv(RX_COLOR_WHITE,glyphTexture,rx.point_sampler,
          0,0,glyphTexture->size_x,glyphTexture->size_y);

      } else
      {

	      if(ewdg(r,&editor))
	      {
	      }

	      for(int i=0; i<1; i+=1)
	      {
	        erect_t f = rect_cut(&r,RECT_kBOT,22.);
	        draw_rect(f,RX_RGBA_UNORM(122,104,81,255));

	        EMU_draw_text_config_t config =
					draw_text_config_init(editor.font,22.,
						f.x0,f.y0,RX_RGBA_UNORM(8,36,36,255),NULL,NULL,-1,NULL);

					config.string = ccformat(
						"%i,%i %s (%i/%i) %s @%f",
						egetcurx(&editor,0),egetcury(&editor,0),
	            editor.buffer.tag,editor.buffer.length,editor.buffer.extent,ccfnames(editor.font.filePath),
	            	editor.text_size);

	        edraw_text(&config);
	      }
      }

    }
#endif

    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}


