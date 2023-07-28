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

int
is_letter(char c)
{
  return
  ( CCWITHIN(c,'a','z') ||
    CCWITHIN(c,'A','Z') ||
    CCWITHIN(c,'0','9') || (c) == '_' );
}

int
is_word_delim(char c)
{
  return !is_letter(c);

  /* todo */
  return (c == ' ' )
      || (c == '\t')
      || (c == '\r')
      || (c == '\n');
}

char *
estring_copy_ex(
  const char *string, unsigned int length)
{
  char *result = ccnull;

  if(length != 0)
  {
    result = ccmalloc(length+1);
    result[length] = 0;

    if(string != 0)
      memcpy(result,string,length);
    else
      memset(result,     0,length);
  }

  return result;
}

char *
estring_copy(
  const char *string)
{
  if(string != ccnull)
  {
    return estring_copy_ex(string,strlen(string));
  }

  return ccnull;
}

