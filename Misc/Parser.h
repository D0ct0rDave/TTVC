// -----------------------------------------------------------------------------
#ifndef ParserH
#define ParserH


#include "PascalCnv.h"
// -----------------------------------------------------------------------------

const int MaxPalabras = 30;

// -----------------------------------------------------------------------------
typedef struct{

    AnsiString Strs[MaxPalabras];
    unsigned char NumStrs;

}TParsedStr;

// -----------------------------------------------------------------------------
void ParseString(AnsiString S,TParsedStr &P);
// AnsiString LowerCase(AnsiString s);
// -----------------------------------------------------------------------------

#endif

 