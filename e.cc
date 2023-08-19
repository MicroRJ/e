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
# define _RX_DEFAULT_WINDOW_SIZE_X 1080
# define _RX_DEFAULT_WINDOW_SIZE_Y 720
# define _RX_REFRESH_RATE          30
#include "rx\\rx.c"

#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"

void
draw_box_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include   "erect.c"
#include "esystem.c"
#include "estring.c"
#include "ebuffer.c"
#include  "earray.c"
#include   "efont.c"
#include "esyntax.h"
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
  esyntax_init(&editor.syntax);
  eaddcur(&editor,(ecursor_t){0,0});
  editor.font = efont_load("assets\\DroidSerif-Italic.ttf",64);
  editor.text_size = 64;
  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  Glyph_Font glyphFont = Load_Glyph_Font(editor.font.fpath,128);
  Glyph_Data glyphData = glyphFont.glyphData['A'-32];
  rxtexture_t glyphTexture =
    rxtexture_create_ex(glyphData.imageWidth,glyphData.imageHeight,rxRGB8,
      glyphData.imageWidth,glyphData.imageData);


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
      efont new_font = efont_load(editor.font.fpath,editor.font.vsize+2);
      efont_free(editor.font);
      editor.font = new_font;
    } else
    if(rxisctrl() && rxismenu() && rxisshft() && rxtstkey(rx_kKEY_DOWN))
    {
      efont new_font = efont_load(editor.font.fpath,editor.font.vsize-2);
      efont_free(editor.font);
      editor.font = new_font;
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
        erect_t f = rect_cut(&r,RECT_kBOT,editor.font.vsize);
        draw_rect(f, RX_RGBA_UNORM(122,104,81,255));
        edraw_text(editor.font,32.,RX_RGBA_UNORM(8,36,36,255),
          f.x0,f.y0,-1,
          ccformat("%i,%i (%f)px %s (%i/%i)",
            egetcurx(&editor,0),
            egetcury(&editor,0), egetinf(&editor,0),
              editor.buffer.tag,
                editor.buffer.length,
                editor.buffer.extent));
      }

      if(debug_overlay)
      {
        int zoom = 2;
        draw_rect(
          erect_xywh(0,0,
            glyphTexture.size_x * zoom,
            glyphTexture.size_y * zoom),RX_COLOR_BLACK);

        rximp_apply();
        rxpipset_program(rx.imp.sha_vtx,rx.imp.sha_pxl_txt);
        rxpipset_texture(REG_PS_TEX_0,glyphTexture);
        rxpipset_sampler(REG_PS_SAM_0,rx.linear_sampler);
        rxadd_rec4_col(RX_COLOR_WHITE,
          0,0,glyphTexture.size_x * zoom,
              glyphTexture.size_y * zoom, 0,0,1,1);
      }

    }
#endif

    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}


