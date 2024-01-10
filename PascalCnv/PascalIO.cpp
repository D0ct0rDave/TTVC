//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include "PrecompiledHeaders.h"
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
#include "PascalIO.h"
#include <string.h>
// ------------------------------------------------------------------------------------
int IORESULT;

int BLOCKREAD(FILE *&fd,void* buffer,int size)
{
	IORESULT = fread(buffer,size,1,fd);
	IORESULT = 0;
	return(0);
}

int BLOCKWRITE(FILE *&fd,void* buffer,int size)
{
	IORESULT = fwrite(buffer,size,1,fd);
	IORESULT = 0;                               
	return(0);
}

char szAssignedFilename[16][256];
int  iFilenames = 0;

int ASSIGN(FILE* &fd,char *Filename)
{
	if (! Filename) return(0);
	
	strcpy(szAssignedFilename[iFilenames],Filename);
	// iFilenames++;
	return(0);
}

int RESET(FILE* &fd,int i)
{
	// iFilenames--;
	fd = fopen(szAssignedFilename[iFilenames],"rb");
    IORESULT = (fd == NULL);
	return(0);
}

int REWRITE(FILE* &fd,int i)
{
	// iFilenames--;
	fd = fopen(szAssignedFilename[iFilenames],"wb");
    IORESULT = (fd == NULL);
	return(0);
}

int FILEPOS(FILE* &fd)
{
	return( ftell(fd) );
}

int SEEK(FILE *&fd,int iPos)
{
	return( fseek(fd,iPos,SEEK_SET));
}

int CLOSE(FILE *&fd)
{
	fclose(fd);
	return(0);
}

int EoF(FILE* &fd)
{
	int iPos = ftell(fd);
	fseek(fd,0,SEEK_END);
	int iLen = ftell(fd);
	fseek(fd,iPos,SEEK_SET);

	return( iPos == iLen );
}
