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

typedef struct
{
  struct
  {
    int offset_x, offset_y;
    int stride, height;
    unsigned char *cursor;
  } dst;

  struct
  {
    int offset_x, offset_y;
    int stride, height;
    unsigned char const *cursor;
  } src;

} Emu_memcpy2d_config_t;

/* is this silly, am I silly? #todo */
ccfunc ccinle void
EMU_memcpy2d( Emu_memcpy2d_config_t *config )
{
  for (int y = 0; y < config->src.height; y += 1)
  {
    memcpy(
      config->dst.cursor + config->dst.stride * (y + config->dst.offset_y) + config->dst.offset_x,
      config->src.cursor + config->src.stride * (y + config->src.offset_y) + config->src.offset_x,

    	config->src.stride );
  }
}

