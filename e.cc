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

FONScontext* fsContext;

void* userPtr;
int __renderCreate(void* uptr, int width, int height)
{
  return 1;
}
int __renderResize(void* uptr, int width, int height)
{
  return 1;
}
void __renderUpdate(void* uptr, int* rect, const unsigned char* data)
{
  void const *memory = fonsGetTextureData(fsContext,NULL,NULL);

  rxborrowed_t borrow = rxtexture_borrow(atlas);
  memcpy(borrow.memory,(void*)memory,atlas.size_x*atlas.size_y);
  rxreturn(borrow);
}

void __renderDraw(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
{
}
void __renderDelete(void* uptr)
{
}

int main(int argc, char **argv)
{
  rxinit(L"e");

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));
  esyntax_init(&editor.syntax);
  eaddcur(&editor,(ecursor_t){0,0});
  editor.font = efont_load("C:\\Windows\\Fonts\\cascadiamono.ttf",64);
  editor.text_size = 64;

  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  int debug_overlay = FALSE;

  FONSparams params;
  ZeroMemory(&params,sizeof(params));
  params.renderCreate = __renderCreate;
  params.renderResize = __renderResize;
  params.renderUpdate = __renderUpdate;
  params.renderDraw = __renderDraw;
  params.renderDelete = __renderDelete;
  params.width = 512;
  params.height = 512;
  params.flags = FONS_ZERO_BOTTOMLEFT;

  atlas = rxtexture_create(params.width,params.height,rxRGB8);

  fsContext = fonsCreateInternal(&params);
  if (!fsContext)
  {
      // Handle initialization error
  }

  int fontNormal = fonsAddFont(fsContext,"cascadia_mono",editor.font.fpath);
  if (fontNormal == FONS_INVALID)
  {
    // Handle font loading error
    ccbreak();
  }

  fonsClearState(fsContext);
  fonsSetFont(fsContext,fontNormal);
  fonsSetSize(fsContext,64);
  // fonsSetBlur(fsContext, 10.0f);


  // fonsDrawText(fsContext,0,0,"the motherfucking bitch in this house get the fuck out, moma's dead and the fucking next",NULL);




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
        // draw_rect(
        //   erect_xywh(0,0,
        //     atlas.size_x,
        //     atlas.size_y),
        //       RX_COLOR_BLACK);
        // rximp_apply();
        // rxpipset_texture(REG_PS_TEX_0,atlas);
        // rxpipset_sampler(REG_PS_SAM_0,rx.point_sampler);
        // rxadd_rec4_col(RX_COLOR_WHITE,
        //   0,0,
        //   atlas.size_x,
        //   atlas.size_y, 0,0,1,1);

        rximp_apply();
          draw_text2(fsContext,0,0,strlen("Hello, Sailor!"),"Hello, Sailor!");
      }

    }
#endif

    /* todo: this is something that rx should do intrinsically */
    Sleep(8);

  } while(rxtick());

  return 0;
}


