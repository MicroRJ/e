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

/* todo: cursors need to be sorted and merged if occupy the same slot */

int
ecurnum(
  eeditor_t *editor)
{
  return ccarrlen(editor->cursor);
}

int
ecurcmp(
  ecursor_t c0, ecursor_t c1)
{
  return
    c0.yline != c1.yline ?
      (c0.yline > c1.yline ? +1 :
       c0.yline < c1.yline ? -1 : 0) :

      (c0.xchar > c1.xchar ? +1 :
       c0.xchar < c1.xchar ? -1 : 0) ;
}

int
efndcur(
  eeditor_t *editor, int xchar, int yline)
{
  /* todo */
  for(int i=0; i<ecurnum(editor); i+=1)
  { if( (egetcurx(editor,i) == xchar) &&
        (egetcury(editor,i) == yline) ) return i;
  }

  return - 1;
}

/* probably do not use bubble sort */
void
esrtcur(
  eeditor_t *editor)
{
  int sorted;

  do
  {
    sorted = cctrue;

    for(int i=0; i<ecurnum(editor)-1; i+=1)
    {
      ecursor_t c0 = egetcur(editor,i+0);
      ecursor_t c1 = egetcur(editor,i+1);

      int cmp = ecurcmp(c0,c1);
      if(cmp == +1)
      { esetcur(editor,i+0,c1);
        esetcur(editor,i+1,c0);

        sorted = ccfalse;
      }
    }
  } while(sorted != cctrue);
}


int
eaddcur(
  eeditor_t *editor, ecursor_t cur)
{
  /* todo: make this better */
  *ccarradd(editor->cursor,1) = cur;
  *ccarradd(editor->curinf,1) = 0;

  esrtcur(editor);
  return efndcur(editor,cur.xchar,cur.yline);
}

void
erecache(
  eeditor_t *widget)
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
    ecurrow_t *line = ccarradd(widget->lcache,1);
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
ccinle ecurrow_t
egetrow(
  eeditor_t *wdg, int index)
{
  /* remove, this should not happen? */
  if(wdg->lcache == ccnull)
  {
    ecurrow_t row;
    row.indent = 0;
    row.length = 0;
    row.offset = 0;
    return row;
  }

  return wdg->lcache[index];
}

ccinle int
erowlen(
  eeditor_t *wdg, int index)
{
  return egetrow(wdg,index).length;
}

ccinle int
egetloc2(
  eeditor_t *wdg, int xchar, int yline)
{
  return egetrow(wdg,yline).offset + xchar;
}

ccinle int
ecurloc(
  eeditor_t *editor, int index)
{
  return egetloc2(editor,
            egetcurx(editor,index),
            egetcury(editor,index));
}

ccinle char *
egetptr(eeditor_t *wdg, int index)
{
  /* todo: this should point somewhere safer */
  ccglobal char p;

  p = 0; /* in case it was overwritten */

  if(index >= 0 && index < wdg->buffer.length)
  {
    return &wdg->buffer.memory[index];
  }

  return &p;
}

ccinle char
egetchr(eeditor_t *wdg, int index)
{
  return *egetptr(wdg,index);
}

ccinle char *
egetptr2(
  eeditor_t *wdg, int xchar, int yline)
{
  return egetptr(wdg,egetloc2(wdg,xchar,yline));
}

ccinle float
egetinf(
  eeditor_t *editor, int index)
{
  return editor->curinf[index];
}

ccinle ecursor_t
egetcur(
  eeditor_t *editor, int index)
{
  return editor->cursor[index];
}

ccinle int
egetcurx(
  eeditor_t *editor, int index)
{
  return egetcur(editor,index).xchar;
}

ccinle int
egetcury(
  eeditor_t *editor, int index)
{
  return egetcur(editor,index).yline;
}

/* todo: */
/* todo: check for all the cursors with the same coordinates and join them */
void
esetcur(
  eeditor_t *editor, int index, ecursor_t cur)
{
  /* ensure the cursor is never out of bounds */
  cur.yline = rxclamp(cur.yline,0,ccarrlen(editor->lcache)-1);
  cur.xchar = rxclamp(cur.xchar,0,erowlen(editor,cur.yline));

  ecursor_t old = egetcur(editor,index);

  float inf = egetinf(editor,index);

  if(cur.yline != old.yline)
  {
    old.xchar = 0;

    inf = 0;
  }

  if(cur.xchar != old.xchar)
  {
    char *ptr = egetptr2(editor,old.xchar,cur.yline);
    if(cur.xchar > old.xchar)
    { for(int i=0;i<cur.xchar-old.xchar;i+=1)
        inf += efont_code_xadv(editor->font,ptr[i-0]);
    } else
    if(cur.xchar < old.xchar)
    { for(int i=0;i>cur.xchar-old.xchar;i-=1)
        inf -= efont_code_xadv(editor->font,ptr[i-1]);
    }
  }

  editor->cursor[index] = cur;
  editor->curinf[index] = inf;
}

