/*
**
**                        -+- e -+-
**
**         A Text Editor For Those Who Fear Death
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**
**                    [ MEMENTO MORI ]
**
**
**                  github.com/MicroRJ/e
**
**
**                  USE AT YOUR OWN RISK
**
*/

typedef struct ecur_t
{
  /* there's no need to store this per cursor */
  unsigned select: 1;
  int      selmin;
  int      selmax;

  int      xchar;
  int      yline;
} ecur_t;


typedef struct erow_t
{
  int offset;
  int length;
} erow_t;


typedef struct ewdg_t
{
  /* theme */
  rxcolor_t    color;

  int        focused;

  ecur_t      cursor;
  int         lyview;

  erow_t     *lcache;

  char       *buffer;
  int         length;

} ewdg_t;

/* implementation */
void
erecache(
  ewdg_t *widget, int length, char const *buffer)
{
  char const *cursor = buffer;

  ccarrdel(widget->lcache);
  widget->lcache = 0;

retry:
  if(cursor < buffer + length)
  {
    // if(ccarrlen(widget->lcache) >= 64)
    // {
    //   return;
    // }
    erow_t *line = ccarradd(widget->lcache,1);
    line->offset = cursor - buffer;
    line->length = 0;

    while(cursor < buffer + length)
    {
      if(*cursor == '\n')
      { cursor += 1;

        if(*cursor == '\r')
          cursor += 1;

        goto retry;
      } else
      if(*cursor == '\r')
      { cursor += 1;

        if(*cursor == '\n')
          cursor += 1;

        goto retry;
      } else
      {
        cursor += 1;
        line->length += 1;
      }
    }
  }
}

int
buflen(
  ewdg_t *widget)
{
  return widget->length;
}

/* get line at index */
erow_t
egetrow(
  ewdg_t *wdg, int index)
{
  return wdg->lcache[index];
}

/* get the length of the line at index, does not include line terminator */
int
egetlen(
  ewdg_t *wdg, int index)
{
  return egetrow(wdg,index).length;
}

/* get the offset of line at index, the offset is the same as the location in buffer */
int
egetoff(
  ewdg_t *wdg, int index)
{
  return egetrow(wdg,index).offset;
}

void
esetcur(ewdg_t *wdg, ecur_t cur)
{
  wdg->cursor = cur;
}

/* get the cursor at index, (index not implemented) */
ecur_t
egetcur(ewdg_t *wdg)
{
  return wdg->cursor;
}

/* get the y coordinate of the buffer in line space */
int
egetcurx(ewdg_t *wdg)
{
  return egetcur(wdg).xchar;
}

/* get the x coordinate of the buffer in line space */
int
egetcury(ewdg_t *wdg)
{
  return egetcur(wdg).yline;
}

/* get the position of the cursor within the buffer */
int
ecurloc(ewdg_t *wdg)
{
  return egetoff(wdg,egetcury(wdg)) + egetcurx(wdg);
}

rect_t
currec(
  ewdg_t *widget,
  /* this is temporary, the widget should store this? */
    rect_t rect)
{
  int xcur = widget->cursor.xchar;
  int ycur = widget->cursor.yline;

  ycur -= widget->lyview;

  ycur  *= 32;
  xcur  *= rxchrxsz(32);
  return rect_by_size(rect.x0+xcur,rect.y1-ycur-32,16,32);
}



void
emovcurx(
  ewdg_t *wdg, int mov)
{
  ecur_t cur = egetcur(wdg);

  if(cur.xchar + mov > egetlen(wdg,cur.yline))
  {
    if(cur.yline + 1 <= ccarrlen(wdg->lcache) - 1)
    {
      cur.yline += 1;
      cur.xchar  = 0;
    }
  } else
  if(cur.xchar + mov < 0)
  {
    if(cur.yline - 1 >= 0)
    {
      cur.yline -= 1;
      cur.xchar  = egetlen(wdg,cur.yline);
    }
  } else
  {
    cur.xchar += mov;
  }

  esetcur(wdg,cur);
}


void
emovcury(
  ewdg_t *wdg, int mov)
{
  ecur_t cur = egetcur(wdg);

  if(cur.yline + mov <= ccarrlen(wdg->lcache) - 1 &&
     cur.yline + mov >= 0 )
  {
    cur.yline += mov;
    cur.xchar =  rxclamp(cur.xchar,0,egetlen(wdg,cur.yline));

    esetcur(wdg,cur);
  }
}


