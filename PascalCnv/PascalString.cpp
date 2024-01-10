//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "PascalString.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIRTYTRICK
#ifdef DIRTYTRICK
AnsiString oAux;
#endif
// ----------------------------------------------------------------------
char szString[1024];

void STRAssign(char *_pcSrcData,char *_pcDstData,unsigned char *_pucDstSize,unsigned int _uiMaxSize)
{
	strncpy((char*)_pcDstData,_pcSrcData,_uiMaxSize);
	unsigned int uiLen = strlen(_pcSrcData);
	if (uiLen > _uiMaxSize) uiLen = _uiMaxSize;

	_pcDstData[uiLen] = 0;
	*_pucDstSize = uiLen;
}

char *STRConcat(char *_pcSrcData,char *_pcDstData)
{
	// src + dst	
	sprintf(szString,"%s%s",_pcSrcData,_pcDstData);
	return(szString);

}

bool STRCmp(char *_pcSrcData,char *_pcDstData,unsigned char _ucSrcSize,unsigned char _ucDstSize)
{
	if (_ucSrcSize != _ucDstSize) return(false);
	return ( memcmp(_pcSrcData,_pcDstData,_ucSrcSize) == 0);
}
// ----------------------------------------------------------------------
AnsiString::AnsiString(char *_pucData)
{
	STRAssign(_pucData,(char*)data,&ucSize,255);
}

const AnsiString & AnsiString::operator = (const AnsiString &right)
{
    STRAssign((char*)right.data,(char*)data,&ucSize,255);
    return (*this);
}

const AnsiString AnsiString::operator + (const AnsiString &right) const
{
    return AnsiString ( STRConcat((char*)data,(char*)right.data) );
}

const bool AnsiString::operator == (const AnsiString &right) const
{
	return( STRCmp((char*)data,(char*)right.data,ucSize,right.ucSize) );
}

const bool AnsiString::operator != (const AnsiString &right) const
{
	return( ! STRCmp((char*)data,(char*)right.data,ucSize,right.ucSize) );
}

const AnsiString &AnsiString::operator = (const char right)
{
	char SrcStr[2];
	SrcStr[0] = right;
	SrcStr[1] = 0;

	STRAssign((char*)&SrcStr,(char*)data,&ucSize,255);

	return (*this);
}

const void AnsiString::operator +=(const AnsiString &right)
{
	STRAssign(STRConcat((char*)data,(char*)right.data),(char*)data,&ucSize,255);
}

const void AnsiString::operator +=(const char right)
{
	char SrcStr[2];
	SrcStr[0] = right;
	SrcStr[1] = 0;

	STRAssign(STRConcat((char*)data,(char*)SrcStr),(char*)data,&ucSize,255);
}

int AnsiString::Pos(AnsiString SubStr)
{
	char *szS = strstr((char*)data,(char*)SubStr.data);

	if (szS)
		return(szS - (char*)data);
	else
		return(-1);
}

AnsiString AnsiString::SubString(int _iStart,int _iLen)
{	
	char szStr[1024];
	strncpy(szStr,(char*)data + _iStart,_iLen);
	szStr[_iLen] = 0;
	
	return ( AnsiString(szStr) );
}
// ----------------------------------------------------------------------
String256::String256(char *_pucData)
{
	STRAssign(_pucData,(char*)data,&ucSize,255);
}

const String256 & String256::operator = (const String256 &right)
{
    STRAssign((char*)right.data,(char*)data,&ucSize,255);
    return (*this);
}

const String256 String256::operator + (const String256 &right) const
{
    return String256 ( STRConcat((char*)data,(char*)right.data) );
}

const bool String256::operator == (const String256 &right) const
{
	return( STRCmp((char*)data,(char*)right.data,ucSize,right.ucSize) );
}
// ----------------------------------------------------------------------
String35::String35(char *_pucData)
{
	STRAssign(_pucData,(char*)data,&ucSize,35);
}

const String35 & String35::operator = (const String35 &right)
{
	STRAssign((char*)right.data,(char*)data,&ucSize,35);
    return (*this);
}

const String35 String35::operator + (const String35 &right) const
{
	return String35( STRConcat((char*)data,(char*)right.data) );
}

const bool String35::operator == (const String35 &right) const
{
	return( STRCmp((char*)data,(char*)right.data,ucSize,right.ucSize) );
}
// ----------------------------------------------------------------------
String80::String80(char *_pucData)
{
	STRAssign(_pucData,(char*)data,&ucSize,80);
}

const String80 & String80::operator = (const String80 &right)
{
	STRAssign((char*)right.data,(char*)data,&ucSize,80);
    return (*this);
}

const String80 String80::operator + (const String80 &right) const
{
	return String80( STRConcat((char*)data,(char*)right.data) );
}

const bool String80::operator == (const String80 &right) const
{
	return( STRCmp((char*)data,(char*)right.data,ucSize,right.ucSize) );
}
// ----------------------------------------------------------------------
AnsiString IntToStr(int i)
{
	char szStr[16];
	sprintf(szStr,"%d",i);
	return(AnsiString(szStr));
}

// ----------------------------------------------------------------------