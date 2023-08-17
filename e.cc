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

void
draw_box_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness );

#include "esystem.c"
#include "estring.c"
#include "ebuffer.c"
#include "esyntax.h"
#include "ewidget.h"
#include "eeditor.h"
#include "esyntax.c"
#include "ewidget.c"
#include "eeditor.c"
#include    "ecmd.cc"
#include "econfig.c"
#include "eshader.c"

typedef struct efont
{
  rxtexture_t      atlas;
  stbtt_bakedchar *array;

  float            vsize;
} efont;

efont
load_font();
void
draw_text_font(
  efont font, float h, float x, float y, char const *string);

int main(int argc, char **argv)
{
  rxinit(L"e");

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));
  esyntax_init(&editor.syntax);
  eaddcur(&editor,(ecursor_t){0,0});

  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  eeditor_t cmd;
  ZeroMemory(&cmd,sizeof(cmd));
  eaddcur(&cmd,(ecursor_t){0,0});
  cmd.widget.focused = FALSE;

  efont font = load_font();

  eshader_init();

  do
  {
    if(rxtstkey(rx_kKEY_F5))
    {
      RunCommandString("build.msvc.template.bat e TRUE");
    } else
    if(rxtstkey(rx_kKEY_F1))
    {
      eeditor_unload(&editor,editor.buffer.tag);
    } else
    if(rxtstkey(rx_kKEY_F7))
    { regset_texture(REG_PS_TEX_0,font.atlas);
      regset_sampler(REG_PS_SAM_0,rx.point_sampler);
      rxadd_rec4_col(RX_COLOR_WHITE,
        0,0,
        font.atlas.size_x,
        font.atlas.size_y, 0,0,1,1);
    } else
    if(rxisctrl() && rxtstkey('P'))
    { cmd.widget.focused = !cmd.widget.focused;
      editor.widget.focused = !cmd.widget.focused;
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
      draw_rect(r,RX_RGBA_UNORM(19, 7, 13,255));

      for(int i=0; i<1; i+=1)
      {
        erect_t f = rect_cut(&r,RECT_kTOP,16);
        draw_text_font(font,16.,f.x0,f.y0,
          ccformat("%i,%i %s (%i/%i)",
            editor.cursor[0].xchar,
            editor.cursor[0].yline,
              editor.buffer.tag,
                editor.buffer.length,
                editor.buffer.extent));
      }


      if(ewdg(r,&editor))
      {
      }


    }
    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}




efont
load_font()
{
  void *file = ccopenfile("C:\\Windows\\Fonts\\CascadiaMono.ttf","r");

  ccu32_t length = 0;
  unsigned char *memory = ccpullfile(file,0,&length);

  rxtexture_t atlas = rxtexture_create(256,256,rxRGB8);
  rxborrowed_t borrow = rxtexture_borrow(atlas);

  float vsize = 16.;

  stbtt_bakedchar *array = ccmalloc(sizeof(stbtt_bakedchar) * 0x80);
  stbtt_BakeFontBitmap(memory,0,vsize,borrow.memory,atlas.size_x,atlas.size_y,32,96,array);

  ccclosefile(file);
  rxreturn(borrow);
  ccfree(memory);

  efont font;
  font.vsize = vsize;
  font.atlas = atlas;
  font.array = array;

  return font;
}


/* Here I would like to pass in some options, for instance, how should the baseline be used */
void
draw_text_font(
  efont font, float h, float x, float y, char const *string)
{
  int length = strlen(string);

  regset_texture(REG_PS_TEX_0,font.atlas);
  regset_sampler(REG_PS_SAM_0,rx.linear_sampler);

  for(int i=0;i<length;i+=1)
  {
    int code = *string ++;

    stbtt_bakedchar c = font.array[code-32];

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(font.array,
      font.atlas.size_x,
      font.atlas.size_y,
      code - 32,&x,&y,&q,1);

    float xxx = q.y1 - q.y0;

    q.y0 -= 2*c.yoff + xxx;
    q.y1 -= 2*c.yoff + xxx;

    q.x1 = q.x0 + (q.x1 - q.x0) * (h / font.vsize);
    q.y1 = q.y0 + (q.y1 - q.y0) * (h / font.vsize);

    x -= c.xadvance;
    x += c.xadvance * h / font.vsize;

    rxbgn();
      rx.imp.attr.rgba = RX_COLOR_WHITE;
      rxaddvtx(rxvtx_xyuv(q.x0,q.y0,q.s0,q.t1));
      rxaddvtx(rxvtx_xyuv(q.x0,q.y1,q.s0,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y1,q.s1,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y0,q.s1,q.t1));
      rxaddnidx(6,0,1,2,0,2,3);
    rxend();

  }

}
