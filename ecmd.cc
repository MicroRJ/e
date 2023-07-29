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
ecmd(erect_t rect, eeditor_t *widget)
{
	int result = ccfalse;


  // /* drawing */
  // set_clip_rect(rect);

  // rxcolor_t color = BACKGROUND_COLOR;

  // draw_rect(rect,color);

  // for(int i=0; i<rxmini(ccarrlen(widget->lcache) - widget->lyview,32); i += 1)
  // {
  //   erow_t *line = widget->lcache + widget->lyview + i;
  //   rxdraw_text_run(
  //     rect.x0 + line->indent * 16 * 2,
  //     rect.y1 +        - 32 * (1 + i),32,widget,eeditor_draw_text_run_callback);
  // }

  // if(widget->widget.focused)
  // {
  //   color = RX_COLOR_GREEN;
  //   color.a = rxclamp((.5+sin((rx.total_seconds/.250) * rxPI_F))*.5,.05,1.);
  //   draw_rect(ecurrec(widget,rect),color);
  // }

  // set_clip_rect(RECT_HUGE);

  return result;
}