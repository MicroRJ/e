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

/* [[IMPLEMENTATION]] */

void
Editor_keyAll(EEditor *lp, E_KEY key) {
	for (key.cur=enumcur(lp)-1; key.cur>=0; key.cur-=1) {
		Editor_keyOne(lp,key);
	}
}

void
Editor_handleKey(EEditor *lp, E_KID kid, int num, int chr) {

	// printf("handle_key kid := %s num := %i chr := %i\n"
	// , E_KID_toStringMap[kid],num,chr);

	E_KEY key;
	key.mod = lp->mod;
	key.kid = kid;
	key.num = num;
	key.chr = chr;
	key.cur =  -1;
	Editor_keyAll(lp,key);
}

int
enumcur(EEditor *editor) {
	return arrlen(editor->cursor);
}

