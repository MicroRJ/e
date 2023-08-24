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
EMU_load_file_data(
	char const *fpath, size_t *lpsize)
{
  void *file = ccopenfile(fpath,"r");

  ccu32_t length = 0;
  void *memory = ccpullfile(file,0,&length);

  if(lpsize) {
  	*lpsize = length;
  }

  return memory;
}
