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

// box thing of sorts, for widget stuff

enum {
	boxen_left,
	boxen_right,
	boxen_top,
	boxen_bot
};

typedef union boxthing {
	struct {
		rxvec2_t xy0;
		rxvec2_t xy1;
	};
	struct {
		float x0,y0,x1,y1;
	};
} boxthing;

boxthing
makeboxthing(float x0, float y0, float x1, float y1) {
	boxthing rect;
	rect.x0 = x0; rect.x1 = x1;
	rect.y0 = y0; rect.y1 = y1;
	return rect;
}

/* todo: remove */
boxthing
rlBoxCut(boxthing *wndBox, int mode, float size) {
	boxthing result = makeboxthing(0,0,0,0);
	switch(mode) {
		case boxen_top:{
			result = makeboxthing(wndBox->x0,wndBox->y1-size,wndBox->x1,wndBox->y1);
			wndBox->y1 -= size;
		} break;
		case boxen_bot:{
			result = makeboxthing(wndBox->x0,wndBox->y0,wndBox->x1,wndBox->y0+size);
			wndBox->y0 += size;
		} break;
		case boxen_right:{
			result = makeboxthing(wndBox->x1-size,wndBox->y0,wndBox->x1,wndBox->y1);
			wndBox->x1 -= size;
		} break;
		case boxen_left:{
			if (size == -1) {
				size = wndBox->x1 - wndBox->x0;
			}
			result = makeboxthing(wndBox->x0,wndBox->y0,wndBox->x0+size,wndBox->y1);
			wndBox->x0 += size;
		} break;
	}
	return result;
}


int
rect_in_xy(boxthing rect, float x, float y) {
	return
	((x >= rect.x0) && (y >= rect.y0)) &&
	((x <  rect.x1) && (y <  rect.y1));
}

boxthing
rect_center(boxthing parent, boxthing child)
{
	boxthing result;
	result.x0 = parent.x0 + ((parent.x1 - parent.x0) * .5) - ((child.x1 - child.x0) * .5);
	result.y0 = parent.y0 + ((parent.y1 - parent.y0) * .5) - ((child.y1 - child.y0) * .5);
	result.x1 = result.x0 + (child.x1 - child.x0);
	result.y1 = result.y0 + (child.y1 - child.y0);
	return result;
}

boxthing
rect_padd(boxthing rect, float xpadd, float ypadd)
{
	return
	makeboxthing(
	rect.x0 + (xpadd * .5),
	rect.y0 + (ypadd * .5),
	rect.x1 - (xpadd * .5),
	rect.y1 - (ypadd * .5));
}






int
cursor_in_rect(boxthing rect) {
	return rect_in_xy(rect,rx.wnd.in.mice.xcursor,rx.wnd.in.mice.ycursor);
}

int
is_click_leave_rect(boxthing rect) {
	return IS_CLICK_LEAVE(0) && cursor_in_rect(rect);
}

void
rlDrawRect(boxthing rect, rlColor color) {
	Emu_imp_rect(color,rect.x0,rect.y0,(rect.x1-rect.x0),(rect.y1-rect.y0));
}

void
set_clip_rect(boxthing rect) {
  //rxGPU_setClip(
  //  rect.x0,rect.y0,rect.x1,rect.y1);
}
