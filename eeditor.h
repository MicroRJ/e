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


enum
{ kNONE,
  kCHAR,
  kUNDO,
  kREDO, };

typedef struct
{
  int       xchar;
  int       yline;
} ecursor_t;

typedef struct
{
  char      type;

  struct
  {
    /* store a copy of the cursor in case we have to restore it */
    ecursor_t cursor;
    int       length; }; /* optional */
} eevent_t;

typedef struct
{
  int offset;
  int length;
  /* remove */
  int indent;
} ecurrow_t;

typedef struct
{
  ewidget_t  widget;

  esyntax_t  syntax;

  ecursor_t *cursor;/* todo */

  /* todo: we can do better */
  float     *curinf;

  ebuffer_t  buffer;
  int        lyview;
  ecurrow_t *lcache;

  efont      font;
  float      text_size;

  float      tab_size_in_spaces;

  double     last_event_timer;
  eevent_t * trail;
  eevent_t   event;

  /* todo: this is temporary */
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
/* todo: somewhat deprecated, if anything you should get a pointer to signify
 that this is an integral part of the editor, in which case cursors should be
 memory-locked */
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
ecurrow_t
egetrow(
  eeditor_t *, int line);
int
egetlen(
  eeditor_t *, int line);
int
erowloc(
  eeditor_t *, int line);

char *
egetptr(
  eeditor_t *, int index);

/* commands */

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

eevent_t
epopevn(
  eeditor_t *wdg)
{
  eevent_t evn = wdg->event;

  wdg->event.type = kNONE;

  if(earray_length(wdg->trail) != 0)
  {
    wdg->event = ccarrend(wdg->trail)[-1];

    /* todo: don't actually do this */
    earray_remove(wdg->trail,ccarrlen(wdg->trail)-1,1);
  }

  return evn;
}
