/*
**
**                      -+- rx -+-
**
**                 Mundane Graphics API.
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**        NOT PRODUCTION READY/NOT PRODUCTION QUALITY
**
**
**               HONI SOIT QUI MAL Y PENSE
**
**
**                 github.com/MicroRJ/rx
**
*/

/* this widget only works with mono spaced fonts, can't bother right now */
typedef struct cursor_t
{
  unsigned select: 1;
  int      selmin;
  int      selmax;
  // int      cursor;
  int      xchar;
  int      yline;
  int      xpixel;
  int      ypixel;
} cursor_t;

typedef struct line_t
{
  int offset;
  int length;
} line_t;

typedef struct textbox_widget_t
{
  /* base widget */
  int        focused;

  /* theme */
  rxcolor_t    color;

  char const  *label;

  cursor_t    cursor;

  line_t     *lcache;

  char       *buffer;
  int         length;
} textbox_widget_t;

void
lcache_init(
  textbox_widget_t *widget, int length, char const *buffer)
{
  char const *cursor = buffer;

  ccarrdel(widget->lcache);
  widget->lcache = 0;

retry:
  if(cursor < buffer + length)
  {
    if(ccarrlen(widget->lcache) >= 64)
    {
      return;
    }
    line_t *line = ccarradd(widget->lcache,1);
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

void
cancel_selection(
  textbox_widget_t *widget)
{
  widget->cursor.select = ccfalse;
  widget->cursor.selmin = -1;
  widget->cursor.selmax = -1;
}

int
is_selection(
  textbox_widget_t *widget)
{
  return widget->cursor.select;
}

int
buflen(
  textbox_widget_t *widget)
{
  return widget->length;
}


line_t *
getrow(
  textbox_widget_t *widget, int index)
{
  return &widget->lcache[index];
}

line_t *
currow(
  textbox_widget_t *widget)
{
  return getrow(widget,widget->cursor.yline);
}

int
curloc(
  textbox_widget_t *widget)
{
  return currow(widget)->offset + widget->cursor.xchar;
}

int
maymov(
  textbox_widget_t *widget, int xmov, int ymov)
{
  cursor_t cursor = widget->cursor;

  int ymax = ccarrlen(widget->lcache) - 1;
  if((cursor.yline + ymov <= ymax) &&
     (cursor.yline + ymov >=    0))
  {
    int xmax = getrow(widget,cursor.yline+ymov)->length;
    return
        ( (cursor.xchar + xmov <= xmax) &&
          (cursor.xchar + xmov >=    0) );
  }

  return 0;
}

int
movcur(
  textbox_widget_t *widget, int xmov, int ymov)
{
  if(maymov(widget,xmov,ymov))
  {
    widget->cursor.yline += ymov;
    widget->cursor.xchar += xmov;

    widget->cursor.ypixel = -32 * (1 + widget->cursor.yline);
    widget->cursor.xpixel =   rxcp(32, widget->cursor.xchar);

    return 1;
  } else
  {
    return 0;
  }
}


void
addchr(
  textbox_widget_t *widget, int chr)
{
  ccarradd(widget->buffer,1);
  widget->length += 1;

  int loc = curloc(widget);
  char *mem = &widget->buffer[loc];

  int mov = ccarrlen(widget->buffer) - loc - 1;
  memmove(mem+1,mem,mov);

  *mem = chr;

  /* remove */
  lcache_init(widget,widget->length,widget->buffer);

  movcur(widget,1,0);
}

void
delchr(
  textbox_widget_t *widget)
{
  if(movcur(widget,-1,0))
  {
    int loc = curloc(widget);

    char *mem = &widget->buffer[loc];
    *mem = 0;

    int mov = buflen(widget) - loc - 1;
    memmove(mem,mem+1,mov);

    currow(widget)->length -= 1;

    widget->length -= 1;

    // /* remove */
    // lcache_init(widget,ccarrlen(widget->buffer),widget->buffer);
  }

}

void
fload(
  textbox_widget_t *widget, char const *name)
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

      lcache_init(widget,length,buffer);
      movcur(widget,0,0);
    }
  }
}


