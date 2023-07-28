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
#include "rx\\rx.c"

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

int main(int argc, char **argv)
{
  rxinit(L"e");

  eeditor_t editor;
  ZeroMemory(&editor,sizeof(editor));
  esyntax_init(&editor.syntax);
  eaddcur(&editor);

  eeditor_load(&editor,"todo.txt");
  editor.widget.focused = TRUE;

  eeditor_t cmd;
  ZeroMemory(&cmd,sizeof(cmd));
  eaddcur(&cmd);
  cmd.widget.focused = FALSE;

  do
  {
    rx2d();

    if(rxtstkey(rx_kKEY_F5))
    {
      RunCommandString("build.msvc.template.bat e TRUE");
    } else
    if(rxtstkey(rx_kKEY_F1))
    {
      eeditor_unload(&editor,editor.buffer.tag);
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

      draw_rect(r,RX_RGBA_UNORM(19, 7, 13,255));

      draw_text(rect_cut(&r,RECT_kTOP,16),ccformat("%i,%i %s (%i/%i)",
        editor.cursor[0].xchar,
        editor.cursor[0].yline,
          editor.buffer.tag,
          editor.buffer.length,
          editor.buffer.extent),16,RX_COLOR_WHITE);

      if(cmd.widget.focused)
      {
        if(ecmd(rect_cut(&r,RECT_kBOT,32),&cmd))
        {
          char *buffer = cmd.buffer.memory;
          int   length = cmd.buffer.length;

          if(length != 0)
          {
            if(!strncmp(buffer,"/o ",ccCstrlenL("/o ")))
            {
              char *string =
                estring_copy_ex(
                  buffer+ccCstrlenL("/o "),
                  length-ccCstrlenL("/o "));

              eeditor_load(&editor,string);

              ccfree(string);
            }

            if(!strncmp(buffer,"/s -y",ccCstrlenL("/s -y")))
            {
              eeditor_unload(&editor,editor.buffer.tag);
            }

            if(!strncmp(buffer,"/b",ccCstrlenL("/b")))
            {
              RunCommandString("build.msvc.bat");
            }
          }
        }
      }

      if(ewdg(r,&editor))
      {
      }
    }

  } while(rxtick());

  return 0;
}
