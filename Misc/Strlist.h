#ifndef StrListH
#define StrListH

#include "PascalCnv.h"

#define MaxStringsInStrL 256
#define MaxLongStr       128


     class TStrList
     {
        public:
                TStrList();
                ~TStrList();

                void Clear();
                void Add(AnsiString S);
                AnsiString& Strings(unsigned char i);
                int Count();
				// void Set(int i

				AnsiString& operator[](const int index);


        private:
                AnsiString Strs[MaxStringsInStrL];
                unsigned char Num;
     };

     typedef TStrList* PStrList;



#endif
