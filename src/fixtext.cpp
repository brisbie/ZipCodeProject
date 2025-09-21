using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "fixtext.h"
//#include "zipcode.h"


FixedTextBuffer::FixedTextBuffer(int maxFields, int maxChars)
{
	Init(maxFields, maxChars);
}

FixedTextBuffer::FixedTextBuffer(int numFields, int* fieldSize)
{
	 Init(numFields, fieldSize);
}

bool FixedTextBuffer::Init(int numFields, int maxChars)
{
	int i;
	if (numFields <= 0 || maxChars <= 0)
		return FALSE;
	MaxFields = numFields;
	MaxChars = maxChars;
	Buffer = new char[MaxChars];
	if (!Buffer)
		return FALSE;
	FieldSize = new int[MaxFields];
	if (!FieldSize)
	{
	delete[] Buffer;
	return FALSE;
}
	for (i = 0; i < MaxFields; i++)
		FieldSize[i] = 0;
	BufferSize = 0;
	NumFields = 0;
	Clear();
	return TRUE;
}

bool FixedTextBuffer::Init(int numFields, int* fieldSize)
{
	int i;
	if (numFields <= 0 || fieldSize == nullptr)
		return FALSE;

	MaxFields = numFields;

	// Allocate memory for the buffer (holding all field data)
	int totalSize = 0;
	for (i = 0; i < MaxFields; i++)
		totalSize += fieldSize[i];  // Add up all the field sizes

	Buffer = new char[totalSize];  // Allocate memory based on the sum of all field sizes
	if (!Buffer)
		return FALSE;

	// Allocate memory for the array that will store the field sizes
	FieldSize = new int[MaxFields];
	if (!FieldSize)
	{
		delete[] Buffer;
		return FALSE;
	}


	for (i = 0; i < MaxFields; i++)
		FieldSize[i] = fieldSize[i];  // Copy the sizes from the fieldSize array

	BufferSize = totalSize;  // Total size is now the sum of all individual field sizes
	NumFields = MaxFields;   // The number of fields is the size of the fieldSize array
	Clear();
	return TRUE;
}


void FixedTextBuffer::Clear()
{
	NextField = 0;
	NumFieldValues = 0;
	Packing = TRUE;
	NextCharacter = 0;
}

bool FixedTextBuffer::Read(const string& postalFile)
{
	ifstream inputStream(postalFile);

	if (!inputStream.is_open()) {
		cerr << "Error opening file: " << postalFile << endl;
		return FALSE;
	}

	string line;
	if (getline(inputStream, line)) {
		stringstream ss(line);
		string field;
		int fieldIndex = 0;

		while (getline(ss, field, ',')) {
			if (fieldIndex < MaxFields) {
				
				FieldSize[fieldIndex] = field.length();
				fieldIndex++;
			}
		}

		return TRUE;
	}
	return FALSE;
}
