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

typedef struct ewidget_t
{
  erect_t  rect;
  int      focused;
} ewidget_t;

/* this is something we'd have formalize - XXX - the one called rj */
#define FOREGROUND_COLOR RX_RGBA_UNORM(0xC0,0xC0,0xC0,0xff)
#define BACKGROUND_COLOR RX_RGBA_UNORM(0x12,0x12,0x12,0xff)
#define RECT_HUGE RX_TLIT(erect_t){0,0,0xffff,0xffff}

int cursor_in_rect(erect_t rect)
{
  return
  rect_in_xy(rect,
		rx.wnd.in.mice.xcursor,
		rx.wnd.in.mice.ycursor);
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
text_rect(int x0, int y0, int height, const char *string)
{
  erect_t result =
    erect_xywh(x0,y0,rxdraw_text_length(height,string),height);

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
  //rximp_clip(
  //  rect.x0,rect.y0,rect.x1,rect.y1);
}