void
esetcurx(eeditor_t *editor, int index, int xchar)
{
  ecursor_t cur = egetcur(editor,index);

  cur.xchar = xchar;

  esetcur(editor,index,cur);
}

void
esetcury(eeditor_t *editor, int index, int yline)
{
  ecursor_t cur = egetcur(editor,index);

  cur.yline = yline;

  esetcur(editor,index,cur);
}

ccinle char
ecurchr(eeditor_t *wdg, int index, int off)
{
  return egetchr(wdg,ecurloc(wdg,index) + off);
}

erect_t
ecurrec(
  eeditor_t *wdg, int index, erect_t rect)
{
  ecursor_t cur = egetcur(wdg,index);
  ecurrow_t row = egetrow(wdg,cur.yline);

  /* todo: I'd probably want to support having different sized lines */
  float y = (cur.yline - wdg->lyview) * wdg->font.vline;
  float x = row.indent * 16 * 2 +
    egetinf(wdg,index) + efont_code_xoff(wdg->font,ecurchr(wdg,index,0));


  int cur_xsize = rxmaxi(
    efont_code_width(wdg->font,'.'),
    efont_code_width(wdg->font,ecurchr(wdg,index,0)));
  int cur_ysize = wdg->font.vsize;

  y += cur_ysize * .2;

  return erect_xywh(rect.x0+x,rect.y1-y-cur_ysize,cur_xsize,cur_ysize);
}

ecursor_t
emovcurx(
  eeditor_t *editor, int cursor, int mov)
{
  ecursor_t cur = egetcur(editor,cursor);

  if(cur.xchar + mov > erowlen(editor,cur.yline))
  { if(cur.yline + 1 <= ccarrlen(editor->lcache) - 1)
    { cur.yline += 1;
      cur.xchar  = 0;
    }
  } else
  if(cur.xchar + mov < 0)
  { if(cur.yline - 1 >= 0)
    { cur.yline -= 1;
      cur.xchar  = erowlen(editor,cur.yline);
    }
  } else
  {
    cur.xchar += mov;
  }

  esetcur(editor,cursor,cur);

  return cur;
}

void
emovcury(
  eeditor_t *editor, int index, int mov)
{
  esetcury(editor,index,egetcury(editor,index)+mov);
}

void
eaddchr_(
  eeditor_t *wdg, int cursor, int length)
{
  ecursor_t cur = egetcur(wdg,cursor);

  // if(cur.event.type != kCHAR)
  {
    /* if there's no event do not store */
    if(wdg->event.type != kNONE)
    {
      *ccarradd(wdg->trail,1) = wdg->event;
    }

    wdg->event.type   = kCHAR;
    wdg->event.cursor = egetcur(wdg,cursor);
    wdg->event.length = 0;

  }/* then */

  wdg->event.length += length;
}

int
eaddchr(
  eeditor_t *wdg, int cursor, int chr)
{
  ccassert(
    chr != '\r' &&
    chr != '\n' );

  int mov = 1;
  int end = 0;

  /* todo: remove these syntax sensitive controls from here */
  if(chr >= 0x80)
  {
    chr = '?';
    end =   0;
    mov =   1;
  } else
  if(chr == '\t')
  {
    chr = ' ';
    end = ' ';
    mov =   2;
  } else
  switch(chr)
  {
    case '{': end = '}'; break;
    case '[': end = ']'; break;
    case '(': end = ')'; break;
    /* if the cursor is already hovering over any of these
     and the user types one of them, simply move the cursor
     to the right */
    case '}':
    case ']':
    case ')':
    if(ecurchr(wdg,cursor,0) == chr)
      return mov;
  }

  int num = end!=0?2:1;

  char *ptr = ebuffer_insert(&wdg->buffer,ecurloc(wdg,cursor),num);

  ptr[0] = chr;

  if(end != 0)
    ptr[1] = end;

  erecache(wdg);

  /* notify of the event and then move the cursor */
  eaddchr_(wdg,cursor,num);
  emovcurx(wdg,cursor,num);

  return mov;
}

void
enewline(
  eeditor_t *editor, int index)
{
  int num = 1;

  if(ecurchr(editor,index,-1) == '{')
  {
    num += 1;
  }

  char *ptr;
  ptr = ebuffer_insert(&editor->buffer,ecurloc(editor,index),num*2);

  while(num -- != 0)
  { /* proper line end feed */
    ptr[num*2+0] = '\r';
    ptr[num*2+1] = '\n';
  }

  // eaddrow(editor,egetcury(editor,index),num);

  /* remove */
  erecache(editor);

  eaddchr_(editor,index,num*2);
  emovcury(editor,index,    1);
  esetcurx(editor,index,    0);
}

