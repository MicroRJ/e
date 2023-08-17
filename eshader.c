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

/* todo: rename these */
ccglobal rxshader_t vs,ps_circle,ps_box;

void
eshader_init()
{
  vs        = rxshader_load(RX_OBJECT_TYPE_kVERTEX_SHADER,"MainVS","eshader.hlsl");
  ps_circle = rxshader_load( RX_OBJECT_TYPE_kPIXEL_SHADER,"MainPS_CircleSDF","eshader.hlsl");
  ps_box    = rxshader_load( RX_OBJECT_TYPE_kPIXEL_SHADER,"MainPS_BoxSDF","eshader.hlsl");
}

/* todo: instead of center based we should take corner based arguments, makes everything easier */
void
draw_box_sdf(
  rxvec2_t center, rxvec2_t radius, rxcolor_t color, float roundness, float softness )
{
  float x0,y0,x1,y1;
  x0 = center.x - (radius.x + softness);
  y0 = center.y - (radius.y + softness);
  x1 = center.x + (radius.x + softness);
  y1 = center.y + (radius.y + softness);

  shaset(vs,ps_box);
  rxbgn();
    rx.imp.attr.rect.xy     = rxvec2_xy(x0,y0);
    rx.imp.attr.rect.xyxy   = rxvec4_xyzw(center.x,center.y,radius.x,radius.y);
    rx.imp.attr.rect.rgba   = color;
    rx.imp.attr.rect.flag.x = roundness;
    rx.imp.attr.rect.flag.w = softness;
    rxaddvtx(rx.imp.attr);

    rx.imp.attr.rect.xy    = rxvec2_xy(x0,y1);
    rx.imp.attr.rect.xyxy  = rxvec4_xyzw(center.x,center.y,radius.x,radius.y);
    rx.imp.attr.rect.rgba  = color;
    rx.imp.attr.rect.flag.x = roundness;
    rx.imp.attr.rect.flag.w = softness;
    rxaddvtx(rx.imp.attr);

    rx.imp.attr.rect.xy    = rxvec2_xy(x1,y1);
    rx.imp.attr.rect.xyxy  = rxvec4_xyzw(center.x,center.y,radius.x,radius.y);
    rx.imp.attr.rect.rgba  = color;
    rx.imp.attr.rect.flag.x = roundness;
    rx.imp.attr.rect.flag.w = softness;
    rxaddvtx(rx.imp.attr);

    rx.imp.attr.rect.xy    = rxvec2_xy(x1,y0);
    rx.imp.attr.rect.xyxy  = rxvec4_xyzw(center.x,center.y,radius.x,radius.y);
    rx.imp.attr.rect.rgba  = color;
    rx.imp.attr.rect.flag.x = roundness;
    rx.imp.attr.rect.flag.w = softness;
    rxaddvtx(rx.imp.attr);

    rxaddnidx(6, 0,1,2, 0,2,3);
  rxend();
}