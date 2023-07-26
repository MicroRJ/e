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

#include "vendor\\ccures\\ccread.h"
#include "vendor\\ccures\\ccrune.c"
#include "vendor\\ccures\\cctree.h"
#include "vendor\\ccures\\ccread.c"

const char *fdlg();

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

#include "ebuffer.cc"
#include       "e.h"
#include    "ewdg.cc"
#include    "ecmd.cc"
#include     "eos.cc"

void
eload(
  ewdg_t *widget, char const *name);
void
eunload(
  ewdg_t *widget, char const *name);



int main(int argc, char **argv)
{
  rxinit(L"e");

  ewdg_t editor;
  ZeroMemory(&editor,sizeof(editor));
  ccread_init(&editor.reader);
  eload(&editor,"todo.txt");
  editor.focused = TRUE;

  ewdg_t cmd;
  ZeroMemory(&cmd,sizeof(cmd));
  cmd.focused = FALSE;

  do
  {
    rx2d();

    if(rxtstkey(rx_kKEY_F1))
    {
      RunCommandString("build.msvc.bat");
    } else
    if(rxisctrl() && rxtstkey('P'))
    { cmd.focused = !cmd.focused;
      editor.focused = !cmd.focused;
    } else
    if(rxisctrl() && rxtstkey('O'))
    { eload(&editor,fdlg());
    } else
    if(rxisctrl() && rxtstkey('S'))
    { eunload(&editor,fdlg());
    } else
    {
      rxrect_t r = get_window_client_rect();

      draw_rect(r,RX_RGBA_UNORM(19, 7, 13,255));

      draw_text(rect_cut(&r,RECT_kTOP,16),ccformat("%i,%i %s (%i/%i)",
        editor.cursor.xchar,
        editor.cursor.yline,
          editor.buffer.tag,
          editor.buffer.length,
          editor.buffer.extent),16,RX_COLOR_WHITE);

      if(cmd.focused)
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
                copy_string_ex(
                  buffer+ccCstrlenL("/o "),
                  length-ccCstrlenL("/o "));

              eload(&editor,string);

              ccfree(string);
            }

            if(!strncmp(buffer,"/s -y",ccCstrlenL("/s -y")))
            {
              eunload(&editor,editor.buffer.tag);
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



/* editor widget */
void
eload(
  ewdg_t *widget, char const *name)
{
  if(name != 0 && strlen(name) != 0)
  {
    void *file = ccopenfile(name,"r");
    ccu32_t length = 0;
    void *memory = ccpullfile(file,0,&length);

    if(memory != 0)
    {
      ebuffer_uninit(&widget->buffer);
      ebuffer_init(&widget->buffer,name,length);

      memcpy(widget->buffer.memory,memory,length);

      erecache(widget);
    }
  }
}

void
eunload(
  ewdg_t *widget, char const *name)
{
  if(name != 0 && strlen(name) != 0)
  {
    void *file = ccopenfile(name,"w");

    ccpushfile(file,0,
      widget->buffer.length,
      widget->buffer.memory);

    ccclosefile(file);
  }
}



# pragma comment(lib,"Comdlg32")
#include   "commdlg.h"

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
