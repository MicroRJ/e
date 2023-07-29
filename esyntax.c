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

#define IS_LETTER(r)\
  ( CCWITHIN(r,'a','z')|| \
    CCWITHIN(r,'A','Z')|| \
    CCWITHIN(r,'0','9')||(r)=='_' )

void
esyntax_include_token(esyntax_t *syntax, char const *name, int type)
{
  ccu64_t h=5381;

  int c,n;
  for(n=0;c=name[n];++n)
  { ccassert(IS_LETTER(c));
    if(c!=0)
      h=h<<5,h=h+c;
    else
      break;
  }

  cctoken_t *entry=cctblputP(syntax->table,h);
  entry->type=type;
  entry->name=name;
}


void
esyntax_init(esyntax_t *reader)
{
	/* these hashes will instead be auto-generated at compile-time, eventually */
  esyntax_include_token(reader,"__asm",ETOKEN_kMSVC_ATTR_asm);
  esyntax_include_token(reader,"__based",ETOKEN_kMSVC_ATTR_based);
  esyntax_include_token(reader,"__cdecl",ETOKEN_kMSVC_ATTR_cdecl);
  esyntax_include_token(reader,"__clrcall",ETOKEN_kMSVC_ATTR_clrcall);
  esyntax_include_token(reader,"__fastcall",ETOKEN_kMSVC_ATTR_fastcall);
  esyntax_include_token(reader,"__inline",ETOKEN_kMSVC_ATTR_inline);
  esyntax_include_token(reader,"__stdcall",ETOKEN_kMSVC_ATTR_stdcall);
  esyntax_include_token(reader,"__thiscall",ETOKEN_kMSVC_ATTR_thiscall);
  esyntax_include_token(reader,"__vectorcal",ETOKEN_kMSVC_ATTR_vectorcal);

  esyntax_include_token(reader,"_Alignof",ETOKEN_kALIGNOF);
  esyntax_include_token(reader,"_Alignas",ETOKEN_kALIGNAS);

  esyntax_include_token(reader,"const",ETOKEN_kCONST);
  esyntax_include_token(reader,"restrict",ETOKEN_kRESTRICT);
  esyntax_include_token(reader,"volatile",ETOKEN_kVOLATILE);

  esyntax_include_token(reader,"inline",ETOKEN_kINLINE);
  esyntax_include_token(reader,"_Noreturn",ETOKEN_kNO_RETURN);

  esyntax_include_token(reader,"signed",ETOKEN_kSTDC_SIGNED);
  esyntax_include_token(reader,"unsigned",ETOKEN_kSTDC_UNSIGNED);
  esyntax_include_token(reader,"__int8",ETOKEN_kMSVC_INT8);
  esyntax_include_token(reader,"__int16",ETOKEN_kMSVC_INT16);
  esyntax_include_token(reader,"__int32",ETOKEN_kMSVC_INT32);
  esyntax_include_token(reader,"__int64",ETOKEN_kMSVC_INT64);
  esyntax_include_token(reader,"double",ETOKEN_kSTDC_DOUBLE);
  esyntax_include_token(reader,"float",ETOKEN_kSTDC_FLOAT);
  esyntax_include_token(reader,"long",ETOKEN_kSTDC_LONG);
  esyntax_include_token(reader,"int",ETOKEN_kSTDC_INT);
  esyntax_include_token(reader,"short",ETOKEN_kSTDC_SHORT);
  esyntax_include_token(reader,"char",ETOKEN_kSTDC_CHAR);
  esyntax_include_token(reader,"void",ETOKEN_kVOID);
  esyntax_include_token(reader,"_Bool",ETOKEN_kSTDC_BOOL);

  // esyntax_include_token(reader,"_Complex",ETOKEN_kcomplex);
  // esyntax_include_token(reader,"_Atomic",ETOKEN_katomic);

  esyntax_include_token(reader,"enum",ETOKEN_kENUM);
  esyntax_include_token(reader,"struct",ETOKEN_kSTRUCT);

  esyntax_include_token(reader,"typedef",ETOKEN_kTYPEDEF);

  esyntax_include_token(reader,"sizeof",ETOKEN_kSIZEOF);

  esyntax_include_token(reader,"auto",ETOKEN_kAUTO);
  esyntax_include_token(reader,"extern",ETOKEN_kEXTERN);
  esyntax_include_token(reader,"register",ETOKEN_kREGISTER);
  esyntax_include_token(reader,"static",ETOKEN_kSTATIC);
  esyntax_include_token(reader,"_Thread_local",ETOKEN_kTHREAD_LOCAL);
  esyntax_include_token(reader,"__declspec",ETOKEN_kMSVC_DECLSPEC);

  esyntax_include_token(reader,"if",ETOKEN_kIF);
  esyntax_include_token(reader,"switch",ETOKEN_kSWITCH);
  esyntax_include_token(reader,"else",ETOKEN_kELSE);
  esyntax_include_token(reader,"case",ETOKEN_kCASE);
  esyntax_include_token(reader,"default",ETOKEN_kDEFAULT);
  esyntax_include_token(reader,"for",ETOKEN_kFOR);
  esyntax_include_token(reader,"while",ETOKEN_kWHILE);
  esyntax_include_token(reader,"do",ETOKEN_kDO);
  esyntax_include_token(reader,"goto",ETOKEN_kGOTO);
  esyntax_include_token(reader,"return",ETOKEN_kRETURN);
  esyntax_include_token(reader,"break",ETOKEN_kBREAK);
  esyntax_include_token(reader,"continue",ETOKEN_kCONTINUE);

  ccassert(ccerrnon());
}

