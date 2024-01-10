//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "StrList.h"
AnsiString asNULL;

TStrList::TStrList()
{
    Num=0;
}

TStrList::~TStrList()
{
     Num=0;
}

void TStrList::Clear()
{
     Num=0;
}

void TStrList::Add(AnsiString S)
{
     if (Num < MaxStringsInStrL)
     {
          Strs[Num] = S;
          Num++;
     };
}

AnsiString& TStrList::Strings(unsigned char i)
{
     if (i<Num)
        return( Strs[i] );
     else
	 {
		asNULL = "";
        return( asNULL );
 	 }
}

int TStrList::Count()
{
     return( Num );
}

AnsiString& TStrList::operator[](const int index)
{
	return ( Strs[index] );
}


