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
ecmd(rxrect_t rect, ewdg_t *widget)
{
	int result = ccfalse;

  if(widget->focused)
  {
    if(rxtstkey(rx_kHOME))
    {
    	esetcurx(widget,0);
    } else
    if(rxtstkey(rx_kEND))
    {
    	esetcurx(widget,egetlen(widget,egetcury(widget)));
    } else
    if(rxtstkey(rx_kKEY_UP))
    {
    	/* history */
    } else
    if(rxtstkey(rx_kKEY_DOWN))
    {
    	/* history */
    } else
    if(rxtstkey(rx_kKEY_LEFT))
    { if(rxisctrl())
      {
      } else
      {
        emovcurx(widget,-1);
      }
    } else
    if(rxtstkey(rx_kKEY_RIGHT))
    {
      if(rxisctrl())
      {
      } else
      {
        emovcurx(widget,+1);
      }
    } else
    if(rxtstkey(rx_kDELETE))
    {
    	edelchar(widget    );
    } else
    if(rxtstkey(rx_kBCKSPC))
    { if(ecurloc(widget) != 0)
      { emovcurx(widget, -1);
        edelchar(widget    );
      }
    } else
    if(rxtstkey(rx_kRETURN))
    {
    	result = cctrue;
    } else
    { if(rxisctrl())
      {

      } else
      {
        if(rxchr() != 0)
        { eputchar(widget, rxchr());
          emovcurx(widget,       1);
        }
      }
    }

  }


  /* drawing */
  set_clip_rect(rect);

  rxcolor_t color = BACKGROUND_COLOR;

  draw_rect(rect,color);

  for(int i=0; i<rxmini(ccarrlen(widget->lcache) - widget->lyview,32); i += 1)
  {
    gw = widget;
    erow_t *line = widget->lcache + widget->lyview + i;
    rxdraw_text_run(
      rect.x0 + line->indent * 16 * 2,
      rect.y1 +        - 32 * (1 + i),32,line,edraw_text_run_callback);
  }

  if(widget->focused)
  {
    color = RX_COLOR_GREEN;
    color.a = rxclamp((.5+sin((rx.total_seconds/.250) * rxPI_F))*.5,.05,1.);
    draw_rect(ecurrec(widget,rect),color);
  }

  set_clip_rect(RECT_HUGE);

  return result;
}