int
esyntax_get_token_style(esyntax_t *l, int length, char const *string, int *advance)
{
  int the_token = ETOKEN_kINVALID;

  char const *cursor = string;

  if(length <= 0)
  {
    the_token = ETOKEN_kEND;
    goto leave;
  }

  switch(cursor[0])
  { default:
    { cursor += 1;
      the_token = ETOKEN_kINVALID;
    } break;

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
      cci32_t b;
      if(cursor[0] == '0')
      { switch (cursor[1])
        { case 'h':
          case 'x':
          { b = 0x10;
            cursor += 2;
          } break;
          case 'd':
          { b = 0x0A;
            cursor += 2;
          } break;
          case 'o':
          { b = 0x08;
            cursor += 2;
          } break;
          case 'b':
          { b = 0x02;
            cursor += 2;
          } break;
          default:
          { b = 0x0A;
          } break;
        }
      } else
      { b = 0x0A;
      }
      ccu64_t u = 0;

      while(cursor < string + length)
      {
        if((*cursor >= '0' && *cursor <= '9'))
        { u = u * b + 0x00 + *cursor++ - '0';
        } else
        if((*cursor >= 'a' && *cursor <= 'f'))
        { u = u * b + 0x0A + *cursor++ - 'a';
        } else
        if((*cursor >= 'A' && *cursor <= 'F'))
        { u = u * b + 0x0A + *cursor++ - 'A';
        } else
        { break;
        }

      }
      if(*cursor == '.')
      { ++cursor;

        ccf64_t p = 1;
        ccu64_t d = 0;

        for(; cursor < string + length; p *= b)
        { if((*cursor >= '0' && *cursor <= '9'))
          { d = d * b + 0x00 + *cursor++ - '0';
          } else
          if((*cursor >= 'a' && *cursor <= 'f'))
          { d = d * b + 0x0A + *cursor++ - 'a';
          } else
          if((*cursor >= 'A' && *cursor <= 'F'))
          { d = d * b + 0x0A + *cursor++ - 'A';
          } else
          { break;
          }
        }

        the_token=ETOKEN_kFLOAT;
        // l->tok.real=u+d/p;
      } else
      { the_token=ETOKEN_kINTEGER;
        // l->tok.name=(char*)u;
      }
    } break;

    case('A'): case('B'): case('C'): case('D'): case('E'): case('F'):
    case('G'): case('H'): case('I'): case('J'): case('K'): case('L'):
    case('M'): case('N'): case('O'): case('P'): case('Q'): case('R'):
    case('S'): case('T'): case('U'): case('V'): case('W'): case('X'):
    case('Y'): case('Z'):
    case('a'): case('b'): case('c'): case('d'): case('e'): case('f'):
    case('g'): case('h'): case('i'): case('j'): case('k'): case('l'):
    case('m'): case('n'): case('o'): case('p'): case('q'): case('r'):
    case('s'): case('t'): case('u'): case('v'): case('w'): case('x'):
    case('y'): case('z'):
    case('_'):
    {
      ccu64_t h=5381;
      do
      {
        h <<= 5;
        h  += *cursor ++;
      } while(IS_LETTER(*cursor));

      cctoken_t *e=cctblsetP(l->table,h);

      the_token = e->name ? e->type : ETOKEN_kIDENTIFIER;
    } break;
    case '"':
    {
      the_token = ETOKEN_kSTRING;

      cursor += 1;
      while((cursor < string + length) &&
            (*cursor != '"')) cursor += 1;

      cursor += 1;

    } break;
    case ':':
    { ++ cursor, the_token = ETOKEN_kCOLON;
    } break;
    case ',':
    { ++ cursor, the_token = ETOKEN_kCOMMA;
    } break;
    case '(':
    { ++ cursor, the_token = ETOKEN_kLPAREN;
    } break;
    case ')':
    { ++ cursor, the_token = ETOKEN_kRPAREN;
    } break;
    case '[':
    { ++ cursor, the_token = ETOKEN_kLSQUARE;
    } break;
    case ']':
    { ++ cursor, the_token = ETOKEN_kRSQUARE;
    } break;
    case '{':
    { ++ cursor, the_token = ETOKEN_kLCURLY;
    } break;
    case '}':
    { ++ cursor, the_token = ETOKEN_kRCURLY;
    } break;
    case '~':
    {
      cursor += 1, the_token = ETOKEN_kBWINV;
    } break;
    // .
    // ...
    case '.':
    { if(cursor[1] == '.' && cursor[2] == '.')
      { cursor += 3, the_token = ETOKEN_kELLIPSIS;
      } else
      { cursor += 1, the_token = ETOKEN_kDOT;
      }
    } break;
    // ^
    // ^=
    case '^':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kBWXOR_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kBWXOR;
      }
    } break;
    // ||
    // |=
    // |
    case '|':
    {
      if(cursor[1]=='|')
      { cursor += 2, the_token = ETOKEN_kLGOR;
      } else
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kBWOR_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kBWOR;
      }
    } break;
    // &&
    // &
    // &=
    case '&':
    {
      if(cursor[1]=='&')
      { cursor += 2, the_token = ETOKEN_kLGAND;
      } else
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kBWAND_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kBWAND;
      }
    } break;
    case '/':
    { if(cursor[1] == '/')
      {
        cursor += 2;
        the_token = ETOKEN_kCOMMENT;

        while((cursor < string + length) &&
          (cursor[0] != '\r') &&
          (cursor[0] != '\n')) cursor += 1;
      } else
      if(cursor[1] == '*')
      {
        cursor += 2;
        the_token = ETOKEN_kCOMMENT;

        while((cursor < string + length - 1))
        {
          if((cursor[0] == '*') &&
             (cursor[1] == '/'))
          {
            cursor += 2;
            break;
          } else
          {
            cursor += 1;
          }

        }

      } else
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kDIV_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kDIV;
      }
    } break;
    // *=
    // *
    case '*':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kMUL_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kMUL;
      }
    } break;
    // +=,+
    case '+':
    { if(cursor[1]=='=')
        cursor += 2, the_token = ETOKEN_kADD_EQL;
      else
        cursor += 1, the_token = ETOKEN_kADD;
    } break;
    // -=,-
    case '-':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kSUB_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kSUB;
      }
    } break;
    // ==
    // =
    case '=':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kTEQ;
      } else
      { cursor += 1, the_token = ETOKEN_kASSIGN;
      }
    } break;
    // !=
    // !
    case '!':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kFEQ;
      } else
      { cursor += 2, the_token = ETOKEN_kLGNEG;
      }
    } break;
    // >=
    // >
    case '>':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kGTE;
      } else
      if(cursor[1]=='>')
      { cursor += 2, the_token = ETOKEN_kBWSHR;
      } else
      { cursor += 1, the_token = ETOKEN_kGTN;
      }
    } break;
    // <=
    // <
    case '<':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kLTE;
      } else
      if(cursor[1]=='<')
      { cursor += 2, the_token = ETOKEN_kBWSHL;
      } else
      { cursor += 1, the_token = ETOKEN_kLTN;
      }
    } break;
    // %=
    // %
    case '%':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kMOD_EQL;
      } else
      { cursor += 1, the_token = ETOKEN_kMOD;
      }
    } break;

    case '?':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = ETOKEN_kINVALID;
      } else
      { cursor += 2, the_token = ETOKEN_kINVALID;
      }
    } break;
    case '\0':
    { cursor += 1, the_token = ETOKEN_kEND;
    } break;
  }

leave:;

  *advance = cursor - string;
  return the_token;
}

