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

typedef union rlBOX {
	struct {
		rxvec2_t xy0;
		rxvec2_t xy1;
	};
	struct {
		float x0,y0,x1,y1;
	};
} rlBOX;

enum {
	RECT_kLFT,
	RECT_kRGT,
	RECT_kTOP,
	RECT_kBOT
};

rlBOX
rlMakeBox(float x0, float y0, float x1, float y1) {
	rlBOX rect;
	rect.x0 = x0; rect.x1 = x1;
	rect.y0 = y0; rect.y1 = y1;

	ccassert(x0 <= x1);
	ccassert(y0 <= y1);
	return rect;
}

rlBOX
rlMakeBoxBySize(float x, float y, float xx, float yy)	{
	return rlMakeBox(x,y,x+xx,y+yy);
}

/* todo: remove */
rlBOX
rlBoxCut(rlBOX *wndBox, int mode, float size) {
	rlBOX result = rlMakeBoxBySize(0,0,0,0);
	switch(mode) {
		case RECT_kTOP:{
			result = rlMakeBox(wndBox->x0,wndBox->y1-size,wndBox->x1,wndBox->y1);
			wndBox->y1 -= size;
		} break;
		case RECT_kBOT:{
			result = rlMakeBox(wndBox->x0,wndBox->y0,wndBox->x1,wndBox->y0+size);
			wndBox->y0 += size;
		} break;
		case RECT_kRGT:{
			result = rlMakeBox(wndBox->x1-size,wndBox->y0,wndBox->x1,wndBox->y1);
			wndBox->x1 -= size;
		} break;
		case RECT_kLFT:{
			result = rlMakeBox(wndBox->x0,wndBox->y0,wndBox->x0+size,wndBox->y1);
			wndBox->x0 += size;
		} break;
	}
	return result;
}


float
rect_in_xy(rlBOX rect, float x, float y) {
	return
	((x >= rect.x0) && (y >= rect.y0)) &&
	((x <  rect.x1) && (y <  rect.y1));
}

rlBOX
rect_center(rlBOX parent, rlBOX child)
{
	rlBOX result;
	result.x0 = parent.x0 + ((parent.x1 - parent.x0) * .5) - ((child.x1 - child.x0) * .5);
	result.y0 = parent.y0 + ((parent.y1 - parent.y0) * .5) - ((child.y1 - child.y0) * .5);
	result.x1 = result.x0 + (child.x1 - child.x0);
	result.y1 = result.y0 + (child.y1 - child.y0);
	return result;
}

rlBOX
rect_padd(rlBOX rect, float xpadd, float ypadd)
{
	return
	rlMakeBox(
	rect.x0 + (xpadd * .5),
	rect.y0 + (ypadd * .5),
	rect.x1 - (xpadd * .5),
	rect.y1 - (ypadd * .5));
}
