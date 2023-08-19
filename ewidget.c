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


  if(widget->widget.focused)
  {
    double animation = .5 + .5 * sin(rxPI_F * rx.total_seconds/.250);
    /* todo */
    rxcolor_t color = RX_RGBA_UNORM(148,232,148,255);
    color.a = rxclamp(animation,.1,.8);

    for(int i=0;i<ccarrlen(widget->cursor);i+=1)
    {
      erect_t cur_rec = ecurrec(widget,i,rect);

      /* this function should just take an e-rect */
      draw_box_sdf(
        rxvec2_xy(
          cur_rec.x0 + (cur_rec.x1 - cur_rec.x0) * .5,
          cur_rec.y0 + (cur_rec.y1 - cur_rec.y0) * .5),
        rxvec2_xy(
          .5*(cur_rec.x1-cur_rec.x0),
          .5*(cur_rec.y1-cur_rec.y0)) , color, 2.5, 1.);
    }
  }

  memset(&widget->style,0,sizeof(widget->style));
  for(int i=0; i<rxmini(ccarrlen(widget->lcache) - widget->lyview,24); i += 1)
  {
    widget->style.yline = widget->lyview + i;

    ecurrow_t *line = widget->lcache + widget->lyview + i;

#if 0
    rxdraw_text_run(
      rect.x0 + line->indent * 16 * 2,
      rect.y1 +        - 32 * (1 + i),32,widget,eeditor_draw_text_run_callback);
#else
    edraw_text(widget->font,widget->text_size,RX_RGBA_UNORM(122,104,81,255),
      rect.x0 + line->indent * 16 * 2,
      rect.y1 +        - widget->font.vline * (1 + i),
        line->length,widget->buffer.memory + line->offset);
#endif
  }




  set_clip_rect(RECT_HUGE);
  return ccfalse;
}














