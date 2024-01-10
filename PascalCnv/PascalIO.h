// ------------------------------------------------------------------------------------
#ifndef PascalIO
#define PascalIO
// ------------------------------------------------------------------------------------
#include <stdio.h>
// ------------------------------------------------------------------------------------
extern int IORESULT;

int BLOCKREAD(FILE *&fd,void* buffer,int size);
int BLOCKWRITE(FILE *&fd,void* buffer,int size);
int ASSIGN(FILE* &fd,char *Filename);
int RESET(FILE* &fd,int i);
int REWRITE(FILE* &fd,int i);
int FILEPOS(FILE* &fd);
int SEEK(FILE *&fd,int iPos);
int CLOSE(FILE *&fd);
int EoF(FILE* &fd);
// ------------------------------------------------------------------------------------
#endif
// ------------------------------------------------------------------------------------