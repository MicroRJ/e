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

void *
Emu_memory_realloc_aligned(cci64_t length, void *memory)
{
	return _aligned_realloc(memory,length,16);
}

void
Emu_memory_release_aligned(void *memory)
{
	_aligned_free(memory);
}

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

/* #todo */
ccfunc ccinle void
Emu_memcpy2d( Emu_memcpy2d_config_t *config )
{
  for (int y = 0; y < config->src.height; y += 1)
  {
    memcpy(
      config->dst.cursor + config->dst.stride * (y + config->dst.offset_y) + config->dst.offset_x,
      config->src.cursor + config->src.stride * (y + config->src.offset_y) + config->src.offset_x,

    	config->src.stride );
  }
}