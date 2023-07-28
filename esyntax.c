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
  esyntax_include_token(reader,"__asm",cctoken_Kmsvc_attr_asm);
  esyntax_include_token(reader,"__based",cctoken_Kmsvc_attr_based);
  esyntax_include_token(reader,"__cdecl",cctoken_Kmsvc_attr_cdecl);
  esyntax_include_token(reader,"__clrcall",cctoken_Kmsvc_attr_clrcall);
  esyntax_include_token(reader,"__fastcall",cctoken_Kmsvc_attr_fastcall);
  esyntax_include_token(reader,"__inline",cctoken_Kmsvc_attr_inline);
  esyntax_include_token(reader,"__stdcall",cctoken_Kmsvc_attr_stdcall);
  esyntax_include_token(reader,"__thiscall",cctoken_Kmsvc_attr_thiscall);
  esyntax_include_token(reader,"__vectorcal",cctoken_Kmsvc_attr_vectorcal);

  esyntax_include_token(reader,"_Alignof",cctoken_Kalign_of);
  esyntax_include_token(reader,"_Alignas",cctoken_Kalign_as);

  esyntax_include_token(reader,"const",cctoken_Kconst);
  esyntax_include_token(reader,"restrict",cctoken_Krestrict);
  esyntax_include_token(reader,"volatile",cctoken_Kvolatile);

  esyntax_include_token(reader,"inline",cctoken_Kinline);
  esyntax_include_token(reader,"_Noreturn",cctoken_Kno_return);

  esyntax_include_token(reader,"signed",cctoken_kSTDC_SIGNED);
  esyntax_include_token(reader,"unsigned",cctoken_kSTDC_UNSIGNED);
  esyntax_include_token(reader,"__int8",cctoken_kMSVC_INT8);
  esyntax_include_token(reader,"__int16",cctoken_kMSVC_INT16);
  esyntax_include_token(reader,"__int32",cctoken_kMSVC_INT32);
  esyntax_include_token(reader,"__int64",cctoken_kMSVC_INT64);
  esyntax_include_token(reader,"double",cctoken_kSTDC_DOUBLE);
  esyntax_include_token(reader,"float",cctoken_kSTDC_FLOAT);
  esyntax_include_token(reader,"long",cctoken_kSTDC_LONG);
  esyntax_include_token(reader,"int",cctoken_kSTDC_INT);
  esyntax_include_token(reader,"short",cctoken_kSTDC_SHORT);
  esyntax_include_token(reader,"char",cctoken_kSTDC_CHAR);
  esyntax_include_token(reader,"void",cctoken_kVOID);
  esyntax_include_token(reader,"_Bool",cctoken_kSTDC_BOOL);

  // esyntax_include_token(reader,"_Complex",cctoken_Kcomplex);
  // esyntax_include_token(reader,"_Atomic",cctoken_Katomic);

  esyntax_include_token(reader,"enum",cctoken_kENUM);
  esyntax_include_token(reader,"struct",cctoken_kSTRUCT);

  esyntax_include_token(reader,"typedef",cctoken_Ktypedef);

  esyntax_include_token(reader,"sizeof",cctoken_kSIZEOF);

  esyntax_include_token(reader,"auto",cctoken_Kauto);
  esyntax_include_token(reader,"extern",cctoken_Kextern);
  esyntax_include_token(reader,"register",cctoken_Kregister);
  esyntax_include_token(reader,"static",cctoken_Kstatic);
  esyntax_include_token(reader,"_Thread_local",cctoken_Kthread_local);
  esyntax_include_token(reader,"__declspec",cctoken_Kmsvc_declspec);

  esyntax_include_token(reader,"if",cctoken_kIF);
  esyntax_include_token(reader,"switch",cctoken_Kswitch);
  esyntax_include_token(reader,"else",cctoken_kELSE);
  esyntax_include_token(reader,"case",cctoken_Kcase);
  esyntax_include_token(reader,"default",cctoken_Kdefault);
  esyntax_include_token(reader,"for",cctoken_kFOR);
  esyntax_include_token(reader,"while",cctoken_kWHILE);
  esyntax_include_token(reader,"do",cctoken_Kdo);
  esyntax_include_token(reader,"goto",cctoken_kGOTO);
  esyntax_include_token(reader,"return",cctoken_kRETURN);
  esyntax_include_token(reader,"break",cctoken_Kbreak);
  esyntax_include_token(reader,"continue",cctoken_Kcontinue);

  ccassert(ccerrnon());
}

