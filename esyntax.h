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

/* I think this is going to be largely temporary, I think for the most part
 this is going to be auto-generated, I would like to have a file where you
 describe the language and then generate a c executable (dll) that we can
 plugin even at runtime that does the work, instead of wasting time creating something
 incredibly repetitive and tiresome for each language, and you end up with something
 that could be re-used later, and if you want to create your own more efficient parser
 then we would allow for that too. */

/* todo */
typedef struct cctoken_t cctoken_t;
typedef struct cctoken_t
{
  int         type;
  char const *name;
} cctoken_t;

typedef struct esyntax_t
{
  cctoken_t *table;
} esyntax_t;

/* default tokens:  learn.microsoft.com/en-us/cpp/c-language/c-language-reference */
typedef enum ETOKEN_k
{
  ETOKEN_kINVALID = 0,
  ETOKEN_kEND    ,
  ETOKEN_kLPAREN , // '('
  ETOKEN_kRPAREN , // ')'
  ETOKEN_kLCURLY , // '{'
  ETOKEN_kRCURLY , // '}'
  ETOKEN_kLSQUARE, // '['
  ETOKEN_kRSQUARE, // ']'

  ETOKEN_kCOMMA,   // ','
  ETOKEN_kCOLON,   // ':'
  ETOKEN_kSPACE,   // ' '

  ETOKEN_kELLIPSIS,
  ETOKEN_kCOMMENT,
  ETOKEN_kCHARACTER,
  ETOKEN_kSTRING,
  ETOKEN_kIDENTIFIER,
  ETOKEN_kINTEGER,
  ETOKEN_kFLOAT,
  /* control statements */
  ETOKEN_kIF,
  ETOKEN_kELSE,
  ETOKEN_kSWITCH,

  ETOKEN_kCASE,
  ETOKEN_kDEFAULT,

  ETOKEN_kFOR,
  ETOKEN_kWHILE,
  ETOKEN_kDO,

  ETOKEN_kGOTO,
  ETOKEN_kRETURN,
  ETOKEN_kBREAK,
  ETOKEN_kCONTINUE,

  ETOKEN_kBWINV,
  ETOKEN_kLGNEG,  // !
  ETOKEN_kDOT,
  ETOKEN_kMUL,   // Note: multiplicative
  ETOKEN_kDIV,
  ETOKEN_kMOD,
  ETOKEN_kADD,   // Note: additive
  ETOKEN_kSUB,
  ETOKEN_kBWSHL, // Note: shift
  ETOKEN_kBWSHR,
  ETOKEN_kGTN,   // Note: relational
  ETOKEN_kLTN,
  ETOKEN_kTEQ,   // Note: equality
  ETOKEN_kFEQ,
  ETOKEN_kBWAND,
  ETOKEN_kBWXOR,
  ETOKEN_kBWOR,
  ETOKEN_kLGAND,
  ETOKEN_kLGOR,
  ETOKEN_kQMRK,

  ETOKEN_kASSIGN,   // =
  ETOKEN_kMUL_EQL,  // *=
  ETOKEN_kDIV_EQL,  // /=
  ETOKEN_kMOD_EQL,  // %=
  ETOKEN_kADD_EQL,  // +=
  ETOKEN_kSUB_EQL,  // -=
  ETOKEN_kBWSL_EQL, // <<=
  ETOKEN_kBWSR_EQL, // >>=

  ETOKEN_kGTE,
  ETOKEN_kLTE,
  ETOKEN_kBWAND_EQL,
  ETOKEN_kBWXOR_EQL,
  ETOKEN_kBWOR_EQL,
  /* Group: msvc attributes. */
  ETOKEN_kMSVC_ATTR_asm,       // maps to:  __asm
  ETOKEN_kMSVC_ATTR_based,     // maps to:  __based
  ETOKEN_kMSVC_ATTR_cdecl,     // maps to:  __cdecl
  ETOKEN_kMSVC_ATTR_clrcall,   // maps to:  __clrcall
  ETOKEN_kMSVC_ATTR_fastcall,  // maps to:  __fastcall
  ETOKEN_kMSVC_ATTR_inline,    // maps to:  __inline
  ETOKEN_kMSVC_ATTR_stdcall,   // maps to:  __stdcall
  ETOKEN_kMSVC_ATTR_thiscall,  // maps to:  __thiscall
  ETOKEN_kMSVC_ATTR_vectorcal, // maps to:  __vectorcal
  ETOKEN_kMSVC_DECLSPEC,       // __declspec
  /* alignment specifiers */
  ETOKEN_kALIGNOF, // maps to: _Alignof
  ETOKEN_kALIGNAS, // maps to: _Alignas

  /* type qualifiers */
  ETOKEN_kCONST,    // maps to: const
  ETOKEN_kRESTRICT, // maps to: restrict
  ETOKEN_kVOLATILE, // maps to: volatile

  /* function specifiers. */
  ETOKEN_kINLINE,
  ETOKEN_kNO_RETURN,

  ETOKEN_kVOID,
  ETOKEN_kSTDC_INT,
  ETOKEN_kSTDC_LONG,
  ETOKEN_kSTDC_SHORT,
  ETOKEN_kSTDC_DOUBLE,
  ETOKEN_kSTDC_FLOAT,
  ETOKEN_kSTDC_CHAR,
  ETOKEN_kSTDC_BOOL,     // _Bool
  ETOKEN_kSTDC_SIGNED,   // group start
  ETOKEN_kSTDC_UNSIGNED,
  ETOKEN_kMSVC_INT8,     // __int8
  ETOKEN_kMSVC_INT16,    // __int16
  ETOKEN_kMSVC_INT32,    // __int32
  ETOKEN_kMSVC_INT64,    // __int64
  ETOKEN_kENUM,
  ETOKEN_kSTRUCT,
  // Todo:
  // ETOKEN_kcomplex,       // _Complex
  // ETOKEN_katomic,        // _Atomic

  /* storage classes */
  ETOKEN_kTYPEDEF,
  ETOKEN_kAUTO,
  ETOKEN_kEXTERN,
  ETOKEN_kREGISTER,
  ETOKEN_kSTATIC,
  ETOKEN_kTHREAD_LOCAL,  // _Thread_local
  ETOKEN_kSIZEOF,
} ETOKEN_k;
