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
  *rlArray_add(editor->cursor,1) = cur;
  *rlArray_add(editor->curinf,1) = 0;

  esrtcur(editor);
  return efndcur(editor,cur.xchar,cur.yline);
}

ccinle int
ecurloc(
  eeditor_t *wdg, int index)
{
  return ebuffer_get_line_offset(&wdg->buffer,egetcury(wdg,index)) + egetcurx(wdg,index);
}

ccinle char *
egetptr(eeditor_t *wdg, int index)
{
  /* todo: this should point somewhere safer */
  ccglobal char p;

  p = 0; /* in case it was overwritten */

  if(index >= 0 && index < wdg->buffer.length)
  {
    return &wdg->buffer.string[index];
  }

  return &p;
}

ccinle char
egetchr(eeditor_t *wdg, int offset)
{
  return *egetptr(wdg,offset);
}

ccinle char *
egetptr2(
  eeditor_t *wdg, int xchar, int yline)
{
  return egetptr(wdg,ebuffer_get_line_offset(&wdg->buffer,yline)+xchar);
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
  cur.yline = rxclamp(cur.yline,0,
    ccarrlen(editor->buffer.lcache)-1);
  cur.xchar = rxclamp(cur.xchar,0,
    ebuffer_get_line_length(&editor->buffer,cur.yline));

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

rlBOX
ecurrec(
  eeditor_t *wdg, int index, rlBOX rect)
{

  ecursor_t cur = egetcur(wdg,index);
  emarker_t row = Emu_buffer_get_line_at_index(&wdg->buffer,cur.yline);

  int curchr = ecurchr(wdg,index,0);

  float y = (cur.yline - wdg->lyview) * wdg->font->line_height;
  float x = egetinf(wdg,index);

  /* #todo */
  int cur_xsize = rxmaxi(
    efont_code_width(wdg->font,'.'),
    efont_code_width(wdg->font,curchr));

  y += wdg->font->char_height * .2;

  float cursor_height = wdg->font->char_height * 1.2;

  return rlMakeBoxBySize(rect.x0+x,rect.y1-y-wdg->font->line_height,cur_xsize,cursor_height);
}

ecursor_t
emovcurx(
  eeditor_t *editor, int cursor, int mov)
{
  ecursor_t cur = egetcur(editor,cursor);

  if(cur.xchar + mov > ebuffer_get_line_length(&editor->buffer,cur.yline))
  { if(cur.yline + 1 <= ccarrlen(editor->buffer.lcache) - 1)
    { cur.yline += 1;
      cur.xchar  = 0;
    }
  } else
  if(cur.xchar + mov < 0)
  { if(cur.yline - 1 >= 0)
    { cur.yline -= 1;
      cur.xchar  = ebuffer_get_line_length(&editor->buffer,cur.yline);
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
    if(wdg->event.type != EDITOR_kNONE)
    {
      *rlArray_add(wdg->trail,1) = wdg->event;
    }

    wdg->event.type   = EDITOR_kCHAR;
    wdg->event.cursor = egetcur(wdg,cursor);
    wdg->event.length = 0;

  }/* then */

  wdg->event.length += length;
}


void
ekey_one(
  eeditor_t *wdg, int key, int num, int chr, int cur);

void
ekey_all(
  eeditor_t *wdg, int key, int num, int chr)
{
  for(int cur=ecurnum(wdg)-1; cur>=0; cur-=1)
  {
    ekey_one(wdg,key,num,chr,cur);
  }
}

void
ekey_one(
  eeditor_t *wdg, int key, int num, int chr, int cur)
{

  ecursor_t cursor = egetcur(wdg,cur);
  int off = ecurloc(wdg,cur);
  int dir = 0;
  switch(key)
  { case EDITOR_kMOD_CONTROL:
    { wdg->is_control = chr != 0;
    } break;
    case EDITOR_kMOD_SHIFT:
    { wdg->is_shift = chr != 0;
    } break;
    case EDITOR_kMOD_ALT:
    { wdg->is_alt = chr != 0;
    } break;
    case EDITOR_kMOD_INSERT:
    { wdg->is_insert = chr != 0;
    } break;
    case EDITOR_kMOVE_LEFT:
    case EDITOR_kMOVE_RIGHT:
    {
      wdg->cursor_blink_timer = 0;

      dir = +1;

      if(key == EDITOR_kMOVE_LEFT)
      {
        off += (dir = -1);
      }

      if(wdg->is_control)
      {
        if(IS_WORD_DELIMETER(egetchr(wdg,off)))
        { while(egetchr(wdg,off)!=0 && IS_WORD_DELIMETER(egetchr(wdg,off)))
          {
            emovcurx(wdg,cur,dir); off += dir;
          }
        } else
        { while(egetchr(wdg,off)!=0 && !IS_WORD_DELIMETER(egetchr(wdg,off)))
          {
            emovcurx(wdg,cur,dir); off += dir;
          }
        }
      } else
      {
        emovcurx(wdg,cur,dir);
      }
    } break;
    case EDITOR_kMOVE_LINE_RIGHT:
    {
      esetcurx(wdg,cur,ebuffer_get_line_length(&wdg->buffer,cursor.yline));
    } break;
    case EDITOR_kMOVE_LINE_LEFT:
    {
      esetcurx(wdg,cur,0);
    } break;
    case EDITOR_kDELETE_BACK:
      off += (dir = - 1);
    /* fall-through */
    case EDITOR_kDELETE_HERE:
    if( off >= 0 && off + num <= wdg->buffer.length)
    {
      wdg->cursor_blink_timer = 0;

      char *ptr = egetptr(wdg,off);

      /* delete the entire line feed, as supposed to just breaking it */
      if((ptr[0 + dir] == '\r') &&
         (ptr[1 + dir] == '\n'))
      {
        off += dir;
        num += 1;
      }

      /* gotta move the cursor first */
      emovcurx(wdg,cur,dir*num);

      Emu_buffer_remove(&wdg->buffer,off,num);
      Emu_buffer_rescan_lines(&wdg->buffer);

    } break;
    case EDITOR_kCHAR:
    {
      wdg->cursor_blink_timer = 0;

      /* we can handle this properly instead #todo */
      ccassert(
        chr != '\r' &&
        chr != '\n' );

      int mov = 1;
      int end = 0;

      if(chr >= 0x80)
      { chr = '?';
        end =   0;
        mov =   1;
      } else
      /* this should be based on the user's setttings, whether the user
        wants to convert tabs to spaces #todo*/
      if(chr == '\t')
      { chr = ' ';
        end = ' ';
        mov =   2;
      } else
      /* remove these syntax sensitive controls from here #todo */
      switch(chr)
      {
        case '{': end = '}'; break;
        case '[': end = ']'; break;
        case '(': end = ')'; break;
        /* if the cursor is already hovering over any of these
         and the user types one of them, simply move the cursor to the right */
        case '}':
        case ']':
        case ')':
        if(egetchr(wdg,off) == chr)
        {
          mov = 1;
          chr = 0;
          end = 0;
        } break;
      }

      if(chr != 0)
      {
        num = end!=0?2:1;

        char *ptr = Emu_buffer_insert(&wdg->buffer,off,num);
        ptr[0] = chr;

        if(end != 0)
          ptr[1] = end;

        /* notify of the event and then move the index */
        eaddchr_(wdg,off,num);
        Emu_buffer_rescan_lines(&wdg->buffer);
      }

      emovcurx(wdg,cur,mov);
    } break;
    case EDITOR_kLINE:
    {
      /* this is syntax specific #todo */
      if(egetchr(wdg,off-1) == '{')
        num += 1;

      char *ptr = Emu_buffer_insert(&wdg->buffer,off,num*2);

      /* proper line end feed #todo */
      while(num -- != 0)
      {
        ptr[num*2+0] = '\r';
        ptr[num*2+1] = '\n';
      }

      // eaddrow(wdg,egetcury(wdg,index),num);

      eaddchr_(wdg,cur,num*2);
      emovcury(wdg,cur,    1);
      esetcurx(wdg,cur,    0);

      Emu_buffer_rescan_lines(&wdg->buffer);
    } break;
  }
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
      Emu_buffer_uinit(&editor->buffer);
      Emu_buffer_init(&editor->buffer,name,length);

      memcpy(editor->buffer.string,memory,length);

      Emu_buffer_rescan_lines(&editor->buffer);
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
          editor->buffer.string,
          editor->buffer.length,&length,0x00))
      {
        result = length == editor->buffer.length;
      }

      CloseHandle(file);
    }
  }


  return result;
}

