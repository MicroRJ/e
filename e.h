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


/* todo: faster color logic */
/* todo: support for different fonts */
/* todo: basic formatting options */

typedef struct ecursor_t
{
  /* there's no need to store this per cursor, is there? */
  unsigned select: 1;
  int      selmin;
  int      selmax;

  int      xchar;
  int      yline;
} ecursor_t;

typedef struct erow_t
{
  int offset;
  int length;

  int indent;
} erow_t;

/* base widget */
typedef struct ewdg_t
{
  /* theme */
  int        focused;

  ccread_t    reader;

  ecursor_t   cursor;
  ebuffer_t   buffer;

  int         lyview;
  erow_t     *lcache;
} ewdg_t;

typedef struct e_editor_wdg_t
{
  ewdg_t    base;

  ccread_t  reader;
} e_editor_wdg_t;

typedef struct e_shell_wdg_t
{
  ewdg_t base;
} e_shell_wdg_t;

/* utilities */

