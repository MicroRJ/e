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
Editor_render(EEditor *lpEditor, boxthing bx) {

	rlFont_Face *lpFont = lpEditor->font;

	float blink_duration = lpEditor->cursor_blink_speed_in_seconds;
	if (blink_duration == 0) {
		blink_duration = .250;
	}
	float blink_timer = lpEditor->cursor_blink_timer;
	blink_timer += rx.delta_seconds;
	double animation = E_CURSOR_PHASE(lpEditor,blink_timer,blink_duration);

	lpEditor->cursor_blink_timer = blink_timer;

	/* todo */
	rlColor curcolor = E_CURSOR_COLOR(lpEditor);
	curcolor.a = clamp(animation,.1,.8);
	float lineHeight = lpFont->line_height;
	/* [[TODO]]: rename to "firstline" */
	__int32 yview = lpEditor->lyview;
	/* [[TODO]]: to become a pointer */
	EBuffer *lpBuffer = &lpEditor->buffer;
	emarker_t *lpLines = lpBuffer->lcache;
	char const *lpBufStr = lpBuffer->string;
	float spaceW = lpFont->spaceWidth;

	/* [[TODO]]: render only visible cursors */
	/* [[TODO]]: adapt the size of the cursor the size of the character */
	for (int i=0;i<enumcur(lpEditor);i+=1) {
		ECursor cur = egetcur(lpEditor,i);
		emarker_t line = lpLines[cur.yline];
		char const *lineStart = lpBufStr + line.offset;
		float cury = bx.y1 - ((cur.yline - yview + 1) * lineHeight + lpFont->char_height * .2);
		float curx = bx.x0 + rlFont_getWidth(lpFont,lineStart,cur.xchar);
		float curw = spaceW;
		float curh = lpFont->char_height * 1.2;
		rxvec2_t currad = rxvec2_xy(curw*.5,curh*.5);
		rxvec2_t curcen = rxvec2_xy(curx+currad.x,cury+currad.y);
		Emu_imp_rect_sdf(curcen,currad,curcolor,2.5,1.);
	}

	EBuffer buffer = lpEditor->buffer;

	rxTTF_DRAW config;
	ZeroMemory(&config,sizeof(config));
	config.font = lpFont;
	config.x = bx.x0;
	config.y = bx.y1 - lpFont->line_height;
	config.char_height = lpFont->char_height;
	config.line_height = lpFont->line_height;
   config.tab_size = 3; /* in spaces */
	config.color = rxColor_RGBA(1,1,1,.8);
	config.color_table = 0;
	config.length = buffer.length;
	config.string = buffer.memory;
	config.colors = NULL;
	config.line_array = buffer.lcache + yview;
	config.line_count = (bx.y1 - bx.y0) / lpFont->line_height + 1;

	if(config.line_count > arrlen(buffer.lcache)) {
		config.line_count = arrlen(buffer.lcache);
	}

	rx_drawText( &config );

	return rxFalse;
}



/* todo: cursors need to be sorted and merged if occupy the same slot */


int
ecurcmp(
ECursor c0, ECursor c1)
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
EEditor *editor, int xchar, int yline)
{
  /* todo */
	for(int i=0; i<enumcur(editor); i+=1)
	{ if( (egetcurx(editor,i) == xchar) &&
		(egetcury(editor,i) == yline) ) return i;
}

return - 1;
}

/* probably do not use bubble sort */
void
esrtcur(
EEditor *editor)
{
	int sorted;

	do
	{
		sorted = rxTrue;

		for(int i=0; i<enumcur(editor)-1; i+=1)
		{
			ECursor c0 = egetcur(editor,i+0);
			ECursor c1 = egetcur(editor,i+1);

			int cmp = ecurcmp(c0,c1);
			if(cmp == +1)
			{ esetcur(editor,i+0,c1);
				esetcur(editor,i+1,c0);

				sorted = rxFalse;
			}
		}
	} while(sorted != rxTrue);
}

int
eaddcur(EEditor *lp, ECursor cur) {
	*arradd(lp->cursor,1) = cur;
	esrtcur(lp);
	return efndcur(lp,cur.xchar,cur.yline);
}

