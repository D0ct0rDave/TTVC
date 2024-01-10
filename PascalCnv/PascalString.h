#ifndef PascalStringH
#define PascalStringH

#ifdef _MBCS
class AnsiString
{
	public:	AnsiString(){}
			~AnsiString(){}
			AnsiString(char *_pucData);

			const AnsiString & operator=(const AnsiString &right);
			const AnsiString operator + (const AnsiString &right) const;
			const void operator +=(const AnsiString &right);
			const void operator +=(const char right);

			const bool operator == (const AnsiString &right) const;
			const bool operator != (const AnsiString &right) const;
			
			AnsiString SubString(int _iStart,int _iLen);

			int Length(){return (ucSize);};
			char *c_str() { return((char*)data); };
			int Pos(AnsiString SubStr);

			const char operator[](int i) const {	return data[i]; };
			const AnsiString & operator = (const char right);
			
	private:
			unsigned char ucSize;
			unsigned char data[256];
};
#endif

class String256
{
	public:	String256() {};
			~String256()  {};
			 String256(char *_pucData);

			const String256 & operator=(const String256 &right);
			const String256 operator + (const String256 &right) const;
			const bool operator == (const String256 &right) const;
			char *CString() { return((char*)data); };
	private:
			unsigned char ucSize;
			unsigned char data[256];
};

class String35
{
	public:	String35() {};
			~String35()  {};
			String35(char *_pucData);

	  const String35 &operator= (const String35 &right);
      const String35 operator + (const String35 &right) const;
	  const bool operator == (const String35 &right) const;
			char *CString() { return((char*)data); };
	private:
			unsigned char ucSize;
			unsigned char data[35];
};

class String80
{
	public:	String80() {}; 
			~String80()  {};
			String80(char *_pucData);

	  const String80 &operator= (const String80 &right);
      const String80 operator + (const String80 &right) const;
	  const bool operator == (const String80 &right) const;
			char *CString() { return((char*)data); };
	private:
			unsigned char ucSize;
			unsigned char data[80];
};


AnsiString IntToStr(int i);


#endif
