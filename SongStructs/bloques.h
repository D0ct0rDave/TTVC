#ifndef bloquesH
#define bloquesH

#include "inter.h"

typedef struct
{
    unsigned short  NumTracks,NumBeats;
    PT98Pattern     bl;
}TBloque;

typedef TBloque* PBloque;

void CrearBloque(PBloque &B);
void DestruirBloque(PBloque &B);
bool CargarBloqueGeneral(PBloque &B,FILE *F,unsigned short &ID);
bool SalvarBloqueGeneral(PBloque  B,FILE *F,unsigned short  ID);
int TamanyoBloque(PBloque B);

void CopiarBloque(PT98Pattern F,PBloque D,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF);
void CortarBloque(PT98Pattern F,PBloque D,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF);
void PegarBloque (PT98Pattern D,PBloque F,unsigned short BI,unsigned short TI);
void BorrarBloque(PT98Pattern F,unsigned short BI,unsigned short BF,unsigned short TI,unsigned short TF);

#endif
 