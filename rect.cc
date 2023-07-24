/*
**
**                      -+- rx -+-
**
**                 Mundane Graphics API.
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**        NOT PRODUCTION READY/NOT PRODUCTION QUALITY
**
**
**               HONI SOIT QUI MAL Y PENSE
**
**
**                 github.com/MicroRJ/rx
**
*/

/* this is something we'd have formalize - XXX - the one called rj */
#define FOREGROUND_COLOR RX_RGBA_UNORM(0xC0,0xC0,0xC0,0xff)
#define BACKGROUND_COLOR RX_RGBA_UNORM(0x12,0x12,0x12,0xff)


enum
{ RECT_kLFT ,
  RECT_kRGT,
  RECT_kTOP  ,
  RECT_kBOT };

#define RECT_HUGE RX_TLIT(rect_t){0,0,0xffff,0xffff}

typedef union rect_t rect_t;
typedef union rect_t
{ struct
  {
    rxvec2i_t xy0;
    rxvec2i_t xy1;
  };
  struct
  {
    int x0,y0,x1,y1;
  };
} rect_t;

int is_click_leave_rect(rect_t rect);
int is_click_enter_rect(rect_t rect);
int mouse_is_key(int x);

rect_t
rect_by_xyxy(
  int x0, int y0, int x1, int y1)
{
  rect_t rect;
  rect.x0 = x0; rect.x1 = x1;
  rect.y0 = y0; rect.y1 = y1;

  ccassert(x0 <= x1);
  ccassert(y0 <= y1);
  return rect;
}

rect_t
rect_by_size(
  int x, int y, int xx, int yy)
{
  return rect_by_xyxy(x,y,x+xx,y+yy);
}

int
rect_in_xy(
  rect_t rect, int x, int y)
{
  return
    ((x >= rect.x0) && (y >= rect.y0)) &&
    ((x <  rect.x1) && (y <  rect.y1));
}