void
fsave(
  textbox_widget_t *widget, char const *name)
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
textbox_widget(rect_t rect, textbox_widget_t *widget)
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
    { movcur(widget,0,-1);
    } else
    if(rxtstkey(rx_kKEY_DOWN))
    { movcur(widget,0,+1);
    } else
    if(rxtstkey(rx_kKEY_LEFT))
    { movcur(widget,-1,0);
    } else
    if(rxtstkey(rx_kKEY_RIGHT))
    { movcur(widget,+1,0);
    } else
    if(rxtstkey(rx_kBCKSPC))
    {
      delchr(widget);
    } else
    if(rxtstkey(rx_kRETURN))
    {
      addchr(widget,'\n');
    } else
    {
      int chr = rxchr();
      if(chr != 0)
      {
        addchr(widget,chr);
      }
    }

  }


  /* drawing */
  // set_clip_rect(rect);

  rxcolor_t color = BACKGROUND_COLOR;

  draw_rect(rect,color);

  line_t *line;
  ccarrfor(widget->lcache,line)
  {
    // rxdraw_text_ex(FOREGROUND_COLOR,
    //   gutter_rect.x0 +       0,
    //   gutter_rect.y1 +    - 32 * (1 + line - widget->lcache),32,-1,ccformat("%i",line-widget->lcache));

    rxdraw_text_ex(FOREGROUND_COLOR,
      rect.x0 +       0,
      rect.y1 +    - 32 * (1 + line - widget->lcache),32,
        line->length,widget->buffer+line->offset);
  }


  /* draw the selection */
  if(is_selection(widget))
  {
    int smin = rxcp(32,widget->cursor.selmin);
    int smax = rxcp(32,widget->cursor.selmax);

    rect_t srec = rect_by_xyxy(
      rect.x0+smin, rect.y0,
      rect.x0+smax, rect.y1 );

    color   = RX_COLOR_GREEN;
    color.a = .5;
    draw_rect(srec,color);
  }

  rect_t cursor_rect = rect_by_size(
    rect.x0 + widget->cursor.xpixel,
    rect.y1 + widget->cursor.ypixel, 16,32);

  color = RX_COLOR_GREEN;
  color.a = (.5+sin(2 * rx.total_seconds * rxPI_F))*.5;

  draw_rect(cursor_rect,color);

  set_clip_rect(RECT_HUGE);
  return ccfalse;
}



  // line_t *line = widget->lcache + widget->cursor.yline;
  // if( (widget->cursor.yline + ymov < -1 + ccarrlen(widget->lcache)) &&
  //     (widget->cursor.yline + ymov >  0) )
  // {
  //   widget->cursor.yline += ymov;

  //   if(widget->cursor.xchar + xmov >= line->length)
  //   {
  //     if(widget->cursor.yline < -1 + ccarrlen(widget->lcache))
  //     {
  //       widget->cursor.yline += 1;
  //       widget->cursor.xchar  = 0;

  //       // widget->cursor.cursor += line->length + 1; // \r\n
  //     }
  //   } else
  //   if(widget->cursor.xchar + xmov < 0)
  //   {
  //     if(widget->cursor.yline > 0)
  //     {
  //       widget->cursor.yline -= 1;
  //       widget->cursor.xchar  = line[-1].length - 1;

  //       // widget->cursor.cursor -= line->length + 1; // \r\n
  //     }
  //   } else
  //   {
  //     widget->cursor.xchar += xmov;
  //   }
  // }



  // if(rx.is_shft)
  // {
  //   if(widget->cursor.select != cctrue)
  //   {
  //     widget->cursor.selmin = widget->cursor.cursor;
  //     widget->cursor.selmax = widget->cursor.cursor;
  //     widget->cursor.select = cctrue;
  //   }
  // } else
  // {
  //   widget->cursor.select = ccfalse;
  // }


  // if(rx.is_shft)
  // {
  //   if(widget->cursor.select != ccfalse)
  //   {
  //     widget->cursor.selmax = widget->cursor.cursor;
  //   }
  // }