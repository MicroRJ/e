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

void
draw_box_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include   "erect.c"
#include "esystem.c"
#include "ememory.c"
#include  "earray.c"
#include "estring.c"
#include "esyntax.h"
#include "ebuffer.h"
#include "ebuffer.c"
#include   "efont.h"
#include   "efont.c"
#include "ewidget.h"
#include "eeditor.h"
#include "esyntax.c"
#include "ewidget.c"
#include "eeditor.c"
#include "econfig.c"

int main(int argc, char **argv)
{
  rxinit(L"e");

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));

  eaddcur(&editor,(ecursor_t){0,0});
  editor.font = Load_Glyph_Font("assets\\Hack\\Hack_v3_003\\Hack-BoldItalic.ttf",32.);
  editor.text_size = 32;
  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  rxtexture_t glyphTexture = editor.font.glyphAtlas;

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
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_UP))
    {
      efont new_font = efont_load(editor.font.filePath,editor.font.height+2);
      efont_free(editor.font);
      editor.font = new_font;
      editor.text_size = editor.font.height;
    } else
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_DOWN))
    {
      efont new_font = efont_load(editor.font.filePath,editor.font.height-2);
      efont_free(editor.font);
      editor.font = new_font;
      editor.text_size = editor.font.height;
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

      /* draw the background */
      draw_rect(r,RX_RGBA_UNORM(8,36,36,255));

      if(ewdg(r,&editor))
      {
      }

      for(int i=0; i<1; i+=1)
      {
        erect_t f = rect_cut(&r,RECT_kBOT,editor.font.height);
        draw_rect(f, RX_RGBA_UNORM(122,104,81,255));

        edraw_text_config_t config =
				draw_text_config_init(editor.font,editor.text_size,
					f.x0,f.y0,RX_RGBA_UNORM(8,36,36,255),NULL,NULL,-1,NULL);

				config.string = ccformat("%i,%i %s (%i/%i) %s",
					egetcurx(&editor,0),egetcury(&editor,0),
            editor.buffer.tag,editor.buffer.length,editor.buffer.extent,ccfnames(editor.font.filePath));

        edraw_text(&config);
      }

      if(debug_overlay)
      {
        ccglobal float zoom = 100.;

        if(rxtstkey(rx_kKEY_UP))
        {
          zoom += 4;
        } else
        if(rxtstkey(rx_kKEY_DOWN))
        {
          zoom -= 4;
        }

        float zzz = zoom / 100.;

        draw_rect(
          erect_xywh(0,0,
            glyphTexture.size_x * zzz,
            glyphTexture.size_y * zzz),RX_COLOR_BLACK);

        rximp_apply();
        rxpipset_program(rx.imp.sha_vtx,rx.imp.sha_pxl);
        rxpipset_texture(REG_PS_TEX_0,glyphTexture);
        rxpipset_sampler(REG_PS_SAM_0,rx.point_sampler);
        rxadd_rec4_col(RX_COLOR_WHITE,
          0,0,glyphTexture.size_x * zzz,
              glyphTexture.size_y * zzz, 0,0,1,1);
      }

    }
#endif

    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}


