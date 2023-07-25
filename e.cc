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
# define _RX_DEFAULT_WINDOW_SIZE_X 1024
# define _RX_DEFAULT_WINDOW_SIZE_Y 512
#include "rx\\rx.c"

#include      "e.h"

int main(int argc, char **argv)
{
  rxinit(L"e");

  ccglobal ewdg_t editor;
  ZeroMemory(&editor,sizeof(editor));

  eload(&editor,"init.txt");

  editor.focused = TRUE;

  do
  {
    rx2d();

    /* todo */
    if(ewdg(get_window_client_rect(),&editor))
    {
    }

  } while(rxtick());

  return 0;
}
