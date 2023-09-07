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
Emu_imp_rect_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include   "erect.c"
#include "esystem.c"
#include   "efile.c"
#include "ememory.c"
#include  "earray.c"
#include "estring.c"
#include "esyntax.h"
#include   "efont.h"
#include   "efont.c"
#include "ebuffer.h"
#include "ebuffer.c"
#include "eeditor.h"
#include "ewidget.h"
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

void draw_single_text_line(
   Emu_glyph_font_t *font, float x, float y, char const *string)
{
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
   config.length = strlen(string);
   config.string = string;
   config.colors = NULL;

   Emu_draw_text( &config );
}

int main(int argc, char **argv)
{
  rxinit(L"e");

  Emu_glyph_font_t *font = Emu_load_font( "assets\\Hack\\Hack_v3_003\\Hack-Regular.ttf", 22);

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));

  eaddcur(&editor,(ecursor_t){0,0});
  editor.font = font;

  eeditor_load(&editor,"todo.txt");

  // Emu_texture_t *glyphTexture = editor.font.glyphAtlas;

  int debug_overlay = 0;
  int searching = 0;

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
    } else
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_DOWN))
    {
    } else
    if(rxisctrl() && rxtstkey('P'))
    {
    	searching = !searching;
    } else
    if(rxisctrl() && rxtstkey('O'))
    { eeditor_load(&editor,fdlg());
    } else
    if(rxisctrl() && rxtstkey('S'))
    { eeditor_unload(&editor,fdlg());
    } else
    {
      Emu_rect_t r = get_window_client_rect();
      draw_rect(r,RX_RGBA_UNORM(8,36,36,255));

      if(Emu_widget_render(&editor,r))
      {
      }

      for(int i=0; i<1; i+=1)
      {
        Emu_rect_t f = rect_cut(&r,RECT_kBOT,22.);
        draw_rect(f,RX_RGBA_UNORM(122,104,81,255));

         draw_single_text_line(editor.font,f.x0,f.y0,
            ccformat("%i,%i %s (%i/%i) %s @%f %f",
               egetcurx(&editor,0),egetcury(&editor,0),
                  editor.buffer.tag,
                  editor.buffer.length,
                  editor.buffer.extent,
                  ccfnames(editor.font->fpath),editor.font->char_height,1./rx.delta_seconds));
      }
    }
#endif

    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}


