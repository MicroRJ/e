/*
**
** Copyright(c) 2023 - Dayan Rodriguez - Dyr
**
** -+- elang -+-
**
*/
// little user interface (lui)

void lui__drawText(lui_Box box, char const *string);
void lui__drawRoundBox(lui_Box box, lgi_Color color, float cornerRadius);

void lui__check_overflow() {
	lui.boxcount++;
	EL_ASSERT(lui.box<lui.boxstack+(sizeof(lui.boxstack)/sizeof(lui.boxstack[0])));
}
void lui__check_underflow() {
	lui.boxcount--;
	EL_ASSERT(lui.box>lui.boxstack);
}
#define lui_inibox() (lui.box = lui.boxstack)
#define lui_dupbox() (lui__check_overflow(), lui.box[1] = lui.box[0], lui.box += 1)
#define lui_popbox() (lui__check_underflow(), *(lui.box -= 1))
#define lui_getbox() (*lui.box)
#define lui_setbox(xx) (lui_getbox() = (xx))
/* this has to be a function, operand has to be evaluated first and only once */
void lui_putbox(lui_Box xx) {
	lui_dupbox();
	lui_setbox(xx);
}
/* TODO: this should clip too */
#define lui_cutbox(side,size) lui_putbox(lui_boxcut(lui.box,FUSE(lui_,side),size))

// WHEN COME BACK FIX THIS
#define lui_hasline() ((lui_getbox().y1-lui_getbox().y0)>=TUI_LINE_HEIGHT)
#define lui_newline() lui_cutbox(top,TUI_LINE_HEIGHT)
#define lui_endline() lui_popbox()

#define lui_newcol(side,size) lui_cutbox(side,size)
#define lui_endcol() lui_popbox()


int lui_testinbox(lui_Box box, float x, float y) {
	return ((x >= box.x0) && (y >= box.y0)) && ((x <  box.x1) && (y <  box.y1));
}
int lui_clickbox(lui_Box box) {
	return IS_CLICK_ENTER(0) && lui_testinbox(box,rx.wnd.in.mice.xcursor,rx.wnd.in.mice.ycursor);
}
int lui_unclickbox(lui_Box box) {
	return IS_CLICK_LEAVE(0) && lui_testinbox(box,rx.wnd.in.mice.xcursor,rx.wnd.in.mice.ycursor);
}

#define lui_text(xx) lui__drawText(*lui.box,xx)
#define lui_texf(ff,...) lui__drawText(*lui.box,elStringFormat(ff,__VA_ARGS__))


lui_Box
lui_dobox(float x0, float y0, float x1, float y1) {
	lui_Box rect;
	rect.x0 = x0; rect.x1 = x1;
	rect.y0 = y0; rect.y1 = y1;
	return rect;
}

lui_Box
lui_boxcut(lui_Box *box, int mode, float size) {
	lui_Box result = lui_dobox(0,0,0,0);
	switch(mode) {
		case lui_top: {
			result = lui_dobox(box->x0,box->y1-size,box->x1,box->y1);
			box->y1 -= size;
		} break;
		case lui_bottom: {
			result = lui_dobox(box->x0,box->y0,box->x1,box->y0+size);
			box->y0 += size;
		} break;
		case lui_right: {
			result = lui_dobox(box->x1-size,box->y0,box->x1,box->y1);
			box->x1 -= size;
		} break;
		case lui_left: {
			if (size == -1) {
				size = box->x1 - box->x0;
			}
			result = lui_dobox(box->x0,box->y0,box->x0+size,box->y1);
			box->x0 += size;
		} break;
	}
	return result;
}

lui_Box
rect_center(lui_Box parent, lui_Box child) {
	lui_Box result;
	result.x0 = parent.x0 + ((parent.x1 - parent.x0) * .5) - ((child.x1 - child.x0) * .5);
	result.y0 = parent.y0 + ((parent.y1 - parent.y0) * .5) - ((child.y1 - child.y0) * .5);
	result.x1 = result.x0 + (child.x1 - child.x0);
	result.y1 = result.y0 + (child.y1 - child.y0);
	return result;
}

lui_Box
rect_padd(lui_Box rect, float xpadd, float ypadd) {
	return lui_dobox(rect.x0 + (xpadd * .5)
	, rect.y0 + (ypadd * .5)
	, rect.x1 - (xpadd * .5)
	, rect.y1 - (ypadd * .5));
}



#define euyield() Or(lui_getbox().y1<=lui_getbox().y0, lui_getbox().x1<=lui_getbox().x0)

#define setcol(xx,yy) do {\
/* */lui.textColor = xx;\
/* */lui.textColor.r *= yy;\
/* */lui.textColor.g *= yy;\
/* */lui.textColor.b *= yy;\
} while(0)

lui_State *
lui_query_state(elPointer key, elBool createWhenNotFound) {
	if isNull(key) {
		return elNull;
	}
	/* todo: */
	elGlobal lui_State w[0x1000];

	elInteger n = sizeof(w) / sizeof(w[0]);
	elInteger h = (elInteger)(key) % n;

	lui_State *lastState = w + h;

	while isNotNullAnd(lastState->key,lastState->key!=key) {
		lastState += 1;
	}

	lui_State *result = elNull;

	if isTrue(lastState->key == key) {
		result = lastState;
	} else
	if isTrue(createWhenNotFound) {
		if isNull(lastState->key) {
			result = lastState;
			result->key = key;
		} else {
			EL_ROADBLOCK();
		}
	}

	return result;
}

