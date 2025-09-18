// fixtext.h
// Copyright 1997, Gregory A. Riccardi

#ifndef FIXTEXT_H
#define FIXTEXT_H

#include <iostream.h>

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

class FixedTextBuffer 
// a buffer which holds a specific number of fixed sized text fields.
{  public:
	FixedTextBuffer (int maxFields, int maxChars = 1000); // construct with a maximum of maxFields
	FixedTextBuffer (int numFields, int * FieldSize);
		// construct with fields of specific size
	int NumberOfFields () const; // return number of fields 
	void Clear (); // clear field values from buffer
	int AddField (int fieldSize);
	int Read (istream &);
	int Write (ostream &);
	int Pack (const char *); // set the value of the next field of the buffer;
	int Unpack (char *); // extract the value of the next field of the buffer
	void Print (ostream &);
	int Init (int numFields, int maxChars = 1000);
	int Init (int numFields, int * fieldSize);
 private:
	char * Buffer; // character array to hold field values
	int BufferSize; // sum of the sizes of declared fields
	int * FieldSize; // array to hold field sizes
	int MaxFields; // maximum number of fields
	int MaxChars; // maximum number of characters in the buffer
	int NumFields; // actual number of defined fields
	int NextField; // index of next field to be packed/unpacked
	int NumFieldValues; // number of fields which are packed
	int Packing; // TRUE if in packing phase, FALSE o/w
	int NextCharacter; // packing/unpacking position in buffer
};

#endif
