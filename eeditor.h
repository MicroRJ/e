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


typedef struct ecursor_t
{
  int      xchar;
  int      yline;

  /* there's no need to store this per cursor, is there? */
  unsigned select: 1;
  int      selmin;
  int      selmax;

} ecursor_t;

typedef struct erow_t
{
  int offset;
  int length;
  /* remove */
  int indent;
} erow_t;

typedef struct eeditor_t
{
  ewidget_t widget;
  esyntax_t syntax;

  ecursor_t *cursor;/* todo */

  ebuffer_t  buffer;
  int        lyview;
  erow_t    *lcache;


  struct
  {
    /* the y index of the line we're on */
    int       yline;
    /* the current color of the token */
    rxcolor_t color;
    /* the remaining range of the current style */
    int       reach;
  } style;
} eeditor_t;

/* this is temporary */
void
eeditor_msg(
  eeditor_t *);

/* this is temporary */
int
eeditor_draw_text_run_callback(
  void *user, int index, int *code, rxcolor_t *color);

/* this is temporary */
void
erecache(
  eeditor_t *);

/* cursor */
int
enumcur(
  eeditor_t *);
int
eaddcur(
  eeditor_t *, ecursor_t cur);
void
esetcur(
  eeditor_t *, int, ecursor_t cur);
void
esetcurx(
  eeditor_t *, int, int xchar);
void
esetcury(
  eeditor_t *, int, int yline);
ecursor_t
egetcur(
  eeditor_t *, int);
int
egetcurx(
  eeditor_t *, int);
int
egetcury(
  eeditor_t *, int);
int
ecurloc(
  eeditor_t *, int);
char
ecurchr(
  eeditor_t *, int, int off);
char *
ecurptr(
  eeditor_t *, int);
erect_t
ecurrec(
  eeditor_t *, int,
  /* this is temporary, the widget should store this? */
    erect_t);

/* buffer */
erow_t
egetrow(
  eeditor_t *, int line);
int
egetlen(
  eeditor_t *, int line);
int
egetoff(
  eeditor_t *, int line);

char *
egetptr(
  eeditor_t *, int index);
char *
egetend(
  eeditor_t *);

/* commands */
void
emovcurx(
  eeditor_t *,int index, int mov);
void
emovcury(
  eeditor_t *,int index, int mov);

int
eputchar(
  eeditor_t *,int index, int chr);
void
edelchar(
  eeditor_t *,int index);

/* these are to be implemented */
void
eaddrow(
  eeditor_t *, int offset, int number);
void
eremrow(
  eeditor_t *, int offset, int number);

void
eeditor_load(
  eeditor_t *, char const *);

int
eeditor_unload(
  eeditor_t *, char const *);



