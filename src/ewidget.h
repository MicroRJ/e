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

typedef eeditor_t Emu_widget_t;

int cursor_in_rect(rlBOX rect)
{
  return
  rect_in_xy(rect,
		rx.wnd.in.mice.xcursor,
		rx.wnd.in.mice.ycursor);
}

int is_click_leave_rect(rlBOX rect)
{
  return IS_CLICK_LEAVE(0) && cursor_in_rect(rect);
}

int is_click_enter_rect(rlBOX rect)
{
  return IS_CLICK_ENTER(0) && cursor_in_rect(rect);
}

void
rlDrawRect(rlBOX rect, rxcolor_t color)
{
  Emu_imp_rect(color,
    rect.x0, rect.y0,
    (rect.x1 - rect.x0),
    (rect.y1 - rect.y0));
}

void
set_clip_rect(rlBOX rect)
{
  //rximp_clip(
  //  rect.x0,rect.y0,rect.x1,rect.y1);
}
