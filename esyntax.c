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

void
esyntax_init(esyntax_t *syntax)
{
	syntax->sampler = esyntax_sampler_default_impl;

	for(int i=0; i<ccCarrlenL(syntax->color_table); i+=1)
	{
		syntax->color_table[i] = RX_RGBA_UNORM(122,104,81,255);
	}

  syntax->color_table[ETOKEN_kCOMMENT]       = RX_COLOR_SILVER;

  syntax->color_table[ETOKEN_kSTRING]        = RX_COLOR_TEAL;
  syntax->color_table[ETOKEN_kINTEGER]       = RX_COLOR_CYAN;
  syntax->color_table[ETOKEN_kFLOAT]         = RX_COLOR_CYAN;

  syntax->color_table[ETOKEN_kIF]            = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kELSE]          = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kSWITCH]        = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kCASE]          = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kDEFAULT]       = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kFOR]           = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kWHILE]         = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kDO]            = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kGOTO]          = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kRETURN]        = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kBREAK]         = RX_COLOR_RED;
  syntax->color_table[ETOKEN_kCONTINUE]      = RX_COLOR_RED;

  syntax->color_table[ETOKEN_kVOID]          = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kCONST]         = RX_COLOR_MAGENTA;

  syntax->color_table[ETOKEN_kSTDC_INT]      = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_LONG]     = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_SHORT]    = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_DOUBLE]   = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_FLOAT]    = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_CHAR]     = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_BOOL]     = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_SIGNED]   = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTDC_UNSIGNED] = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kMSVC_INT8]     = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kMSVC_INT16]    = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kMSVC_INT32]    = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kMSVC_INT64]    = RX_COLOR_MAGENTA;

  syntax->color_table[ETOKEN_kENUM]          = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTRUCT]        = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kTYPEDEF]       = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kAUTO]          = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kEXTERN]        = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kREGISTER]      = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kSTATIC]        = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kTHREAD_LOCAL]  = RX_COLOR_MAGENTA;
  syntax->color_table[ETOKEN_kMSVC_DECLSPEC] = RX_COLOR_MAGENTA;

	/* these hashes will instead be auto-generated at compile-time, eventually */
  esyntax_register_keyword(syntax,"__asm",         ETOKEN_kMSVC_ATTR_asm);
  esyntax_register_keyword(syntax,"__based",       ETOKEN_kMSVC_ATTR_based);
  esyntax_register_keyword(syntax,"__cdecl",       ETOKEN_kMSVC_ATTR_cdecl);
  esyntax_register_keyword(syntax,"__clrcall",     ETOKEN_kMSVC_ATTR_clrcall);
  esyntax_register_keyword(syntax,"__fastcall",    ETOKEN_kMSVC_ATTR_fastcall);
  esyntax_register_keyword(syntax,"__inline",      ETOKEN_kMSVC_ATTR_inline);
  esyntax_register_keyword(syntax,"__stdcall",     ETOKEN_kMSVC_ATTR_stdcall);
  esyntax_register_keyword(syntax,"__thiscall",    ETOKEN_kMSVC_ATTR_thiscall);
  esyntax_register_keyword(syntax,"__vectorcal",   ETOKEN_kMSVC_ATTR_vectorcal);
  esyntax_register_keyword(syntax,"_Alignof",      ETOKEN_kALIGNOF);
  esyntax_register_keyword(syntax,"_Alignas",      ETOKEN_kALIGNAS);
  esyntax_register_keyword(syntax,"const",         ETOKEN_kCONST);
  esyntax_register_keyword(syntax,"restrict",      ETOKEN_kRESTRICT);
  esyntax_register_keyword(syntax,"volatile",      ETOKEN_kVOLATILE);
  esyntax_register_keyword(syntax,"inline",        ETOKEN_kINLINE);
  esyntax_register_keyword(syntax,"_Noreturn",     ETOKEN_kNO_RETURN);
  esyntax_register_keyword(syntax,"signed",        ETOKEN_kSTDC_SIGNED);
  esyntax_register_keyword(syntax,"unsigned",      ETOKEN_kSTDC_UNSIGNED);
  esyntax_register_keyword(syntax,"__int8",        ETOKEN_kMSVC_INT8);
  esyntax_register_keyword(syntax,"__int16",       ETOKEN_kMSVC_INT16);
  esyntax_register_keyword(syntax,"__int32",       ETOKEN_kMSVC_INT32);
  esyntax_register_keyword(syntax,"__int64",       ETOKEN_kMSVC_INT64);
  esyntax_register_keyword(syntax,"double",        ETOKEN_kSTDC_DOUBLE);
  esyntax_register_keyword(syntax,"float",         ETOKEN_kSTDC_FLOAT);
  esyntax_register_keyword(syntax,"long",          ETOKEN_kSTDC_LONG);
  esyntax_register_keyword(syntax,"int",           ETOKEN_kSTDC_INT);
  esyntax_register_keyword(syntax,"short",         ETOKEN_kSTDC_SHORT);
  esyntax_register_keyword(syntax,"char",          ETOKEN_kSTDC_CHAR);
  esyntax_register_keyword(syntax,"void",          ETOKEN_kVOID);
  esyntax_register_keyword(syntax,"_Bool",         ETOKEN_kSTDC_BOOL);
  esyntax_register_keyword(syntax,"enum",          ETOKEN_kENUM);
  esyntax_register_keyword(syntax,"struct",        ETOKEN_kSTRUCT);
  esyntax_register_keyword(syntax,"typedef",       ETOKEN_kTYPEDEF);
  esyntax_register_keyword(syntax,"sizeof",        ETOKEN_kSIZEOF);
  esyntax_register_keyword(syntax,"auto",          ETOKEN_kAUTO);
  esyntax_register_keyword(syntax,"extern",        ETOKEN_kEXTERN);
  esyntax_register_keyword(syntax,"register",      ETOKEN_kREGISTER);
  esyntax_register_keyword(syntax,"static",        ETOKEN_kSTATIC);
  esyntax_register_keyword(syntax,"_Thread_local", ETOKEN_kTHREAD_LOCAL);
  esyntax_register_keyword(syntax,"__declspec",    ETOKEN_kMSVC_DECLSPEC);
  esyntax_register_keyword(syntax,"if",            ETOKEN_kIF);
  esyntax_register_keyword(syntax,"switch",        ETOKEN_kSWITCH);
  esyntax_register_keyword(syntax,"else",          ETOKEN_kELSE);
  esyntax_register_keyword(syntax,"case",          ETOKEN_kCASE);
  esyntax_register_keyword(syntax,"default",       ETOKEN_kDEFAULT);
  esyntax_register_keyword(syntax,"for",           ETOKEN_kFOR);
  esyntax_register_keyword(syntax,"while",         ETOKEN_kWHILE);
  esyntax_register_keyword(syntax,"do",            ETOKEN_kDO);
  esyntax_register_keyword(syntax,"goto",          ETOKEN_kGOTO);
  esyntax_register_keyword(syntax,"return",        ETOKEN_kRETURN);
  esyntax_register_keyword(syntax,"break",         ETOKEN_kBREAK);
  esyntax_register_keyword(syntax,"continue",      ETOKEN_kCONTINUE);

  ccassert(ccerrnon());
}

esample_t
esyntax_sampler_default_impl(
  esyntax_t *syntax, void *_, int offset, int length, char const *string)
{
	esample_t sample;
  esyntax_get_token_info(
  	syntax,length,string,
  		&sample.token,&sample.width);

  return sample;
}

void
esyntax_get_token_info(esyntax_t *l,
	int length, char const *string, int *token, int *width)
{
  int the_token = ETOKEN_kINVALID;
  int the_width = 1;

  char const *cursor = string;

  if(length <= 0)
  {
    the_token = ETOKEN_kEND;
    the_width = 0;
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

      esyntax_keyword_t *e=cctblsetP(l->keyword,h);

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

  *width = the_width = cursor - string;
  *token = the_token;
}

