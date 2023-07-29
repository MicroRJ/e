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

typedef union erect_t/* lol */
{ struct
  { rxvec2i_t xy0;
    rxvec2i_t xy1;
  };
  struct
  {
    int x0,y0,x1,y1;
  };
} erect_t;

typedef struct ewidget_t
{
  erect_t  rect;
  int      focused;
} ewidget_t;


/* this is something we'd have formalize - XXX - the one called rj */
#define FOREGROUND_COLOR RX_RGBA_UNORM(0xC0,0xC0,0xC0,0xff)
#define BACKGROUND_COLOR RX_RGBA_UNORM(0x12,0x12,0x12,0xff)
#define RECT_HUGE RX_TLIT(erect_t){0,0,0xffff,0xffff}

enum
{ RECT_kLFT ,
  RECT_kRGT,
  RECT_kTOP  ,
  RECT_kBOT };


erect_t
rect_by_xyxy(
  int x0, int y0, int x1, int y1)
{
  erect_t rect;
  rect.x0 = x0; rect.x1 = x1;
  rect.y0 = y0; rect.y1 = y1;

  ccassert(x0 <= x1);
  ccassert(y0 <= y1);
  return rect;
}

erect_t
rect_by_size(
  int x, int y, int xx, int yy)
{
  return rect_by_xyxy(x,y,x+xx,y+yy);
}

/* todo: */
void set_window_rect(HWND wnd, erect_t rect)
{
  SetWindowPos(wnd,HWND_TOP,rect.x0,rect.y0,0,0,SWP_NOSIZE);
}

/* todo: */
erect_t
get_monitor_rect()
{ erect_t rect;
  rect = rect_by_size(0,0,
    GetSystemMetrics(SM_CXSCREEN),
    GetSystemMetrics(SM_CYSCREEN));
  return rect;
}

/* todo: */
erect_t
get_window_rect()
{
  RECT wnd_rect;
  GetWindowRect(rx.Window,&wnd_rect);

  erect_t rect;
  rect = rect_by_xyxy(
      wnd_rect.  left, wnd_rect.   top,
      wnd_rect. right, wnd_rect.bottom );

  return rect;
}

erect_t get_window_client_rect()
{
  erect_t rect;
  rect = rect_by_size(0,0,rx.size_x,rx.size_y);
  return rect;
}

int
rect_in_xy(
  erect_t rect, int x, int y)
{
  return
    ((x >= rect.x0) && (y >= rect.y0)) &&
    ((x <  rect.x1) && (y <  rect.y1));
}

int cursor_in_rect(erect_t rect)
{
  return rect_in_xy(rect,rx.xcursor,rx.ycursor);
}

int is_click_leave_rect(erect_t rect)
{
  return IS_CLICK_LEAVE(0) && cursor_in_rect(rect);
}

int is_click_enter_rect(erect_t rect)
{
  return IS_CLICK_ENTER(0) && cursor_in_rect(rect);
}

erect_t
rect_cut(
  erect_t *rect, int mode, int size)
{
  erect_t result = rect_by_size(0,0,0,0);
  switch(mode)
  { case RECT_kTOP:
    { result = rect_by_xyxy(rect->x0,rect->y1-size,rect->x1,rect->y1);
      rect->y1 -= size;
    } break;
    case RECT_kBOT:
    { result = rect_by_xyxy(rect->x0,rect->y0,rect->x1,rect->y0+size);
      rect->y0 += size;
    } break;
    case RECT_kRGT:
    { result = rect_by_xyxy(rect->x1-size,rect->y0,rect->x1,rect->y1);
      rect->x1 -= size;
    } break;
    case RECT_kLFT:
    { result = rect_by_xyxy(rect->x0,rect->y0,rect->x0+size,rect->y1);
      rect->x0 += size;
    } break;
  }
  return result;
}

erect_t
rect_center(erect_t parent, erect_t child)
{
  erect_t result;
  result.x0 = parent.x0 + ((parent.x1 - parent.x0) >> 1) - ((child.x1 - child.x0) >> 1);
  result.y0 = parent.y0 + ((parent.y1 - parent.y0) >> 1) - ((child.y1 - child.y0) >> 1);
  result.x1 = result.x0 +  (child.x1 - child.x0);
  result.y1 = result.y0 +  (child.y1 - child.y0);
  return result;
}

erect_t
rect_padd(erect_t rect, int xpadd, int ypadd)
{
  return
    rect_by_xyxy(
      rect.x0 + (xpadd >> 1),
      rect.y0 + (ypadd >> 1),
      rect.x1 - (xpadd >> 1),
      rect.y1 - (ypadd >> 1));
}

erect_t
text_rect(int x0, int y0, int height, const char *string)
{
  erect_t result =
    rect_by_size(x0,y0,rxdraw_text_length(height,string),height);

  return result;
}

void
draw_rect(erect_t rect, rxcolor_t color)
{
  rxdraw_rect(color,
    rect.x0, rect.y0,
    (rect.x1 - rect.x0),
    (rect.y1 - rect.y0));
}

void
draw_text(erect_t rect, const char *text, int size, rxcolor_t color)
{
  rxdraw_text_ex(color,rect.x0,rect.y0,size,strlen(text),text);
}

void
set_clip_rect(erect_t rect)
{
  rxclip(
    rect.x0,rect.y0,rect.x1,rect.y1);
}
