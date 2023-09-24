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


void EBuffer_initSized(EBuffer *, char const *, __int64 length);


emarker_t Emu_buffer_get_line_at_index(EBuffer *buffer, int index);
int ebuffer_get_line_length(EBuffer *buffer, int yline);
int ebuffer_get_line_offset(EBuffer *buffer, int yline);
