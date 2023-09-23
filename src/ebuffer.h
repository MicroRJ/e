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

/* invest some skill points in gap buffer research #maybe */
typedef struct
{
   char const * tag;

   char const * disk_path;


   /* convert to unsigned ints instead #todo #urgent */
   emarker_t      *lcache;

   unsigned char  *colors;
   union  {
   void           *memory;
   char           *string;
};

   int           modified;
   meow_u128     checksum;
   /* the max capacity of the buffer */
   cci64_t         extent;
   cci64_t         length;
} EBuffer;

typedef struct
{ char const * tag;
   char const * disk_save;
   char const * disk_load;
   char       * memory;
   char       * colors;
   int          length;
} ebuffer_config_t;

/* don't like this init function #todo */
void Emu_buffer_init(EBuffer *bfufer, char const *, cci64_t);

char *Emu_buffer_reserve_and_commit(EBuffer *,cci64_t   resv, cci64_t   comm);
char *Emu_buffer_insert(EBuffer *,cci64_t offset, cci64_t length);
void  Emu_buffer_remove(EBuffer *,cci64_t offset, cci64_t length);

void
ebuffer_build_lcache(
  EBuffer *buffer);
ccinle emarker_t
Emu_buffer_get_line_at_index(
  EBuffer *buffer, int index);
ccinle int
ebuffer_get_line_length(
  EBuffer *buffer, int yline);
ccinle int
ebuffer_get_line_offset(
  EBuffer *buffer, int yline);
