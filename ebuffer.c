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

void
Emu_buffer_init(
  ebuffer_t *buffer, char const *tag, cci64_t length)
{
   ZeroMemory(buffer,sizeof(*buffer));

   buffer->tag = estring_copy(tag);

   if(length) {

      Emu_buffer_reserve_and_commit(buffer,length,length);
   }

   /* todo: this should be passed in */
   esyntax_init(&buffer->syntax);
}

void
Emu_buffer_uinit(
  ebuffer_t *buffer)
{
  Emu_memory_release_aligned(buffer->string);
  Emu_memory_release_aligned(buffer->colors);
  buffer->length = 0;
  buffer->extent = 0;

  ccfree(buffer->tag);
  buffer->tag = 0;
}

char *
Emu_buffer_reserve_and_commit(
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

    buffer->string = Emu_memory_realloc_aligned(sizeof(*buffer->string) * buffer->extent, buffer->string);
    buffer->colors = Emu_memory_realloc_aligned(sizeof(*buffer->colors) * buffer->extent, buffer->colors);
  }

  buffer->length = buffer->length + commit;

  return buffer->string + buffer->length;
}

char *
Emu_buffer_insert(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  Emu_buffer_reserve_and_commit(buffer,length,length);

  if(offset != buffer->length)
  {
    memmove(
      buffer->string+offset+length,
      buffer->string+offset,
      buffer->length-offset-length);

    memmove(
      buffer->colors+offset+length,
      buffer->colors+offset,
      buffer->length-offset-length);
  }

  return buffer->string+offset;
}

void
Emu_buffer_remove(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  if(offset != buffer->length)
  {
    memmove(
      buffer->string+offset,
      buffer->string+offset+length,
      buffer->length-offset-length);

    memmove(
      buffer->colors+offset,
      buffer->colors+offset+length,
      buffer->length-offset-length);
  }

  /* todo: */
  buffer->length -= length;
}


ccinle emarker_t
Emu_buffer_get_line_at_index(
  ebuffer_t *buffer, int index)
{
  /* remove, this should not happen? */
  if(buffer->lcache == ccnull)
  {
    emarker_t row;
    row.length = 0;
    row.offset = 0;
    return row;
  }

  return buffer->lcache[index];
}

ccinle int
ebuffer_get_line_offset(
  ebuffer_t *buffer, int yline)
{
  return Emu_buffer_get_line_at_index(buffer,yline).offset;
}

ccinle int
ebuffer_get_line_length(
  ebuffer_t *buffer, int yline)
{
  return Emu_buffer_get_line_at_index(buffer,yline).length;
}

void
Emu_buffer_update_colors(
   ebuffer_t *buffer)
{
   esample_t sample;
   for(int i=0; i<buffer->length; i += sample.width)
   {
      sample = buffer->syntax.sampler(&buffer->syntax,NULL,0,buffer->length,buffer->string+i);
      memset(buffer->colors+i,sample.token,sample.width);
   }
}
/* could this be done automatically #todo */
void
Emu_buffer_rescan_lines(
  ebuffer_t *buffer)
{



  char *cursor = buffer->string;

  /* todo */
  earray_delete(buffer->lcache);
  buffer->lcache = 0;


  int indent = 0;
  for(;;)
  {
    emarker_t *line = earray_add(buffer->lcache,1);
    line->offset = (cursor - buffer->string);

    while((cursor < buffer->string + buffer->length) &&
          (*cursor != '\r') &&
          (*cursor != '\n')) cursor += 1;

    line->length = (cursor - buffer->string) - line->offset;

    if(cursor < buffer->string + buffer->length)
    {
      cursor += ((cursor[0] == '\r') &&
                 (cursor[1] == '\n'))  ? 2 : 1;
    } else
    {
      break;
    }
  }

  ccassert(cursor == buffer->string + buffer->length);
}
