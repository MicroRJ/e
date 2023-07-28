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


int
eeditor_draw_text_run_callback(
  void *user, int index, int *code, rxcolor_t *color);

int
ewdg(erect_t rect, eeditor_t *widget)
{
  int in_rect = cursor_in_rect(rect);

  if(IS_CLICK_ENTER(0))
  {
    if(in_rect)
    {
      widget->widget.focused = cctrue;
    }
  }

  if(widget->widget.focused)
  {
    eeditor_msg(widget);
  }


  /* drawing */
  set_clip_rect(rect);

  memset(&widget->style,0,sizeof(widget->style));

  for(int i=0; i<rxmini(ccarrlen(widget->lcache) - widget->lyview,32); i += 1)
  {
    widget->style.yline = widget->lyview + i;

    erow_t *line = widget->lcache + widget->lyview + i;
    rxdraw_text_run(
      rect.x0 + line->indent * 16 * 2,
      rect.y1 +        - 32 * (1 + i),32,widget,eeditor_draw_text_run_callback);
  }

  if(widget->widget.focused)
  {
  	/* todo */
    rxcolor_t color = RX_COLOR_GREEN;
    color.a = rxclamp((.5+sin((rx.total_seconds/.250) * rxPI_F))*.5,.05,1.);

  	for(int i=0;i<ccarrlen(widget->cursor);i+=1)
  	{
	    draw_rect(ecurrec(widget,i,rect),color);
  	}
  }

  set_clip_rect(RECT_HUGE);
  return ccfalse;
}











