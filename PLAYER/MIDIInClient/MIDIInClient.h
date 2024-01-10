#ifndef MIDIInClientH
#define MIDIInClientH

#include "Player/OMidiIn.h"

class MIDIInClient : public TObjetoMidiIn
{
        public:
			MIDIInClient();
            MIDIInClient(char *_szMIDIInServer);
            ~MIDIInClient();

            virtual void  Open (void *FuncionCallBack);
            virtual void  Close();
            virtual void  Start();
            virtual void  Stop ();
};

#endif 