/* remove from here #todo */
void
eeditor_msg(
  eeditor_t *editor)
{
  ekey_one(editor,EDITOR_kMOD_CONTROL,0,rlIO_testCtrlKey(),0);
  ekey_one(editor,EDITOR_kMOD_ALT,0,rlIO_testMenuKey(),0);
  ekey_one(editor,EDITOR_kMOD_SHIFT,0,rlIO_testShiftKey(),0);

  if(IS_CLICK_ENTER(0))
  {
   int xcursor = + rx.wnd.in.mice.xcursor;
   int ycursor = - rx.wnd.in.mice.ycursor + rx.wnd.size_y;

   int yline = editor->lyview + ycursor / editor->font->line_height;

   emarker_t line = Emu_buffer_get_line_at_index(&editor->buffer,yline);
   char *string = editor->buffer.string + line.offset;

   float xwalk = 0;
   for(int xchar=0; xchar<line.length; xchar+=1)
   {
      float width = efont_code_xadv(editor->font,string[xchar]);

      if(xcursor >= xwalk && xcursor <= xwalk + width)
      {
         esetcur(editor,0,(ecursor_t){xchar,yline});
         break;
      }

      xwalk += width;
   }

  } else
  if(rlIO_testCtrlKey() && rlIO_testKey('Z'))
  {
    eevent_t event = epopevn(editor);

    if(event.type == EDITOR_kCHAR)
    {
      // esetcur(editor,0,event.cursor);
      // edelchr(editor,0,event.length);
      ekey_one(editor,EDITOR_kDELETE_BACK,event.length,0,0);
    }

  } else
  if(rlIO_testKey(rx_kESCAPE))
  {
    /* todo */
    ccdlb_t *dlb = ccdlb(editor->cursor);
    dlb->sze_min = 1;

  } else
  if(rx.wnd.in.mice.yscroll != 0)
  {
    /* scroll up */
    editor->lyview += rlIO_testShiftKey() ? 16 : - rx.wnd.in.mice.yscroll;
    editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->buffer.lcache)-1);
  } else
  if(rlIO_testKey(rx_kHOME))
  {
    ekey_all(editor,EDITOR_kMOVE_LINE_LEFT,1,0);

  } else
  if(rlIO_testKey(rx_kEND))
  {
    ekey_all(editor,EDITOR_kMOVE_LINE_RIGHT,1,0);
  } else
  if(rlIO_testCtrlKey() && rlIO_testKey('X'))
  {
    for(int i=ecurnum(editor)-1;i>=0;i-=1)
    {
      emarker_t row = Emu_buffer_get_line_at_index(&editor->buffer,egetcury(editor,i));

      int num = 1;

      char *ptr = egetptr(editor,row.offset+row.length);
      if((ptr[0]=='\r') &&
         (ptr[1]=='\n')) num += 1;

      Emu_buffer_remove(&editor->buffer,row.offset,row.length+num);
      Emu_buffer_rescan_lines(&editor->buffer);
    }

  } else
  if(rlIO_testKey(rx_kKEY_UP))
  {
    if(rlIO_testCtrlKey() && rlIO_testMenuKey())
    {
      ecursor_t cur = egetcur(editor,0);
      cur.yline -= 1;
      eaddcur(editor,cur);
    } else
    if(rlIO_testCtrlKey())
    {
      /* scroll up */
      editor->lyview -= rlIO_testShiftKey() ? 16 : 1;
      editor->lyview  = rxclampi(editor->lyview,0,ccarrlen(editor->buffer.lcache)-1);
    } else
    {
      /* move to the line above */
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcury(editor,i,-1);
    }
  } else
  if(rlIO_testKey(rx_kKEY_DOWN))
  {
    if(rlIO_testCtrlKey() && rlIO_testMenuKey())
    {
      ecursor_t cur = egetcur(editor,ecurnum(editor)-1);
      cur.yline += 1;
      eaddcur(editor,cur);
    } else
    if(rlIO_testCtrlKey())
    {
      /* scroll down */
      editor->lyview += rlIO_testShiftKey() ? 16 : 1;
      editor->lyview = rxclampi(editor->lyview,0,ccarrlen(editor->buffer.lcache)-1);
    } else
    {
      /* move to the line below */
      for(int i=ecurnum(editor)-1;i>=0;i-=1)
        emovcury(editor,i,+1);
    }
  } else
  if(rlIO_testKey(rx_kKEY_LEFT))
  { ekey_all(editor,EDITOR_kMOVE_LEFT,   1,0);
  } else
  if(rlIO_testKey(rx_kKEY_RIGHT))
  {
    ekey_all(editor,EDITOR_kMOVE_RIGHT, 1,0);
  } else
  if(rlIO_testKey(rx_kDELETE))
  { ekey_all(editor,EDITOR_kDELETE_HERE,1,0);
  } else
  if(rlIO_testKey(rx_kBCKSPC))
  { ekey_all(editor,EDITOR_kDELETE_BACK,1,0);
  } else
  if(rlIO_testKey(rx_kRETURN))
  {
    ekey_all(editor,EDITOR_kLINE,1,0);
  } else
  { if(!rlIO_testCtrlKey())
    {
      if(rxchr() != 0)
      {
        ekey_all(editor,EDITOR_kCHAR,1,rxchr());
      }
    }
  }
}