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

#define earray_length(arr) ccarrlen(arr)
#define earray_add(arr,num) rlArray_add(arr,num)

#define earray_delete(arr) ccarrdel(arr)

#define earray_remove(arr,off,num) ((arr) + earray_remove_((void**)(&arr),sizeof(*arr),off,num))
#define earray_insert(arr,off,num) ((arr) + earray_insert_((void**)(&arr),sizeof(*arr),off,num))

cci64_t
earray_remove_(void **lpp, cci32_t itm, cci64_t off, cci64_t num)
{
  ccdlb_t *dlb = ccdlb_(*lpp);

  char *mem = *lpp;

  if(off != dlb->sze_min)
  {
    memmove(
      mem + itm * off,
      mem + itm * (off + num), itm * (dlb->sze_min - off - num));
  }

  dlb->sze_min -= num;

  return off;
}

cci64_t
earray_insert_(void **lpp, cci32_t itm, cci64_t off, cci64_t num)
{
  ccdlbadd_(lpp,itm,num,num);

  ccdlb_t *dlb = ccdlb_(*lpp);

  char *mem = *lpp;

  if(off != dlb->sze_min)
  {
    memmove(
      mem + itm * (off + num),
      mem + itm * off, dlb->sze_min - off - num);
  }

  return off;
}
