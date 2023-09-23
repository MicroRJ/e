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
EBuffer_setName(EBuffer *lpBuffer, char const *lpName) {
	strcpy(lpBuffer->name,lpName);
}

void
EBuffer_setFileName(EBuffer *lpBuffer, char const *lpName) {
	strcpy(lpBuffer->fileName,lpName);
}


void
EBuffer_initSized(EBuffer *lpBuffer, char const *tag, cci64_t length) {
	EBuffer_setName(lpBuffer,tag);

	if(length) {

		EBuffer_allocSize(lpBuffer,length,length);
	}
}

void
EBuffer_uninit(EBuffer *buffer) {

	E_FREE_ALIGNED(buffer->string);
	E_FREE_ALIGNED(buffer->colors);
	buffer->length = 0;
	buffer->extent = 0;
}

char *
EBuffer_allocSize(EBuffer *lpBuffer, __int64 reserve, __int64 commit) {
	E_ASSERT(commit <= reserve + lpBuffer->extent - lpBuffer->length);

	if(lpBuffer->extent < lpBuffer->length+reserve) {
		lpBuffer->extent <<= 1;
		if(lpBuffer->extent < lpBuffer->length + reserve) {
			lpBuffer->extent = lpBuffer->length + reserve;
		}
		lpBuffer->string = E_REALLOC_ALIGNED(sizeof(*lpBuffer->string) * lpBuffer->extent, lpBuffer->string);
		lpBuffer->colors = E_REALLOC_ALIGNED(sizeof(*lpBuffer->colors) * lpBuffer->extent, lpBuffer->colors);
	}

	lpBuffer->length = lpBuffer->length + commit;
	return lpBuffer->string + lpBuffer->length;
}

char *
EBuffer_insertSize(EBuffer *buffer, __int64 offset, __int64 length) {

	char *colors = (char *) buffer->colors + offset;
	char *string = (char *) buffer->string + offset;
	size_t tomove = buffer->length - offset - length;

	// printf("length: [%lli/%lli] insert: %lli tomove: %lli\n"
	// , buffer->length, buffer->extent, length, tomove);

	if (length > 0) {
		EBuffer_allocSize(buffer,length,length);
	}

	if (offset != buffer->length) {


		/* [[todo]]: this could be niftier */
		if (length < 0) {
			E_MEMMOVE(string,string-length,tomove);
			E_MEMMOVE(colors,colors-length,tomove);
		} else {
			E_MEMMOVE(string+length,string,tomove);
			E_MEMMOVE(colors+length,colors,tomove);
		}
	}

	if (length < 0) {
		buffer->length += length;
	}

	return buffer->string+offset;
}


/* could this be done automatically #todo */
void
Emu_buffer_rescan_lines(EBuffer *buffer) {

	char *cursor = buffer->string;
  /* todo */
	earray_delete(buffer->lcache);
	buffer->lcache = 0;

	int indent = 0;
	for(;;) {
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

ccinle emarker_t
Emu_buffer_get_line_at_index(
EBuffer *buffer, int index)
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
EBuffer *buffer, int yline)
{
	return Emu_buffer_get_line_at_index(buffer,yline).offset;
}

ccinle int
ebuffer_get_line_length(
EBuffer *buffer, int yline)
{
	return Emu_buffer_get_line_at_index(buffer,yline).length;
}

