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

/* todo: invest some skill points in gap buffer research */
typedef struct ebuffer_t
{
  char const * tag;
  int          dif;

  char       * memory;
  cci64_t      extent;/* the max capacity of the buffer */
  cci64_t      length;
} ebuffer_t;

/* I don't like this init function */
void ebuffer_init(ebuffer_t *bfufer, char const *, cci64_t);

char *ebuffer_rescom(ebuffer_t *,cci64_t reserve, cci64_t commit);
char *ebuffer_insert(ebuffer_t *,cci64_t offset, cci64_t length);
void ebuffer_delete(ebuffer_t *,cci64_t offset, cci64_t length);

/* implementation */
void
ebuffer_init(
  ebuffer_t *buffer, char const *tag, cci64_t length)
{
  ZeroMemory(buffer,sizeof(*buffer));

  buffer->tag = estring_copy(tag);

  if(length)
  {
    ebuffer_rescom(buffer,length,length);
  }
}

void
ebuffer_uninit(
  ebuffer_t *buffer)
{
  ccfree(buffer->memory);
  buffer->length = 0;
  buffer->extent = 0;

  ccfree(buffer->tag);
  buffer->tag = 0;

  buffer->dif = 0;
}

char *
ebuffer_rescom(
  ebuffer_t *buffer, cci64_t reserve, cci64_t commit)
{
  ccassert(commit <= reserve + buffer->extent - buffer->length);

  if(buffer->extent < buffer->length+reserve)
  {
    /* this might need fixing */
    buffer->extent <<= 1;

    if(buffer->extent < buffer->length + reserve)
    {
      buffer->extent = buffer->length + reserve;
    }

    buffer->memory = ccrealloc(buffer->memory, buffer->extent);
  }

  buffer->length = buffer->length + commit;

  return buffer->memory + buffer->length;
}

char *
ebuffer_insert(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  ebuffer_rescom(buffer,length,length);

  if(offset != buffer->length)
  {
    memmove(
      buffer->memory+offset+length,
      buffer->memory+offset,
      buffer->length-offset-length);
  }

  return buffer->memory+offset;
}

void
ebuffer_delete(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  if(offset != buffer->length)
  {
    memmove(
      buffer->memory+offset,
      buffer->memory+offset+length,
      buffer->length-offset-length);
  }

  /* todo: */
  buffer->length -= length;
}

