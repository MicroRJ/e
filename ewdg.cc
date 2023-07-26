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

void
erecache(
  ewdg_t *widget)
{

  int   length = widget->buffer.length;
  char *buffer = widget->buffer.memory;
  char *cursor = buffer;

  /* todo */
  ccarrdel(widget->lcache);
  widget->lcache = 0;

  int indent = 0;
  for(;;)
  {
    erow_t *line = ccarradd(widget->lcache,1);
    line->offset = (cursor - buffer);
    line->indent = indent;

    while((cursor < buffer + length) &&
          (*cursor != '\r') &&
          (*cursor != '\n'))
    {
      /* probably integrate this better */
      if(*cursor == '{')
      {
        line->indent = indent;
        indent += 1;
      } else
      if(*cursor == '}')
      {
        indent -= 1;
        line->indent = indent;
      }

      cursor += 1;
    }

    line->length = (cursor - buffer) - line->offset;

    if(cursor < buffer + length)
    {
      cursor += ((cursor[0] == '\r') &&
                 (cursor[1] == '\n'))  ? 2 : 1;
    } else
    {
      break;
    }
  }

  ccassert(cursor == buffer + length);
}

/* get line at index */
erow_t
egetrow(
  ewdg_t *wdg, int index)
{
  /* remove, this should not happen? */
  if(wdg->lcache == ccnull)
  {
    erow_t row;
    row.indent = 0;
    row.length = 0;
    row.offset = 0;
    return row;
  }

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

/* get the cursor at index, (index not implemented) */
ecursor_t
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

void
esetcur(ewdg_t *wdg, ecursor_t cur)
{
  wdg->cursor.yline = rxclamp(cur.yline,0,ccarrlen(wdg->lcache)-1);
  wdg->cursor.xchar = rxclamp(cur.xchar,0,egetlen(wdg,cur.yline));
}

void
esetcurx(ewdg_t *wdg, int xchar)
{
  ecursor_t cur = egetcur(wdg);

  cur.xchar = xchar;

  esetcur(wdg,cur);
}

void
esetcury(ewdg_t *wdg, int yline)
{
  ecursor_t cur = egetcur(wdg);

  cur.yline = yline;

  esetcur(wdg,cur);
}

/* get the position of the cursor within the buffer */
int
ecurloc(ewdg_t *wdg)
{
  int loc = egetoff(wdg,egetcury(wdg)) + egetcurx(wdg);
  return loc;
}

char
ecurchr(ewdg_t *wdg, int off)
{
  int loc = ecurloc(wdg);

  if(CCWITHIN(loc+off,0,wdg->buffer.length-1))
  {
    return wdg->buffer.memory[loc+off];
  }

  return 0;
}

char *
ecurptr(ewdg_t *wdg)
{
  return &wdg->buffer.memory[ecurloc(wdg)];
}

rxrect_t
ecurrec(
  ewdg_t *widget,
  /* this is temporary, the widget should store this? */
    rxrect_t rect)
{

  ecursor_t cur = egetcur(widget);
  int xcur = cur.xchar;
  int ycur = cur.yline;
  erow_t row = egetrow(widget,ycur);

  int y = ycur * 32 - widget->lyview * 32;
  int x = row.indent * 16 * 2 + xcur * rxchrxsz(32);

  return rect_by_size(rect.x0+x,rect.y1-y-32,16,32);
}

void
emovcurx(
  ewdg_t *wdg, int mov)
{
  ecursor_t cur = egetcur(wdg);

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
  esetcury(wdg,egetcury(wdg)+mov);
}

void
enewline(
  ewdg_t *wdg)
{
  int num = 1;

  if(ecurchr(wdg,-1) == '{') num += 1;

  char *ptr = ebuffer_insert(&wdg->buffer,ecurloc(wdg),num*2);
  while(num -- != 0)
  { /* proper line end feed */
    ptr[num*2+0] = '\r';
    ptr[num*2+1] = '\n';
  }

  /* remove */
  erecache(wdg);

  emovcury(wdg,1);
  esetcurx(wdg,0);
}

void
eputchar(
  ewdg_t *wdg, int chr)
{
  ccassert(chr != '\r' && chr != '\n');

  int end = 0;
  switch(chr)
  {
  case '{': end = '}'; break;
  case '[': end = ']'; break;
  case '(': end = ')'; break;
  case '}':
  case ']':
  case ')':
    if(chr == *ecurptr(wdg))
      return;
  }

  char *mem = ebuffer_insert(&wdg->buffer,ecurloc(wdg),end!=0?2:1);
  mem[0] = chr;

  if(end != 0)
  {
    mem[1] = end;
  }


  erecache(wdg);
}

/* delete the character or characters at the cursor's position */
void
edelchar(
  ewdg_t *wdg)
{
  if(wdg->buffer.length <= 0)
  {
    return;
  }

  char *ptr = ecurptr(wdg);
  /* just check if you're at the begging of a line in which case you just delete
    it, the problem is that the user is expected to move the cursor beforehand, so
    in which case it might by simpler to have to separate delete functions */
  int num = 1;
  if(ptr[0] == '\r' &&
     ptr[1] == '\n') num += 1;

  ebuffer_delete(&wdg->buffer,ecurloc(wdg),num);

  /* remove */
  erecache(wdg);
}


/* remove this please */
ccglobal ewdg_t *gw;

int
edraw_text_run_callback(
  void *user, int index, int *count, int *code, rxcolor_t *color)
{
  erow_t *line = user;

  char *cursor = gw->buffer.memory + line->offset + index;

  *code = *cursor;

  /* this is so stupid */
  if(*count <= 0)
  {
    *color = RX_RGBA_UNORM(221,206,150,255);

    gw->reader.doc_max=cursor + (line->length-index);
    gw->reader.doc_min=
    gw->reader.tok_min=
    gw->reader.tok_max=cursor;

    ccread_token(&gw->reader);
    switch(gw->reader.tok.kind)
    { case cctoken_kLITSTR:
      { *color = RX_COLOR_TEAL;
      } break;
      case cctoken_kLITINT:
      case cctoken_kLITFLO:
      { *color = RX_COLOR_CYAN;
      } break;
      case cctoken_kIF:
      case cctoken_kELSE:
      case cctoken_Kswitch:
      case cctoken_Kcase:
      case cctoken_Kdefault:
      case cctoken_kFOR:
      case cctoken_kWHILE:
      case cctoken_Kdo:
      case cctoken_kGOTO:
      case cctoken_kRETURN:
      case cctoken_Kbreak:
      case cctoken_Kcontinue:
      case cctoken_kVOID:
      {
        *color = RX_COLOR_RED;
      } break;
      case cctoken_kSTDC_INT:
      case cctoken_kSTDC_LONG:
      case cctoken_kSTDC_SHORT:
      case cctoken_kSTDC_DOUBLE:
      case cctoken_kSTDC_FLOAT:
      case cctoken_kSTDC_CHAR:
      case cctoken_kSTDC_BOOL:
      case cctoken_kSTDC_SIGNED:
      case cctoken_kSTDC_UNSIGNED:
      case cctoken_kMSVC_INT8:
      case cctoken_kMSVC_INT16:
      case cctoken_kMSVC_INT32:
      case cctoken_kMSVC_INT64:
      case cctoken_kENUM:
      case cctoken_kSTRUCT:

      case cctoken_Ktypedef:
      case cctoken_Kauto:
      case cctoken_Kextern:
      case cctoken_Kregister:
      case cctoken_Kstatic:
      case cctoken_Kthread_local:
      case cctoken_Kmsvc_declspec:
      {
        *color = RX_COLOR_MAGENTA;
      } break;
    }

    *count = gw->reader.tok_max - gw->reader.tok_min;
  }

  return index < line->length;

}

int
ewdg(rxrect_t rect, ewdg_t *widget)
{
  int in_rect = cursor_in_rect(rect);

  if(IS_CLICK_ENTER(0))
  {
    if(in_rect)
    {
      widget->focused = cctrue;
    }
  }

  if(widget->focused)
  {

    if(rxtstkey(rx_kHOME))
    { esetcurx(widget,0);
    } else
    if(rxtstkey(rx_kEND))
    { esetcurx(widget,egetlen(widget,egetcury(widget)));
    } else
    if(rxtstkey(rx_kKEY_UP))
    { if(rxisctrl())
      {
        widget->lyview -= rxisshft() ? 16 : 1;
        widget->lyview = rxclampi(widget->lyview,0,ccarrlen(widget->lcache)-1);
      } else
      {
        emovcury(widget,-1);
      }
    } else
    if(rxtstkey(rx_kKEY_DOWN))
    {
      if(rxisctrl())
      {
        widget->lyview += rxisshft() ? 16 : 1;
        widget->lyview = rxclampi(widget->lyview,0,ccarrlen(widget->lcache)-1);
      } else
      {
        emovcury(widget,+1);
      }
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
    { if(ecurloc(widget) < widget->buffer.length)
      {
        edelchar(widget);
      }
    } else
    if(rxtstkey(rx_kBCKSPC))
    { if(ecurloc(widget) != 0)
      { emovcurx(widget, -1);
        edelchar(widget    );
      }
    } else
    if(rxtstkey(rx_kRETURN))
    {
      enewline(widget);
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
    rxcolor_t color = RX_COLOR_GREEN;
    color.a = rxclamp((.5+sin((rx.total_seconds/.250) * rxPI_F))*.5,.05,1.);
    draw_rect(ecurrec(widget,rect),color);
  }

  set_clip_rect(RECT_HUGE);
  return ccfalse;
}