int
ecurloc(EEditor *wdg, int index) {
	return ebuffer_get_line_offset(&wdg->buffer,egetcury(wdg,index)) + egetcurx(wdg,index);
}

char *
egetptr(EEditor *wdg, int index)
{
  /* todo: this should point somewhere safer */
	rxGlobal char p;

  p = 0; /* in case it was overwritten */

	if(index >= 0 && index < wdg->buffer.length)
	{
		return &wdg->buffer.string[index];
	}

	return &p;
}

inline char
egetchr(EEditor *wdg, int offset)
{
	return *egetptr(wdg,offset);
}

inline ECursor
egetcur(
EEditor *editor, int index)
{
	return editor->cursor[index];
}

inline int
egetcurx(
EEditor *editor, int index)
{
	return egetcur(editor,index).xchar;
}

inline int
egetcury(
EEditor *editor, int index)
{
	return egetcur(editor,index).yline;
}

void
esetcur(EEditor *editor, int index, ECursor cur) {
	cur.yline = iclamp(cur.yline,0,arrlen(editor->buffer.lcache)-1);
	cur.xchar = iclamp(cur.xchar,0,ebuffer_get_line_length(&editor->buffer,cur.yline));
	editor->cursor[index] = cur;

#if 0
	ECursor old = egetcur(editor,index);
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
			inf += rlFont_getWidth(editor->font,ptr[i-0]);
		} else
		if(cur.xchar < old.xchar)
		{ for(int i=0;i>cur.xchar-old.xchar;i-=1)
			inf -= rlFont_getWidth(editor->font,ptr[i-1]);
		}
	}
	editor->curinf[index] = inf;
#endif
}

void
esetcurx(EEditor *editor, int index, int xchar)
{
	ECursor cur = egetcur(editor,index);

	cur.xchar = xchar;

	esetcur(editor,index,cur);
}

void
esetcury(EEditor *editor, int index, int yline)
{
	ECursor cur = egetcur(editor,index);

	cur.yline = yline;

	esetcur(editor,index,cur);
}

char
ecurchr(EEditor *wdg, int index, int off) {
	return egetchr(wdg,ecurloc(wdg,index) + off);
}

ECursor
emovcurx(EEditor *editor, int cursor, int mov) {
	ECursor cur = egetcur(editor,cursor);

	if(cur.xchar + mov > ebuffer_get_line_length(&editor->buffer,cur.yline)) {
		if(cur.yline + 1 <= arrlen(editor->buffer.lcache) - 1) {
			cur.yline += 1;
			cur.xchar  = 0;
		}
	} else
	if(cur.xchar + mov < 0) {
		if(cur.yline - 1 >= 0) {
			cur.yline -= 1;
			cur.xchar  = ebuffer_get_line_length(&editor->buffer,cur.yline);
		}
	} else {
		cur.xchar += mov;
	}

	esetcur(editor,cursor,cur);

	return cur;
}

void
emovcury(
EEditor *editor, int index, int mov)
{
	esetcury(editor,index,egetcury(editor,index)+mov);
}

void
eaddchr_(EEditor *wdg, int cursor, int length) {
}


