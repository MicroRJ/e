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

typedef union erect_t/* lol */
{ struct
  { rxvec2_t xy0;
    rxvec2_t xy1;
  };
  struct
  {
    float x0,y0,x1,y1;
  };
} erect_t;

enum
{ RECT_kLFT ,
  RECT_kRGT,
  RECT_kTOP  ,
  RECT_kBOT };


ccinle erect_t
erect_xyxy(
  float x0, float y0, float x1, float y1)
{
  erect_t rect;
  rect.x0 = x0; rect.x1 = x1;
  rect.y0 = y0; rect.y1 = y1;

  ccassert(x0 <= x1);
  ccassert(y0 <= y1);
  return rect;
}

ccinle erect_t
erect_xywh(
  float x, float y, float xx, float yy)
{
  return erect_xyxy(x,y,x+xx,y+yy);
}

float
rect_in_xy(
  erect_t rect, float x, float y)
{
  return
    ((x >= rect.x0) && (y >= rect.y0)) &&
    ((x <  rect.x1) && (y <  rect.y1));
}

erect_t
rect_cut(
  erect_t *rect, int mode, float size)
{
  erect_t result = erect_xywh(0,0,0,0);
  switch(mode)
  { case RECT_kTOP:
    { result = erect_xyxy(rect->x0,rect->y1-size,rect->x1,rect->y1);
      rect->y1 -= size;
    } break;
    case RECT_kBOT:
    { result = erect_xyxy(rect->x0,rect->y0,rect->x1,rect->y0+size);
      rect->y0 += size;
    } break;
    case RECT_kRGT:
    { result = erect_xyxy(rect->x1-size,rect->y0,rect->x1,rect->y1);
      rect->x1 -= size;
    } break;
    case RECT_kLFT:
    { result = erect_xyxy(rect->x0,rect->y0,rect->x0+size,rect->y1);
      rect->x0 += size;
    } break;
  }
  return result;
}

erect_t
rect_center(erect_t parent, erect_t child)
{
  erect_t result;
  result.x0 = parent.x0 + ((parent.x1 - parent.x0) * .5) - ((child.x1 - child.x0) * .5);
  result.y0 = parent.y0 + ((parent.y1 - parent.y0) * .5) - ((child.y1 - child.y0) * .5);
  result.x1 = result.x0 + (child.x1 - child.x0);
  result.y1 = result.y0 + (child.y1 - child.y0);
  return result;
}

erect_t
rect_padd(erect_t rect, float xpadd, float ypadd)
{
  return
    erect_xyxy(
      rect.x0 + (xpadd * .5),
      rect.y0 + (ypadd * .5),
      rect.x1 - (xpadd * .5),
      rect.y1 - (ypadd * .5));
}
