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
eaddcur(
  eeditor_t *editor)
{
  ecursor_t *cur = ccarradd(editor->cursor,1);
  cur->select = 0;
  cur->selmin = 0;
  cur->selmax = 0;
  cur->xchar  = 0;
  cur->yline  = 0;

  return ccarrlen(editor->cursor) - 1;
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
  eeditor_t *wdg, int index)
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
  eeditor_t *wdg, int index)
{
  return egetrow(wdg,index).length;
}

int
egetoff(
  eeditor_t *wdg, int index)
{
  return egetrow(wdg,index).offset;
}

ecursor_t
egetcur(
  eeditor_t *editor, int index)
{
  return editor->cursor[index];
}

int
egetcurx(
  eeditor_t *editor, int index)
{
  return egetcur(editor,index).xchar;
}
int
egetcury(
  eeditor_t *editor, int index)
{
  return egetcur(editor,index).yline;
}

/* todo: check for all the cursors with the same coordinates and join them */
void
esetcur(
  eeditor_t *editor, int index, ecursor_t cur)
{
  int yline = rxclamp(cur.yline,0,ccarrlen(editor->lcache)-1);
  int xchar = rxclamp(cur.xchar,0,egetlen(editor,cur.yline));

  editor->cursor[index].yline = yline;
  editor->cursor[index].xchar = xchar;
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

int
ecurloc(
  eeditor_t *editor, int index)
{
  int loc = egetoff(editor,egetcury(editor,index)) + egetcurx(editor,index);
  return loc;
}

char
ecurchr(eeditor_t *wdg, int index, int off)
{
  int loc = ecurloc(wdg,index);

  if(CCWITHIN(loc+off,0,wdg->buffer.length-1))
  {
    return wdg->buffer.memory[loc+off];
  }

  return 0;
}

char *
egetptr(eeditor_t *wdg, int index)
{
  return &wdg->buffer.memory[index];
}

char *
egetend(eeditor_t *editor)
{
  return &editor->buffer.memory[editor->buffer.length-1];
}

char *
ecurptr(
  eeditor_t *editor, int index)
{
  return egetptr(editor,ecurloc(editor,index));
}

erect_t
ecurrec(
  eeditor_t *widget, int index, erect_t rect)
{

  ecursor_t cur = egetcur(widget,index);
  int xcur = cur.xchar;
  int ycur = cur.yline;
  erow_t row = egetrow(widget,ycur);

  int y = ycur * 32 - widget->lyview * 32;
  int x = row.indent * 16 * 2 + xcur * rxchrxsz(32);

  return rect_by_size(rect.x0+x,rect.y1-y-32,16,32);
}

void
emovcurx(
  eeditor_t *editor, int index, int mov)
{
  ecursor_t cur = egetcur(editor,index);

  if(cur.xchar + mov > egetlen(editor,cur.yline))
  {
    if(cur.yline + 1 <= ccarrlen(editor->lcache) - 1)
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
      cur.xchar  = egetlen(editor,cur.yline);
    }
  } else
  {
    cur.xchar += mov;
  }

  esetcur(editor,index,cur);
}

void
emovcury(
  eeditor_t *editor, int index, int mov)
{
  esetcury(editor,index,egetcury(editor,index)+mov);
}

void
enewline(
  eeditor_t *editor, int index)
{

  int num = 1;

  if(ecurchr(editor,index,-1) == '{')
    num += 1;

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

  emovcury(editor,index,1);
  esetcurx(editor,index,0);
}

int
eputchar(
  eeditor_t *editor, int index, int chr)
{
  ccassert(
    chr != '\r' &&
    chr != '\n' );

  int mov = 1;
  int end = 0;

  /* todo: remove these conversions from here? */
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
    case '}':
    case ']':
    case ')':
      if(ecurchr(editor,index,0) == chr)
        return mov;
  }

  char *ptr =
    ebuffer_insert(&editor->buffer,
      ecurloc(editor,index),end!=0?2:1);

  ptr[0] = chr;

  if(end != 0)
    ptr[1] = end;

  erecache(editor);

  return mov;
}

