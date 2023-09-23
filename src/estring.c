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

#define IS_LETTER(r) ( CCWITHIN(r,'a','z')|| \
CCWITHIN(r,'A','Z')|| \
CCWITHIN(r,'0','9')|| (r) == '_' )

#define IS_WHITESPACE(chr) ( (chr ==  ' ') ||\
(chr == '\t') ||\
(chr == '\r') ||\
(chr == '\n') )

#define IS_WORD_DELIMETER(r) (!IS_LETTER(r))
