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
      EMU_imp_rect_sdf(
        rxvec2_xy(
          cur_rec.x0 + (cur_rec.x1 - cur_rec.x0) * .5,
          cur_rec.y0 + (cur_rec.y1 - cur_rec.y0) * .5),
        rxvec2_xy(
          .5*(cur_rec.x1-cur_rec.x0),
          .5*(cur_rec.y1-cur_rec.y0)) , color, 2.5, 1.);
    }
  }

  for(int i=0; i<rxmini(earray_length(widget->buffer.lcache) - widget->lyview,64); i += 1)
  {
    emarker_t *line = widget->buffer.lcache + widget->lyview + i;

    EMU_draw_text_config_t config =
			draw_text_config_init(widget->font,widget->text_size,
				0,0,RX_RGBA(0,0,0,0),NULL,NULL,line->length,NULL);
		config.color_table = widget->buffer.syntax.color_table;
		config.color_array = widget->buffer.colors + line->offset;
		config.string      = widget->buffer.memory + line->offset;
    config.x           = rect.x0;
    config.y           = rect.y1 - (widget->line_height) * (1 + i);

		edraw_text( &config );
  }

  set_clip_rect(RECT_HUGE);
  return ccfalse;
}














