/*
**
**                     -+- terra -+-
**
**                   A Game About Life
**
**                    [ MEMENTO MORI ]
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**                github.com/MicroRJ/terra
**
**
**                  USE AT YOUR OWN RISK
**
*/

/* This is all just a big todo - XXXX - the one called rj */

#define MOUSE rx.mice.array[0]

#ifndef WAS_DOWN
#define WAS_DOWN(x) ((MOUSE.btn_last & (1 << x)) != 0)
# endif
#ifndef  IS_DOWN
#define  IS_DOWN(x) ((MOUSE.btn      & (1 << x)) != 0)
# endif

int mouse_is_key(int x)
{
  return IS_DOWN(x);
}

int rxtstkey(int x)
{
  return rx.key[x];
}

int rxchr()
{
	return rx.chr;
}

int is_click_leave(int x)
{ return !IS_DOWN(x) &&  WAS_DOWN(x); }

int is_click_enter(int x)
{ return  IS_DOWN(x) && !WAS_DOWN(x); }

int cursor_in_rect(rect_t rect)
{
  return rect_in_xy(rect,rx.xcursor,rx.ycursor);
}

int is_click_leave_rect(rect_t rect)
{
  return is_click_leave(0) && cursor_in_rect(rect);
}

int is_click_enter_rect(rect_t rect)
{
  return is_click_enter(0) && cursor_in_rect(rect);
}

int mouse_poll()
{
  MOUSE.btn_last = MOUSE.btn;
  MOUSE.btn      = ((rxtstkey(rx_kLBUTTON) != 0) << 0) |
                   ((rxtstkey(rx_kRBUTTON) != 0) << 1) |
                   ((rxtstkey(rx_kMBUTTON) != 0) << 2);


  int result = MOUSE.btn_last != MOUSE.btn;


  if(is_click_enter(0))
  {
    MOUSE.xclick = rx.xcursor;
    MOUSE.yclick = rx.ycursor;
  }

  int yscroll = rxtstkey(rx_kMVWHEEL);

  if(MOUSE.yscroll != yscroll)
  {
    MOUSE.yscroll = yscroll;
  } else
  {
    MOUSE.yscroll = 0;
  }

  return result;
}