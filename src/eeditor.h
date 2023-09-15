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

enum {
	EDITOR_kNONE = 0,

	EDITOR_kMOD_CONTROL,
	EDITOR_kMOD_SHIFT,
	EDITOR_kMOD_ALT,
	EDITOR_kMOD_INSERT,

	EDITOR_kMOVE_LEFT,
	EDITOR_kMOVE_UP,
	EDITOR_kMOVE_RIGHT,
	EDITOR_kMOVE_DOWN,

	EDITOR_kMOVE_WORD_LEFT,
	EDITOR_kMOVE_WORD_RIGHT,

	EDITOR_kMOVE_LINE_LEFT,
	EDITOR_kMOVE_LINE_RIGHT,

	EDITOR_kMOVE_PAGE_START,
	EDITOR_kMOVE_PAGE_END,

	EDITOR_kDELETE_BACK,
	EDITOR_kDELETE_HERE,

	EDITOR_kCUT,
	EDITOR_kCOPY,
	EDITOR_kPASTE,

	EDITOR_kLINE,
	EDITOR_kCHAR,
};

typedef struct {
	int xchar;
	int yline;
} ecursor_t;

typedef struct {
	char      type;

	struct {
/* store a copy of the cursor in case we have to restore it */
		ecursor_t cursor;
		int       length;
	};
} eevent_t;


typedef struct {
	unsigned is_control: 1;
	unsigned is_insert:  1;
	unsigned is_shift:   1;
	unsigned is_alt:     1;

	ecursor_t *cursor;

/* we can do better #todo */
	float     *curinf;

	ebuffer_t  buffer;
	int        lyview;

	rxFont *font;

	double     last_event_timer;
	eevent_t * trail;
	eevent_t   event;

	float cursor_blink_speed_in_seconds;
	float cursor_blink_timer;
} eeditor_t;

/* this is temporary */
void
eeditor_msg(
eeditor_t *);

/* this is temporary */
int
eeditor_draw_text_run_callback(
void *user, int index, int *code, rxcolor_t *color);

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
rlWindowBox
ecurrec(
eeditor_t *, int,
/* this is temporary, the widget should store this? */
rlWindowBox);

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

	wdg->event.type = EDITOR_kNONE;

	if(earray_length(wdg->trail) != 0)
	{
		wdg->event = ccarrend(wdg->trail)[-1];

/* todo: don't actually do this */
		earray_remove(wdg->trail,ccarrlen(wdg->trail)-1,1);
	}

	return evn;
}