void
eputchar(
  ewdg_t *widget, int chr)
{
  ccarradd(widget->buffer,1);
  widget->length += 1;

  int loc = ecurloc(widget);
  char *mem = &widget->buffer[loc];

  int mov = ccarrlen(widget->buffer) - loc - 1;
  memmove(mem+1,mem,mov);

  *mem = chr;

  /* remove */
  erecache(widget,widget->length,widget->buffer);
}

void
edelchar(
  ewdg_t *widget)
{
  int loc = ecurloc(widget);

  char *mem = &widget->buffer[loc];
  *mem = 0;

  int mov = buflen(widget) - loc - 1;
  memmove(mem,mem+1,mov);

  widget->length -= 1;

  // /* remove */
  erecache(widget,ccarrlen(widget->buffer),widget->buffer);
}

void
fload(
  ewdg_t *widget, char const *name)
{
  if(name != 0 && strlen(name) != 0)
  {
    void *file = ccopenfile(name,"r");

    ccu32_t length = 0;
    void *memory = ccpullfile(file,0,&length);

    if(memory != 0)
    {
      ccarrdel(widget->buffer);
      widget->buffer = 0;

      void *buffer = ccarradd(widget->buffer,length);
      memcpy(buffer,memory,length);

      widget->length = length;

      erecache(widget,length,buffer);
    }
  }
}


void
fsave(
  ewdg_t *widget, char const *name)
{
  if(name != 0 && strlen(name) != 0)
  {
    void *file = ccopenfile(name,"w");

    ccu32_t length = widget->length;
    ccpushfile(file,0,length,widget->buffer);

    ccclosefile(file);
  }
}

int
ewdg(rect_t rect, ewdg_t *widget)
{
  int in_rect = cursor_in_rect(rect);

  if(is_click_enter(0))
  {
    if(in_rect)
    {
      widget->focused = cctrue;
    }
  }

  if(widget->focused)
  {
    if(rxtstkey(rx_kKEY_UP))
    { if(rx.is_ctrl)
      {
        widget->lyview -= 1;
        widget->lyview = rxclampi(widget->lyview,0,ccarrlen(widget->lcache)-1);
      } else
      {
        emovcury(widget,-1);
      }
    } else
    if(rxtstkey(rx_kKEY_DOWN))
    {
      if(rx.is_ctrl)
      {
        widget->lyview += 1;
        widget->lyview = rxclampi(widget->lyview,0,ccarrlen(widget->lcache)-1);
      } else
      {
        emovcury(widget,+1);
      }
    } else
    if(rxtstkey(rx_kKEY_LEFT))
    { if(rx.is_ctrl)
      {
      } else
      {
        emovcurx(widget,-1);
      }
    } else
    if(rxtstkey(rx_kKEY_RIGHT))
    {
      if(rx.is_ctrl)
      {
      } else
      {
        emovcurx(widget,+1);
      }
    } else
    if(rxtstkey(rx_kDELETE))
    { edelchar(widget    );
    } else
    if(rxtstkey(rx_kBCKSPC))
    { if(ecurloc(widget) != 0)
      { emovcurx(widget, -1);
        edelchar(widget    );
      }
    } else
    if(rxtstkey(rx_kRETURN))
    { eputchar(widget, '\r');
      eputchar(widget, '\n');
      emovcurx(widget,    1);
    } else
    { if(rxchr() != 0)
      { eputchar(widget, rxchr());
        emovcurx(widget,       1);
      }
    }

  }

  /* drawing */
  // set_clip_rect(rect);

  rxcolor_t color = BACKGROUND_COLOR;

  draw_rect(rect,color);

  for(int i=0; i<rxmini(ccarrlen(widget->lcache) - widget->lyview,32); i += 1)
  {
    erow_t *the_line = widget->lcache + widget->lyview + i;

    rxdraw_text_ex(FOREGROUND_COLOR,
      rect.x0 +       0,
      rect.y1 +    - 32 * (1 + i),32,
        the_line->length,widget->buffer+the_line->offset);
  }

  color = RX_COLOR_GREEN;
  color.a = rxclamp((.5+sin((rx.total_seconds/.250) * rxPI_F))*.5,.05,1.);

  draw_rect(currec(widget,rect),color);

  set_clip_rect(RECT_HUGE);
  return ccfalse;
}

