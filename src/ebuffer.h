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

typedef Emu_text_line_t emarker_t;

typedef struct {
	char name[MAX_PATH];
	char fileName[MAX_PATH];

   /* [[todo]]: this could be replaced with single ints, the length of the line
   	isn't necessary, at least not explicitly, it can be calculated by using the
   	offset of the next line instead  */
	emarker_t      *lcache;

	unsigned char  *colors;
	union  {
		void        *memory;
		char        *string;
	};

	int           modified;
	meow_u128     checksum;

	__int64         extent;
	__int64         length;
} EBuffer;

void EBuffer_setName(EBuffer *, char const *);
void EBuffer_setFileName(EBuffer *, char const *);

void EBuffer_initSized(EBuffer *, char const *, __int64 length);

/* [[todo]]: alloc and insert could be replaced with a single really nifty function */
char *EBuffer_allocSize(EBuffer *, __int64 reserve, __int64 commit);
char *EBuffer_insertSize(EBuffer *, __int64 offset, __int64 length);

emarker_t Emu_buffer_get_line_at_index(EBuffer *buffer, int index);
int ebuffer_get_line_length(EBuffer *buffer, int yline);
int ebuffer_get_line_offset(EBuffer *buffer, int yline);
