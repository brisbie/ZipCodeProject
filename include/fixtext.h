#ifndef FIXTEXT_H
#define FIXTEXT_H

#include <iostream>
using namespace std;

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

class FixedTextBuffer
	// a buffer which holds a specific number of fixed sized text fields.
{
public:
	FixedTextBuffer(int maxFields, int maxChars = 1000); // construct with a maximum of maxFields
    FixedTextBuffer(int numFields, int* fieldSize);
	// construct with fields of specific size
	int NumberOfFields() const; // return number of fields 
	void Clear(); // clear field values from buffer
	int AddField(int fieldSize);
	bool Read(const string& postalFile);
	bool Write(const string& postalFile);
	int Pack(const char*); // set the value of the next field of the buffer;
	int Unpack(char*); // extract the value of the next field of the buffer
	void Print(ostream&);
	bool Init(int numFields, int maxChars = 1000);
	bool Init (int numFields, int* fieldSize);
private:
	char* Buffer; // character array to hold field values
	int BufferSize; // sum of the sizes of declared fields
	int* FieldSize; // array to hold field sizes
	int MaxFields; // maximum number of fields
	int MaxChars; // maximum number of characters in the buffer
	int NumFields; // actual number of defined fields
	int NextField; // index of next field to be packed/unpacked
	int NumFieldValues; // number of fields which are packed
	int Packing; // TRUE if in packing phase, FALSE o/w
	int NextCharacter; // packing/unpacking position in buffer
};

#endif