/* delete the character or characters at the cursor's position */
void
edelchr(
  eeditor_t *wdg, int cursor, int length)
{
  int offset = ecurloc(wdg,cursor);

  if( wdg->buffer.length < offset + length )
  {
    return;
  }

  char *ptr = egetptr(wdg,offset);

  /* todo: instead just check if you're at the begging of a line in which case you just delete
    it, the problem is that the user is expected to move the cursor beforehand, so
    in which case it might by simpler to have two separate delete functions? */
  if((ptr[0] == '\r') &&
     (ptr[1] == '\n')) length += 2;

  ebuffer_remove(&wdg->buffer,offset,length);

  /* remove */
  erecache(wdg);
}

void
eeditor_load(
  eeditor_t *editor, char const *name)
{
  if(name != 0 && strlen(name) != 0)
  {
    void *file = ccopenfile(name,"r");
    ccu32_t length = 0;
    void *memory = ccpullfile(file,0,&length);

    if(memory != 0)
    {
      ebuffer_uninit(&editor->buffer);
      ebuffer_init(&editor->buffer,name,length);

      memcpy(editor->buffer.memory,memory,length);

      erecache(editor);
    }
  }
}

/* todo: this shouldn't be platform specific */
int
eeditor_unload(
  eeditor_t *editor, char const *name)
{
  int result = ccfalse;

  /* todo: this should be safer */
  if((name != 0) && (strlen(name) != 0))
  {
    HANDLE file = CreateFileA(name,
      GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,0x00,CREATE_ALWAYS,0x00,0x00);

    if(file != INVALID_HANDLE_VALUE)
    {
      DWORD length = 0;

      /* todo: convert indentations to white space here? */
      if(WriteFile(file,
          editor->buffer.memory,
          editor->buffer.length,&length,0x00))
      {
        result = length == editor->buffer.length;
      }

      CloseHandle(file);
    }
  }


  return result;
}


void
eeditor_msg(
  eeditor_t *editor)
{
  if(rxisctrl() && rxtstkey('Z'))
  {
    eevent_t event = epopevn(editor);

    if(event.type == kCHAR)
    {
      esetcur(editor,0,event.cursor);
      edelchr(editor,0,event.length);
    }

  } else
  if(rxtstkey(rx_kESCAPE))
  {
    /* todo */
    ccdlb_t *dlb = ccdlb(editor->cursor);
    dlb->sze_min = 1;

  } else
  if(rx.wnd.in.mice.yscroll != 0)
  {
    /* scroll up */
    editor->lyview += rxisshft() ? 16 : - rx.wnd.in.mice.yscroll;
    editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
  } else
  if(rxtstkey(rx_kHOME))
  {
    /* move to the start of the line */
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
      esetcurx(editor,i,0);

  } else
  if(rxtstkey(rx_kEND))
  {
    /* move to the end of the line */
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
      esetcurx(editor,i,erowlen(editor,egetcury(editor,i)));

  } else
  if(rxisctrl() && rxtstkey('X'))
  {
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
    {
      ecurrow_t row = egetrow(editor,egetcury(editor,i));

      int num = 1;

      char *ptr = egetptr(editor,row.offset+row.length);
      if((ptr[0]=='\r') &&
         (ptr[1]=='\n')) num += 1;

      ebuffer_remove(&editor->buffer,row.offset,row.length+num);
      erecache(editor);
    }

  } else
  if(rxtstkey(rx_kKEY_UP))
  {
    if(rxisctrl() && rxismenu())
    {
      ecursor_t cur = egetcur(editor,0);
      cur.yline -= 1;
      eaddcur(editor,cur);
    } else
    if(rxisctrl())
    {
      /* scroll up */
      editor->lyview -= rxisshft() ? 16 : 1;
      editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
    } else
    {
      /* move to the line above */
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcury(editor,i,-1);
    }
  } else
  if(rxtstkey(rx_kKEY_DOWN))
  {
    if(rxisctrl() && rxismenu())
    {
      ecursor_t cur = egetcur(editor,ecurnum(editor)-1);
      cur.yline += 1;
      eaddcur(editor,cur);
    } else
    if(rxisctrl())
    {
      /* scroll down */
      editor->lyview += rxisshft() ? 16 : 1;
      editor->lyview = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
    } else
    {
      /* move to the line below */
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcury(editor,i,+1);
    }
  } else
  if(rxtstkey(rx_kKEY_LEFT))
  { if(rxisctrl())
    {
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
      {
        if(is_word_delim(ecurchr(editor,i,-1)))
        { while(ecurchr(editor,i,-1)!=0 && is_word_delim(ecurchr(editor,i,-1)))
            emovcurx(editor,i,-1);
        } else
        { while(ecurchr(editor,i,-1)!=0 && !is_word_delim(ecurchr(editor,i,-1)))
            emovcurx(editor,i,-1);
        }
      }
    } else
    {
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcurx(editor,i,-1);
    }
  } else
  if(rxtstkey(rx_kKEY_RIGHT))
  {
    if(rxisctrl())
    {
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
      {
        if(is_word_delim(ecurchr(editor,i,0)))
        { while(ecurchr(editor,i,0)!=0 && is_word_delim(ecurchr(editor,i,0)))
            emovcurx(editor,i,+1);
        } else
        { while(ecurchr(editor,i,0)!=0 && !is_word_delim(ecurchr(editor,i,0)))
            emovcurx(editor,i,+1);
        }
      }
    } else
    {
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcurx(editor,i,+1);
    }
  } else
  if(rxtstkey(rx_kDELETE))
  {
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
      edelchr(editor,i,1);

  } else
  if(rxtstkey(rx_kBCKSPC))
  {
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
    {
      if(ecurloc(editor,i) != 0)
      {
        emovcurx(editor,i,-1);
        edelchr(editor,i,1);
      }
    }
  } else
  if(rxtstkey(rx_kRETURN))
  {
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
      enewline(editor,i);
  } else
  { if(rxisctrl())
    {

    } else
    {
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
      { if(rxchr() != 0)
        {
          eaddchr(editor,i,rxchr());
        }
      }
    }
  }
}

