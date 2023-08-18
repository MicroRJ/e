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

/* todo: this has to become more advanced */
void
edraw_text(
  efont font,
    float x, float y, int length, char const *string)
{
  if(length == -1)
  {
    length = strlen(string);
  }

  if(length == 0)
  {
    return;
  }

  float vsize = font.vsize;

  rximp_apply();

  rxpip_regset_texture(REG_PS_TEX_0,font.atlas);
  rxpip_regset_sampler(REG_PS_SAM_0,rx.point_sampler);

  for(int i=0;i<length;i+=1)
  {
    int code = *string ++;

    if(code < 32)
      continue;

    stbtt_bakedchar c = font.array[code-32];

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(font.array,
      font.atlas.size_x,
      font.atlas.size_y,
      code - 32,&x,&y,&q,0);

    float xxx = q.y1 - q.y0;

    q.y0 -= 2*c.yoff + xxx;
    q.y1 -= 2*c.yoff + xxx;

    q.x1 = q.x0 + (q.x1 - q.x0); // * /*todo*/(vsize / vsize);
    q.y1 = q.y0 + (q.y1 - q.y0); // * /*todo*/(vsize / vsize);

    // x -= c.xadvance;
    // x += c.xadvance * /*todo*/(vsize / vsize);

    rxbgn();
      rx.imp.attr.rgba = RX_COLOR_WHITE;
      rxaddvtx(rxvtx_xyuv(q.x0,q.y0,q.s0,q.t1));
      rxaddvtx(rxvtx_xyuv(q.x0,q.y1,q.s0,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y1,q.s1,q.t0));
      rxaddvtx(rxvtx_xyuv(q.x1,q.y0,q.s1,q.t1));
      rxaddnidx(6, 0,1,2, 0,2,3);
    rxend();

  }
}


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
    rxcolor_t color = RX_COLOR_CYAN;
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
    edraw_text(widget->font,
      rect.x0 + line->indent * 16 * 2,
      rect.y1 +        - widget->font.vline * (1 + i),
        line->length,widget->buffer.memory + line->offset);
#endif
  }




  set_clip_rect(RECT_HUGE);
  return ccfalse;
}














