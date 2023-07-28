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
typedef enum cctoken_k
{
  cctoken_kINVALID=0,
  /**
   * Group: syntactic operators.
   **/
  cctoken_kEND,

  // Todo: remove
  cctoken_Kendimpl, // '\r\n'
  cctoken_Kendexpl, // ';'

  cctoken_kLPAREN,  // '('
  cctoken_kRPAREN,  // ')'
  cctoken_kLCURLY,  // '{'
  cctoken_kRCURLY,  // '}'
  cctoken_kLSQUARE, // '['
  cctoken_kRSQUARE, // ']'

  cctoken_kCMA,   // ','
  cctoken_kCOLON, // ':'
  cctoken_Kspace, // ' '
  /**
   * Group: literals.
   **/
  cctoken_Kliteral_ellipsis,
  cctoken_Kliteral_comment,
  cctoken_Kliteral_character,
  cctoken_kLITSTR,
  cctoken_Kliteral_string_format,
  cctoken_kLITSTR_INVALID,
  cctoken_kLITIDENT,
  cctoken_kLITINT,
  cctoken_kLITFLO,
  /**
   * Group: msvc attributes.
   *
   *  ** these are reserved keywords **
   **/
  cctoken_Kmsvc_attr_asm,       // maps to:  __asm
  cctoken_Kmsvc_attr_based,     // maps to:  __based
  cctoken_Kmsvc_attr_cdecl,     // maps to:  __cdecl
  cctoken_Kmsvc_attr_clrcall,   // maps to:  __clrcall
  cctoken_Kmsvc_attr_fastcall,  // maps to:  __fastcall
  cctoken_Kmsvc_attr_inline,    // maps to:  __inline
  cctoken_Kmsvc_attr_stdcall,   // maps to:  __stdcall
  cctoken_Kmsvc_attr_thiscall,  // maps to:  __thiscall
  cctoken_Kmsvc_attr_vectorcal, // maps to:  __vectorcal
  /**
   * Group: alignment specifiers
   *
   * ** these are reserved keywords **
   **/
  kttc__algn_spec_0,
  cctoken_Kalign_of, // maps to: _Alignof
  cctoken_Kalign_as, // maps to: _Alignas
  kttc__algn_spec_1,
  /**
   * Group: type qualifiers
   *
   * ** these are reserved keywords **
   **/
  cctype_qual_0,
  cctoken_Kconst,    // maps to: const
  cctoken_Krestrict, // maps to: restrict
  cctoken_Kvolatile, // maps to: volatile
  cctype_qual_1,
  /**
   * Group: function specifiers.
   *
   * ** these are reserved keywords **
   **/
  kttc__func_spec_0,
  cctoken_Kinline,
  cctoken_Kno_return,
  kttc__func_spec_1,

  // Note: type specifiers ...
  cctoken_kVOID,
  cctoken_kSTDC_INT,
  cctoken_kSTDC_LONG,
  cctoken_kSTDC_SHORT,
  cctoken_kSTDC_DOUBLE,
  cctoken_kSTDC_FLOAT,
  cctoken_kSTDC_CHAR,
  cctoken_kSTDC_BOOL,     // _Bool
  cctoken_kSTDC_SIGNED,   // group start
  cctoken_kSTDC_UNSIGNED,
  cctoken_kMSVC_INT8,     // __int8
  cctoken_kMSVC_INT16,    // __int16
  cctoken_kMSVC_INT32,    // __int32
  cctoken_kMSVC_INT64,    // __int64
  cctoken_kENUM,
  cctoken_kSTRUCT,

  // Todo:
  // cctoken_Kcomplex,       // _Complex
  // cctoken_Katomic,        // _Atomic

  // Note: storage classes
  cctoken_Ktypedef,
  cctoken_Kauto,
  cctoken_Kextern,
  cctoken_Kregister,
  cctoken_Kstatic,
  cctoken_Kthread_local, // _Thread_local
  cctoken_Kmsvc_declspec, // __declspec


  cctoken_kSIZEOF,

  /**
   * Group: control statements.

   * ** these are reseverd keywords **
   **/
  cctoken_kIF,
  cctoken_kELSE,
  cctoken_Kswitch,

  cctoken_Kcase,
  cctoken_Kdefault,

  cctoken_kFOR,
  cctoken_kWHILE,
  cctoken_Kdo,

  cctoken_kGOTO,
  cctoken_kRETURN,
  cctoken_Kbreak,
  cctoken_Kcontinue,

  cctoken_kBWINV,
  cctoken_kLGNEG,  // !
  cctoken_kDOT,
  cctoken_kMUL,   // Note: multiplicative
  cctoken_kDIV,
  cctoken_kMOD,
  cctoken_kADD,   // Note: additive
  cctoken_kSUB,
  cctoken_kBWSHL, // Note: shift
  cctoken_kBWSHR,
  cctoken_kGTN,   // Note: relational
  cctoken_kLTN,
  cctoken_kTEQ,   // Note: equality
  cctoken_kFEQ,
  cctoken_kBWAND,
  cctoken_kBWXOR,
  cctoken_kBWOR,
  cctoken_kLGAND,
  cctoken_kLGOR,
  cctoken_kQMRK,

  cctoken_kASSIGN, // Note: assignment
  cctoken_kMUL_EQL,
  cctoken_kDIV_EQL,
  cctoken_kMOD_EQL,
  cctoken_kADD_EQL,
  cctoken_kSUB_EQL,
  cctoken_kBWSL_EQL,
  cctoken_kBWSR_EQL,

  cctoken_kGTE,
  cctoken_kLTE,
  cctoken_kBWAND_EQL,
  cctoken_kBWXOR_EQL,
  cctoken_kBWOR_EQL,

  cctoken_kCOMMENT,
} cctoken_k;