void
Editor_keyOne(EEditor *wdg, E_KEY key) {

	ECursor cursor = egetcur(wdg,key.cur);
	int off = ecurloc(wdg,key.cur);
	int dir = 0;
	int cur = key.cur;
	int num = key.num;
	int chr = key.chr;
	switch(key.kid) {
		case E_kMOVE_LEFT:
		case E_kMOVE_RIGHT: {
			wdg->cursor_blink_timer = 0;
			dir = +1;
			if (key.kid == E_kMOVE_LEFT) {
				off += (dir = -1);
			}
			if (key.mod & E_MOD_CTRL_BIT) {
				if (E_IS_WORD_DELI(egetchr(wdg,off))) {
					while (egetchr(wdg,off)!=0 && E_IS_WORD_DELI(egetchr(wdg,off))) {
						emovcurx(wdg,cur,dir); off += dir;
					}
				} else {
					while (egetchr(wdg,off)!=0 && !E_IS_WORD_DELI(egetchr(wdg,off))) {
						emovcurx(wdg,cur,dir); off += dir;
					}
				}
			} else {
				emovcurx(wdg,cur,dir);
			}
		} break;
		case E_kMOVE_LINE_RIGHT: {
			esetcurx(wdg,cur,ebuffer_get_line_length(&wdg->buffer,cursor.yline));
		} break;
		case E_kMOVE_LINE_LEFT: {
			esetcurx(wdg,cur,0);
		} break;

		case E_kDELETE_BACK: {
			off += (dir = - 1);
		}
    	/* fall-through */
		case E_kDELETE_HERE:

		if (wdg->isReadonly) {
			return;
		}

		if (off >= 0 && off + num <= wdg->buffer.length) {
			wdg->cursor_blink_timer = 0;

			char *ptr = egetptr(wdg,off);

      	/* delete the entire line feed, as supposed to just breaking it */
			if((ptr[0 + dir] == '\r') && (ptr[1 + dir] == '\n')) {
				off += dir;
				num += 1;
			}

      /* gotta move the cursor first */
			emovcurx(wdg,cur,dir*num);

			EBuffer_insertSize(&wdg->buffer,off,-num);
			EBuffer_reformat(&wdg->buffer);

		} break;
		case E_kCHAR: {
			if (wdg->isReadonly) {
				return;
			}

			wdg->cursor_blink_timer = 0;
			E_ASSERT(chr != '\r' && chr != '\n' );

			int mov = 1;
			int end = 0;

			if(chr >= 0x80) {
				chr = '?';
				end =   0;
				mov =   1;
			} else
			if (chr == '\t') {
				chr = ' ';
				end = ' ';
				mov =   2;
			} else
			switch(chr) {
				case '{': end = '}'; break;
				case '[': end = ']'; break;
				case '(': end = ')'; break;
				case '}':
				case ']':
				case ')':
				if(egetchr(wdg,off) == chr) {
					mov = 1;
					chr = 0;
					end = 0;
				} break;
			}

			if (chr != 0) {
				num = end != 0 ? 2 : 1;

				char *ptr = EBuffer_insertSize(&wdg->buffer,off,num);
				ptr[0] = chr;

				if (end != 0) {
					ptr[1] = end;
				}

        		/* notify of the event and then move the index */
				eaddchr_(wdg,off,num);
				EBuffer_reformat(&wdg->buffer);
			}

			emovcurx(wdg,cur,mov);
		} break;
		case E_kLINE: {
			if (wdg->isReadonly) {
				return;
			}
      /* this is syntax specific #todo */
			if(egetchr(wdg,off-1) == '{') {
				num += 1;
			}

			char *ptr = EBuffer_insertSize(&wdg->buffer,off,num*2);

      /* proper line end feed #todo */
			while(num -- != 0) {
				ptr[num*2+0] = '\r';
				ptr[num*2+1] = '\n';
			}

      // eaddrow(wdg,egetcury(wdg,index),num);

			eaddchr_(wdg,cur,num*2);
			emovcury(wdg,cur,    1);
			esetcurx(wdg,cur,    0);

			EBuffer_reformat(&wdg->buffer);
		} break;
	}
}