int
esyntax_get_token_style(esyntax_t *l, int length, char const *string, int *advance)
{
  int the_token = cctoken_kINVALID;

  char const *cursor = string;

  if(length <= 0)
  {
    the_token = cctoken_kEND;
    goto leave;
  }

  switch(cursor[0])
  { default:
    { cursor += 1;
      the_token = cctoken_kINVALID;
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

        // Todo: should we instead store the two parts?
        the_token=cctoken_kLITFLO;
        // l->tok.real=u+d/p;
      } else
      { the_token=cctoken_kLITINT;
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

      the_token = e->name ? e->type : cctoken_kLITIDENT;
    } break;
    case '"':
    {
      the_token = cctoken_kLITSTR;

      cursor += 1;
      while((cursor < string + length) &&
            (*cursor != '"')) cursor += 1;

      cursor += 1;

    } break;
    case ':':
    { ++ cursor, the_token = cctoken_kCOLON;
    } break;
    case ',':
    { ++ cursor, the_token = cctoken_kCMA;
    } break;
    case '(':
    { ++ cursor, the_token = cctoken_kLPAREN;
    } break;
    case ')':
    { ++ cursor, the_token = cctoken_kRPAREN;
    } break;
    case '[':
    { ++ cursor, the_token = cctoken_kLSQUARE;
    } break;
    case ']':
    { ++ cursor, the_token = cctoken_kRSQUARE;
    } break;
    case '{':
    { ++ cursor, the_token = cctoken_kLCURLY;
    } break;
    case '}':
    { ++ cursor, the_token = cctoken_kRCURLY;
    } break;
    case '~':
    {
      cursor += 1, the_token = cctoken_kBWINV;
    } break;
    // .
    // ...
    case '.':
    { if(cursor[1] == '.' && cursor[2] == '.')
      { cursor += 3, the_token = cctoken_Kliteral_ellipsis;
      } else
      { cursor += 1, the_token = cctoken_kDOT;
      }
    } break;
    // ^
    // ^=
    case '^':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kBWXOR_EQL;
      } else
      { cursor += 1, the_token = cctoken_kBWXOR;
      }
    } break;
    // ||
    // |=
    // |
    case '|':
    {
      if(cursor[1]=='|')
      { cursor += 2, the_token = cctoken_kLGOR;
      } else
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kBWOR_EQL;
      } else
      { cursor += 1, the_token = cctoken_kBWOR;
      }
    } break;
    // &&
    // &
    // &=
    case '&':
    {
      if(cursor[1]=='&')
      { cursor += 2, the_token = cctoken_kLGAND;
      } else
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kBWAND_EQL;
      } else
      { cursor += 1, the_token = cctoken_kBWAND;
      }
    } break;
    case '/':
    { if(cursor[1] == '/')
      {
        cursor += 2;
        the_token = cctoken_kCOMMENT;

        while((cursor < string + length) &&
          (cursor[0] != '\r') &&
          (cursor[0] != '\n')) cursor += 1;
      } else
      if(cursor[1] == '*')
      {
        cursor += 2;
        the_token = cctoken_kCOMMENT;

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
      { cursor += 2, the_token = cctoken_kDIV_EQL;
      } else
      { cursor += 1, the_token = cctoken_kDIV;
      }
    } break;
    // *=
    // *
    case '*':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kMUL_EQL;
      } else
      { cursor += 1, the_token = cctoken_kMUL;
      }
    } break;
    // +=,+
    case '+':
    { if(cursor[1]=='=')
        cursor += 2, the_token = cctoken_kADD_EQL;
      else
        cursor += 1, the_token = cctoken_kADD;
    } break;
    // -=,-
    case '-':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kSUB_EQL;
      } else
      { cursor += 1, the_token = cctoken_kSUB;
      }
    } break;
    // ==
    // =
    case '=':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kTEQ;
      } else
      { cursor += 1, the_token = cctoken_kASSIGN;
      }
    } break;
    // !=
    // !
    case '!':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kFEQ;
      } else
      { cursor += 2, the_token = cctoken_kLGNEG;
      }
    } break;
    // >=
    // >
    case '>':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kGTE;
      } else
      if(cursor[1]=='>')
      { cursor += 2, the_token = cctoken_kBWSHR;
      } else
      { cursor += 1, the_token = cctoken_kGTN;
      }
    } break;
    // <=
    // <
    case '<':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kLTE;
      } else
      if(cursor[1]=='<')
      { cursor += 2, the_token = cctoken_kBWSHL;
      } else
      { cursor += 1, the_token = cctoken_kLTN;
      }
    } break;
    // %=
    // %
    case '%':
    {
      if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kMOD_EQL;
      } else
      { cursor += 1, the_token = cctoken_kMOD;
      }
    } break;

    case '?':
    { if(cursor[1]=='=')
      { cursor += 2, the_token = cctoken_kINVALID;
      } else
      { cursor += 2, the_token = cctoken_kINVALID;
      }
    } break;
    case '\0':
    { cursor += 1, the_token = cctoken_kEND;
    } break;
  }

leave:;

  *advance = cursor - string;
  return the_token;
}

