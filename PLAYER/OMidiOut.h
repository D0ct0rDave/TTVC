#ifndef OMidiOutH
#define OMidiOutH

#include <windows.h>
#include <mmSystem.h>

    class TObjetoMidiOut
    {
        public:
            TObjetoMidiOut();
            ~TObjetoMidiOut();

            void  Open(void *FuncionCallBack);
            void  Close();

            void  Comando(unsigned char C,unsigned char V1,unsigned char V2);
            void  SysEx(unsigned char *pucBuffer,unsigned int Num);

            void  EstablecerDispositivo(int D);
            int Dispositivo();
            bool Abierto;


        private:

          HMIDIOUT          Handler;
          unsigned int      DispositivoActual;

          char *			BufferSysEx;
          PMIDIHDR          BufferSysExHdr;

          HANDLE            BufferSysExHnd;
          HANDLE            BufferSysExHdrHnd;
    };

    typedef TObjetoMidiOut* PObjetoMidiOut;

    int NumeroDispositivosMIDIOut();
    char *ObtenerNombreDispositivoMidiOut(int Dispositivo);

#endif


 