/*
**
**                     -+- sonoro -+-
**
**                   A Sonic Experience
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**
**                    [ MEMENTO MORI ]
**
**
**               github.com/MicroRJ/sonoro
**
**
**                  USE AT YOUR OWN RISK
**
*/
# define _RX_STANDALONE
# define _RX_NO_TRUNCATION_WARNING
# define _RX_NO_CONVERSION_WARNING
# define _RX_DEFAULT_WINDOW_SIZE_X 1024
# define _RX_DEFAULT_WINDOW_SIZE_Y 512
#include "rx\\rx.c"

/* networking stuff */
# pragma comment(lib,"Comdlg32")
#include   "commdlg.h"

#include      "rect.cc"
#include     "mouse.cc"
#include    "widget.cc"

char *
copy_string_ex(
  const char *the_string, unsigned int the_length)
{
  if((the_string == 0) ||
     (the_length == 0)) return ccnull;

  char *the_result = ccmalloc(the_length+1);
  memcpy(the_result,the_string,the_length+1);
  the_result[the_length] = 0;

  return the_result;
}

char *
copy_string(
  const char *the_string)
{
  if((the_string == 0)) return ccnull;

  return copy_string_ex(the_string,strlen(the_string));
}

const char *fdlg()
{
  char DirBuff[MAX_PATH];
  GetCurrentDirectory(sizeof(DirBuff),DirBuff);

  OPENFILENAMEA OpenFileName;
  ZeroMemory(&OpenFileName,sizeof(OpenFileName));
  OpenFileName.lStructSize     = sizeof(OpenFileName);
  OpenFileName.hwndOwner       = NULL;
  OpenFileName.lpstrFile       = ccstatic_alloc(MAX_PATH,TRUE);
  OpenFileName.nMaxFile        = MAX_PATH;
  OpenFileName.lpstrFilter     = "All\0*.*\0Text\0*.TXT\0";
  OpenFileName.nFilterIndex    = 1;
  OpenFileName.lpstrFileTitle  = NULL;
  OpenFileName.nMaxFileTitle   = 0;
  OpenFileName.lpstrInitialDir = NULL;
  OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  GetOpenFileNameA(&OpenFileName);

  SetCurrentDirectory(DirBuff);

  return OpenFileName.lpstrFile;
}

int main(int argc, char **argv)
{
  rxinit(L"Terra");

  ccglobal textbox_widget_t editor;
  ZeroMemory(&editor,sizeof(editor));

  fload(&editor,"light.hlsl");

  movcur(&editor,0,0);

  editor.focused = TRUE;

  do
  {
    /* todo */
    if(rx.is_ctrl && rxtstkey('O'))
    {
      editor.focused = FALSE;

      fload(&editor,fdlg());
    }

    if(rx.is_ctrl && rxtstkey('S'))
    {
      editor.focused = FALSE;

      fsave(&editor,fdlg());
    }


    mouse_poll();

    rx2d();

    rect_t r = get_window_client_rect();

    rect_t s = rect_cut(&r,RECT_kTOP,16);
    draw_text(s,ccformat("%i,%i",editor.cursor.xchar,editor.cursor.yline),16,RX_COLOR_WHITE);

    if(textbox_widget(r,&editor))
    {
    }

    editor.focused = TRUE;

  } while(rxtick());

  return 0;
}