void
Editor_testKeys(EEditor *editor) {

	int mod = 0;

	/* [[todo]]: why doesn't rl just use a flag for this */
	if rx_testCtrlKey() {
		mod |= E_MOD_CTRL_BIT;
	}
	if rx_testAltKey() {
		mod |= E_MOD_ALT_BIT;
	}
	if rx_testShiftKey() {
		mod |= E_MOD_SHIFT_BIT;
	}

	if(IS_CLICK_ENTER(0)) {
		#if 0
		int xcursor = + rx.wnd.in.mice.xcursor;
		int ycursor = - rx.wnd.in.mice.ycursor + rx.wnd.size_y;

		int yline = editor->lyview + ycursor / editor->font->line_height;

		emarker_t line = Emu_buffer_get_line_at_index(&editor->buffer,yline);
		char *string = editor->buffer.string + line.offset;

		float xwalk = 0;
		for(int xchar=0; xchar<line.length; xchar+=1)
		{
			float width = rlFont_getWidth(editor->font,string[xchar]);

			if(xcursor >= xwalk && xcursor <= xwalk + width)
			{
				esetcur(editor,0,(ECursor){xchar,yline});
				break;
			}

			xwalk += width;
		}
#endif
	} else
	if(rx_testCtrlKey() && rx_testKey('Z')) {
	} else
	if(rx_testKey(rx_kESCAPE))
	{
    /* todo */
		dlb_t *dlb = ccdlb(editor->cursor);
		dlb->sze_min = 1;

	} else
	if(rx.wnd.in.mice.yscroll != 0)
	{
    /* scroll up */
		editor->lyview += rx_testShiftKey() ? 16 : - rx.wnd.in.mice.yscroll;
		editor->lyview  = iclamp(editor->lyview,0,arrlen(editor->buffer.lcache)-1);
	} else
	if(rx_testKey(rx_kHOME))
	{
		Editor_handleKey(editor,E_kMOVE_LINE_LEFT,1,0);

	} else
	if(rx_testKey(rx_kEND))
	{
		Editor_handleKey(editor,E_kMOVE_LINE_RIGHT,1,0);
	} else
	if(rx_testCtrlKey() && rx_testKey('X'))
	{
		for(int i=enumcur(editor)-1;i>=0;i-=1)
		{
			emarker_t row = Emu_buffer_get_line_at_index(&editor->buffer,egetcury(editor,i));

			int num = 1;

			char *ptr = egetptr(editor,row.offset+row.length);
			if((ptr[0]=='\r') &&
			(ptr[1]=='\n')) num += 1;

			EBuffer_insertSize(&editor->buffer,row.offset,-(row.length+num));
		EBuffer_reformat(&editor->buffer);
	}

} else
if(rx_testKey(rxKEY_kUP))
{
	if(rx_testCtrlKey() && rx_testAltKey())
	{
		ECursor cur = egetcur(editor,0);
		cur.yline -= 1;
		eaddcur(editor,cur);
	} else
	if(rx_testCtrlKey())
	{
      /* scroll up */
		editor->lyview -= rx_testShiftKey() ? 16 : 1;
		editor->lyview  = iclamp(editor->lyview,0,arrlen(editor->buffer.lcache)-1);
	} else
	{
      /* move to the line above */
		for(int i=enumcur(editor)-1;i>=0;i-=1)
		emovcury(editor,i,-1);
}
} else
if(rx_testKey(rxKEY_kDOWN))
{
	if(rx_testCtrlKey() && rx_testAltKey())
	{
		ECursor cur = egetcur(editor,enumcur(editor)-1);
		cur.yline += 1;
		eaddcur(editor,cur);
	} else
	if(rx_testCtrlKey())
	{
      /* scroll down */
		editor->lyview += rx_testShiftKey() ? 16 : 1;
		editor->lyview = iclamp(editor->lyview,0,arrlen(editor->buffer.lcache)-1);
	} else
	{
      /* move to the line below */
		for(int i=enumcur(editor)-1;i>=0;i-=1)
		emovcury(editor,i,+1);
}
} else
if(rx_testKey(rxKEY_kLEFT))
{ Editor_handleKey(editor,E_kMOVE_LEFT,   1,0);
} else
if(rx_testKey(rxKEY_kRIGHT))
{
	Editor_handleKey(editor,E_kMOVE_RIGHT, 1,0);
} else
if(rx_testKey(rx_kDELETE))
{ Editor_handleKey(editor,E_kDELETE_HERE,1,0);
} else
if(rx_testKey(rx_kBCKSPC))
{ Editor_handleKey(editor,E_kDELETE_BACK,1,0);
} else
if(rx_testKey(rxKEY_kRETURN))
{
	Editor_handleKey(editor,E_kLINE,1,0);
} else
{ if(!rx_testCtrlKey())
	{
		if(rx_testChar() != 0)
		{
			Editor_handleKey(editor,E_kCHAR,1,rx_testChar());
		}
	}
}
}