int
eeditor_draw_text_run_callback(
  void *user, int index, int *code, rxcolor_t *color)
{
  eeditor_t *editor = user;

  ecurrow_t line = egetrow(editor,editor->style.yline);

  char *cursor = editor->buffer.memory;
  cursor += line.offset;
  cursor += index;

  *code  = *cursor;
  *color = editor->style.color;

  /* silly */
  if(editor->style.reach <= 0)
  {
    int token = esyntax_get_token_style(&editor->syntax,
      (editor->buffer.length) - (cursor - editor->buffer.memory),cursor,
        &editor->style.reach);

    *color = RX_RGBA_UNORM(221,206,150,255);

    switch(token)
    {
      case ETOKEN_kCOMMENT:
      {
        *color = RX_COLOR_SILVER;
      } break;
      case ETOKEN_kSTRING:
      { *color = RX_COLOR_TEAL;
      } break;
      case ETOKEN_kINTEGER:
      case ETOKEN_kFLOAT:
      { *color = RX_COLOR_CYAN;
      } break;
      case ETOKEN_kIF:
      case ETOKEN_kELSE:
      case ETOKEN_kSWITCH:
      case ETOKEN_kCASE:
      case ETOKEN_kDEFAULT:
      case ETOKEN_kFOR:
      case ETOKEN_kWHILE:
      case ETOKEN_kDO:
      case ETOKEN_kGOTO:
      case ETOKEN_kRETURN:
      case ETOKEN_kBREAK:
      case ETOKEN_kCONTINUE:
      case ETOKEN_kVOID:
      {
        *color = RX_COLOR_RED;
      } break;
      case ETOKEN_kSTDC_INT:
      case ETOKEN_kSTDC_LONG:
      case ETOKEN_kSTDC_SHORT:
      case ETOKEN_kSTDC_DOUBLE:
      case ETOKEN_kSTDC_FLOAT:
      case ETOKEN_kSTDC_CHAR:
      case ETOKEN_kSTDC_BOOL:
      case ETOKEN_kSTDC_SIGNED:
      case ETOKEN_kSTDC_UNSIGNED:
      case ETOKEN_kMSVC_INT8:
      case ETOKEN_kMSVC_INT16:
      case ETOKEN_kMSVC_INT32:
      case ETOKEN_kMSVC_INT64:
      case ETOKEN_kENUM:
      case ETOKEN_kSTRUCT:

      case ETOKEN_kTYPEDEF:
      case ETOKEN_kAUTO:
      case ETOKEN_kEXTERN:
      case ETOKEN_kREGISTER:
      case ETOKEN_kSTATIC:
      case ETOKEN_kTHREAD_LOCAL:
      case ETOKEN_kMSVC_DECLSPEC:
      {
        *color = RX_COLOR_MAGENTA;
      } break;

    }

    editor->style.color = *color;
  }

  editor->style.reach -= 1;

  return index < line.length;

}


