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
	E_MOD mod;

	ecursor_t *cursor;

/* we can do better #todo */
	float     *curinf;

	EBuffer  buffer;
	int        lyview;

	rlFont *font;

	double     last_event_timer;
	eevent_t * trail;
	eevent_t   event;

	float cursor_blink_speed_in_seconds;
	float cursor_blink_timer;
} Editor;


int
ecurnum(Editor *editor) {
	return ccarrlen(editor->cursor);
}

/* this is temporary */
void
Editor_testKeys(Editor *);

void
Editor_keyOne(Editor *wdg, E_KEY key);

void
Editor_keyAll(Editor *lp, E_KEY key) {
	for (key.cur=ecurnum(lp)-1; key.cur>=0; key.cur-=1) {
		Editor_keyOne(lp,key);
	}
}

void
Editor_handleKey(Editor *lp, E_KEY_NAME name, int num, int chr) {
	E_KEY key;
	key.mod = lp->mod;
	key.key = name;
	key.num = num;
	key.chr = chr;
	key.cur =  -1;
	Editor_keyAll(lp,key);
}






/* this is temporary */
int
eeditor_draw_text_run_callback(
void *user, int index, int *code, rxcolor_t *color);

/* cursor */
int
enumcur(
Editor *);
int
eaddcur(
Editor *, ecursor_t cur);
void
esetcur(
Editor *, int, ecursor_t cur);
/* todo: somewhat deprecated, if anything you should get a pointer to signify
that this is an integral part of the editor, in which case cursors should be
memory-locked */
ecursor_t
egetcur(
Editor *, int);
int
egetcurx(
Editor *, int);
int
egetcury(
Editor *, int);
int
ecurloc(
Editor *, int);
char
ecurchr(
Editor *, int, int off);
char *
ecurptr(
Editor *, int);
boxthing
ecurrec(
Editor *, int,
/* this is temporary, the widget should store this? */
boxthing);

char *
egetptr(
Editor *, int index);

/* commands */

int
eputchar(
Editor *,int index, int chr);
void
edelchar(
Editor *,int index);

/* these are to be implemented */
void
eaddrow(
Editor *, int offset, int number);
void
eremrow(
Editor *, int offset, int number);

void
eeditor_load(
Editor *, char const *);

int
eeditor_unload(
Editor *, char const *);

eevent_t
epopevn(
Editor *wdg)
{
	eevent_t evn = wdg->event;

	wdg->event.type = E_kNONE;

	if(earray_length(wdg->trail) != 0)
	{
		wdg->event = ccarrend(wdg->trail)[-1];

/* todo: don't actually do this */
		earray_remove(wdg->trail,ccarrlen(wdg->trail)-1,1);
	}

	return evn;
}