/* delete the character or characters at the cursor's position */
void
edelchar(
  eeditor_t *editor, int index)
{
  if(editor->buffer.length <= 0)
  {
    return;
  }

  char *ptr = ecurptr(editor,index);
  /* instead just check if you're at the begging of a line in which case you just delete
    it, the problem is that the user is expected to move the cursor beforehand, so
    in which case it might by simpler to have to separate delete functions */
  int num = 1;
  if((ptr[0] == '\r') &&
     (ptr[1] == '\n')) num += 1;

  ebuffer_delete(&editor->buffer,ecurloc(editor,index),num);

  /* remove */
  erecache(editor);
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
  if(rxtstkey(rx_kMVWHEEL))
  {
    /* scroll up */
    editor->lyview += rxisshft() ? 16 : - rx.yscroll;
    editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
  } else
  if(rxtstkey(rx_kHOME))
  {
    /* move to the start of the line */
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
      esetcurx(editor,i,0);

  } else
  if(rxtstkey(rx_kEND))
  {
    /* move to the end of the line */
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
      esetcurx(editor,i,egetlen(editor,egetcury(editor,i)));

  } else
  if(rxisctrl() && rxtstkey('X'))
  {
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
    {
      erow_t row = egetrow(editor,egetcury(editor,i));

      int num = 1;

      char *ptr = egetptr(editor,row.offset+row.length);
      if((ptr[0]=='\r') &&
         (ptr[1]=='\n')) num += 1;

      ebuffer_delete(&editor->buffer,row.offset,row.length+num);
      erecache(editor);
    }

  } else
  if(rxtstkey(rx_kKEY_UP))
  {
    if(rxisctrl() && rxismenu())
    {
      ecursor_t cur = egetcur(editor,0);
      cur.yline -= 1;
      esetcur(editor,eaddcur(editor),cur);
    } else
    if(rxisctrl())
    {
      /* scroll up */
      editor->lyview -= rxisshft() ? 16 : 1;
      editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
    } else
    {
      /* move to the line above */
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
        emovcury(editor,i,-1);
    }
  } else
  if(rxtstkey(rx_kKEY_DOWN))
  {
    if(rxisctrl() && rxismenu())
    {
      ecursor_t cur = egetcur(editor,ccarrlen(editor->cursor)-1);
      cur.yline += 1;
      esetcur(editor,eaddcur(editor),cur);
    } else
    if(rxisctrl())
    {
      /* scroll down */
      editor->lyview += rxisshft() ? 16 : 1;
      editor->lyview = rxclampi(editor->lyview,0,ccarrlen(editor->lcache)-1);
    } else
    {
      /* move to the line below */
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
        emovcury(editor,i,+1);
    }
  } else
  if(rxtstkey(rx_kKEY_LEFT))
  { if(rxisctrl())
    {
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
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
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
        emovcurx(editor,i,-1);
    }
  } else
  if(rxtstkey(rx_kKEY_RIGHT))
  {
    if(rxisctrl())
    {
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
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
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
        emovcurx(editor,i,+1);
    }
  } else
  if(rxtstkey(rx_kDELETE))
  {
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
      if(ecurloc(editor,i) < editor->buffer.length)
        edelchar(editor,i);
  } else
  if(rxtstkey(rx_kBCKSPC))
  {
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
    {
      if(ecurloc(editor,i) != 0)
      { emovcurx(editor,i, -1);
        edelchar(editor,i    );
      }
    }
  } else
  if(rxtstkey(rx_kRETURN))
  {
    for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
      enewline(editor,i);
  } else
  { if(rxisctrl())
    {

    } else
    {
      for(int i=ccarrlen(editor->cursor)-1;i>=0;i-=1)
      {
        if(rxchr() != 0)
        {
          emovcurx(editor,i,eputchar(editor,i,rxchr()));
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

  erow_t line = egetrow(editor,editor->style.yline);

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