rect_t
rect_cut(
  rect_t *rect, int mode, int size)
{
  rect_t result = rect_by_size(0,0,0,0);
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

rect_t
rect_center(rect_t parent, rect_t child)
{
  rect_t result;
  result.x0 = parent.x0 + ((parent.x1 - parent.x0) >> 1) - ((child.x1 - child.x0) >> 1);
  result.y0 = parent.y0 + ((parent.y1 - parent.y0) >> 1) - ((child.y1 - child.y0) >> 1);
  result.x1 = result.x0 +  (child.x1 - child.x0);
  result.y1 = result.y0 +  (child.y1 - child.y0);
  return result;
}

rect_t
rect_padd(rect_t rect, int xpadd, int ypadd)
{
  return
    rect_by_xyxy(
      rect.x0 + (xpadd >> 1),
      rect.y0 + (ypadd >> 1),
      rect.x1 - (xpadd >> 1),
      rect.y1 - (ypadd >> 1));
}

rect_t text_rect(int x0, int y0, int height, const char *string)
{
  rect_t result =
    rect_by_size(x0,y0,rxdraw_text_length(height,string),height);

  return result;
}

void set_window_rect(HWND wnd, rect_t rect)
{
  SetWindowPos(wnd,HWND_TOP,rect.x0,rect.y0,0,0,SWP_NOSIZE);
}

rect_t get_monitor_rect()
{ rect_t rect;
  rect = rect_by_size(0,0,
    GetSystemMetrics(SM_CXSCREEN),
    GetSystemMetrics(SM_CYSCREEN));
  return rect;
}

rect_t get_window_rect()
{
  RECT wnd_rect;
  GetWindowRect(rx.Window,&wnd_rect);

  rect_t rect;
  rect = rect_by_xyxy(
      wnd_rect.  left, wnd_rect.   top,
      wnd_rect. right, wnd_rect.bottom );

  return rect;
}

rect_t get_window_client_rect()
{
  rect_t rect;
  rect = rect_by_size(0,0,rx.size_x,rx.size_y);
  return rect;
}

void draw_rect(rect_t rect, rxcolor_t color)
{
  rxdraw_rect(color,
    rect.x0, rect.y0,
    (rect.x1 - rect.x0),
    (rect.y1 - rect.y0));
}

void draw_text(rect_t rect, const char *text, int size, rxcolor_t color)
{
  rxdraw_text_ex(color,rect.x0,rect.y0,size,strlen(text),text);
}

void set_clip_rect(rect_t rect)
{
  rxclip(
    rect.x0,rect.y0,rect.x1,rect.y1);
}

int button_widget(rect_t rect, const char *string)
{
  set_clip_rect(rect);

  draw_rect(rect,BACKGROUND_COLOR);

  rect_t the_text_rect =
    rect_center(rect,text_rect(0,0,32,string));

  draw_text(the_text_rect,string,32,
    rect_in_xy(rect,rx.xcursor,rx.ycursor) ?
      RX_COLOR_GREEN : FOREGROUND_COLOR);

  int result = is_click_leave_rect(rect);

  set_clip_rect(RECT_HUGE);
  return result;
}

int checkbox_widget(rect_t rect, char const *label, int *is_on)
{
  set_clip_rect(rect);

  draw_rect(rect,BACKGROUND_COLOR);

  int is_over = rect_in_xy(rect,rx.xcursor,rx.ycursor);


  rect_t r;
  rxcolor_t color;
  rect_t q;

  r = rect_padd(rect,4,4);

  /* the indicator */
  { color = *is_on ? RX_COLOR_GREEN : RX_COLOR_RED;

    char const *ts = *is_on?"[yes]":"[nop]";

    q = text_rect(0,0,32,ts);
    q = rect_center(rect_cut(&r,RECT_kRGT,q.x1-q.x0),q);
    draw_text(q,ts,32,color);
  }
  /* the label */
  { color = is_over ? RX_COLOR_GREEN : FOREGROUND_COLOR;

    q = text_rect(0,0,32,label);
    q = rect_cut(&r,RECT_kLFT,q.x1-q.x0);
    draw_text(q,label,32,color);
  }

  if(is_click_leave_rect(rect))
  {
    *is_on = !*is_on;
  }

  set_clip_rect(RECT_HUGE);

  return *is_on;
}

int dropdown_widget(rect_t rect, const char *string, int *is_on)
{
  set_clip_rect(rect);

  draw_rect(rect,BACKGROUND_COLOR);

  int is_over = rect_in_xy(rect,rx.xcursor,rx.ycursor);

  rxcolor_t color = is_over ? RX_COLOR_GREEN :
    (*is_on) ? RX_COLOR_YELLOW : FOREGROUND_COLOR;

  rect_t the_btn_prnt_rect = rect;
  char const *the_btn_text = *is_on?"[-]":"[+]";

  rect_t the_btn_text_rect = text_rect(0,0,32,the_btn_text);
  the_btn_text_rect = rect_center(
    rect_cut(&the_btn_prnt_rect,RECT_kLFT,the_btn_text_rect.x1-the_btn_text_rect.x0),the_btn_text_rect);

  draw_text(the_btn_text_rect,the_btn_text,32,color);

  rect_t the_text_rect = rect_center(rect,text_rect(0,0,32,string));
  draw_text(the_text_rect,string,32,color);

  if(is_click_leave_rect(rect))
  {
    *is_on = !*is_on;
  }

  set_clip_rect(RECT_HUGE);

  return *is_on;
}

int cursor_in_rect(rect_t rect);
int is_click_enter(int x);
int rxtstkey(int x);
int rxchr();

int list_item_widget(rect_t rect, rxcolor_t color, const char *string)
{
  set_clip_rect(rect);

  rect_t the_text_rect = rect;

  draw_rect(rect,BACKGROUND_COLOR);

  draw_text(the_text_rect,string,32,
    rect_in_xy(rect,rx.xcursor,rx.ycursor)?RX_COLOR_GREEN:color);

  int result = is_click_leave_rect(rect);

  set_clip_rect(RECT_HUGE);
  return result;
}

int slider_widget(rect_t rect, const char *label, double min, double max, double *value)
{
  draw_rect(rect,BACKGROUND_COLOR);

  rect_t the_inner_rect = rect_padd(rect,4,4);

  int the_thumb_offset =
    rxremix(
      rxclamp(*value,min,max),min,max,0,
        the_inner_rect.x1 - the_inner_rect.x0 - 0x10);

  rect_t the_thumb_rect =
    rect_by_xyxy(
      the_inner_rect.x0 + the_thumb_offset + 0x00, the_inner_rect.y0 + 0x00,
      the_inner_rect.x0 + the_thumb_offset + 0x10, the_inner_rect.y1 + 0x00 );

  int is_active = mouse_is_key(0) && rect_in_xy(rect,rx.xcursor,rx.ycursor);

  if(is_active)
  {
    *value = rxclamp(rxremix(rx.xcursor,
      the_inner_rect.x0,the_inner_rect.x1,min,max),min,max);
  }


  draw_rect(the_thumb_rect,RX_RGBA_UNORM(0x32,0x32,0x32,0xFF));

  return is_active;
}