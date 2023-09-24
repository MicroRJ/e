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

#define rlArray_remove(arr,off,num) ((arr) + earray_remove_((void**)(&arr),sizeof(*arr),off,num))
#define rlArray_insert(arr,off,num) ((arr) + earray_insert_((void**)(&arr),sizeof(*arr),off,num))

__int64
earray_remove_(void **lpp, __int32 itemSize, __int64 off, __int64 num)
{
	ccdlb_t *dlb = ccdlb_(*lpp);

	char *mem = *lpp;

	if(off != dlb->sze_min)
	{
		memmove(
		mem + itemSize * off,
		mem + itemSize * (off + num), itemSize * (dlb->sze_min - off - num));
	}

	dlb->sze_min -= num;

	return off;
}

__int64
earray_insert_(void **lpp, __int32 itemSize, __int64 off, __int64 num)
{
	ccdlbadd_(lpp,itemSize,num,num);

	ccdlb_t *dlb = ccdlb_(*lpp);

	char *mem = *lpp;

	if(off != dlb->sze_min)
	{
		memmove(
		mem + itemSize * (off + num),
		mem + itemSize * off, dlb->sze_min - off - num);
	}

	return off;
}
