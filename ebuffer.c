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

/* implementation */
void
ebuffer_init(
  ebuffer_t *buffer, char const *tag, cci64_t length)
{
  ZeroMemory(buffer,sizeof(*buffer));

  buffer->tag = estring_copy(tag);

  if(length)
  {
    ebuffer_manage(buffer,length,length);
  }

  /* todo: this should be passed in */
  esyntax_init(&buffer->syntax);
}

void
ebuffer_uninit(
  ebuffer_t *buffer)
{
  ememory_release_aligned(buffer->memory);
  ememory_release_aligned(buffer->colors);
  buffer->length = 0;
  buffer->extent = 0;

  ccfree(buffer->tag);
  buffer->tag = 0;
}

char *
ebuffer_manage(
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

    buffer->memory = ememory_realloc_aligned(sizeof(*buffer->memory) * buffer->extent, buffer->memory);
    buffer->colors = ememory_realloc_aligned(sizeof(*buffer->colors) * buffer->extent, buffer->colors);
  }

  buffer->length = buffer->length + commit;

  return buffer->memory + buffer->length;
}

char *
ebuffer_insert(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  ebuffer_manage(buffer,length,length);

  if(offset != buffer->length)
  {
    memmove(
      buffer->memory+offset+length,
      buffer->memory+offset,
      buffer->length-offset-length);

    memmove(
      buffer->colors+offset+length,
      buffer->colors+offset,
      buffer->length-offset-length);
  }

  return buffer->memory+offset;
}

void
ebuffer_remove(
  ebuffer_t *buffer, cci64_t offset, cci64_t length)
{
  if(offset != buffer->length)
  {
    memmove(
      buffer->memory+offset,
      buffer->memory+offset+length,
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
ebuffer_get_line_marker(
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
  return ebuffer_get_line_marker(buffer,yline).offset;
}

ccinle int
ebuffer_get_line_length(
  ebuffer_t *buffer, int yline)
{
  return ebuffer_get_line_marker(buffer,yline).length;
}

/* this should be done automatically #todo */
void
ebuffer_update_lcache(
  ebuffer_t *buffer)
{

  for(int i=0; i<buffer->length;)
  {
  	esample_t sample =
  		buffer->syntax.sampler(&buffer->syntax,NULL,0,
  			buffer->length,buffer->memory+i);

		for(int j=0;j<sample.width;j+=1)
		{
			buffer->colors[i+j] = sample.token;
		}

		i += sample.width;
  }

  char *cursor = buffer->memory;

  /* todo */
  earray_delete(buffer->lcache);
  buffer->lcache = 0;


  int indent = 0;
  for(;;)
  {
    emarker_t *line = earray_add(buffer->lcache,1);
    line->offset = (cursor - buffer->memory);

    while((cursor < buffer->memory + buffer->length) &&
          (*cursor != '\r') &&
          (*cursor != '\n')) cursor += 1;

    line->length = (cursor - buffer->memory) - line->offset;

    if(cursor < buffer->memory + buffer->length)
    {
      cursor += ((cursor[0] == '\r') &&
                 (cursor[1] == '\n'))  ? 2 : 1;
    } else
    {
      break;
    }
  }

  ccassert(cursor == buffer->memory + buffer->length);
}
