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
	MaxFields = numFields; //might cause some problems
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
	BufferSize = static_cast <size_t>(MaxChars);
	NumFields = 0;
	NextField = 0; 
	NumFieldValues = 0;  
	Packing = TRUE; 
	NextCharacter = 0;
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
	size_t totalSize = 0;
	for (i = 0; i < MaxFields; i++)
		totalSize += static_cast <size_t>( fieldSize[i]);  // Add up all the field sizes

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
	NextField = 0;
	NumFieldValues = 0;
	Packing = TRUE;
	NextCharacter = 0;

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
	while (getline(inputStream, line)) {
		stringstream ss(line);
		string field;
		int fieldIndex = 0;

		int i;

		Clear();

		while (getline(ss, field, ',')) {

			if (fieldIndex >= MaxFields) {
				cerr << "Too many fields in line; MaxFields = " << MaxFields << endl;
				return FALSE;
			}

			int rawSize = FieldSize[fieldIndex];
			if (rawSize <= 0) {
				cerr << "Invalid FieldSize[" << fieldIndex << "] = " << rawSize << endl;
				return FALSE;
			}

			size_t size = static_cast<size_t>(rawSize);

			if (field.length() > size) {
				cerr << "Warning: Field " << fieldIndex << " is too large and will be truncated." << endl;
				field = field.substr(0, size);  
			}


			for (i = 0; i < FieldSize[fieldIndex]; i++) {
				if (i < static_cast<int>(field.length())) {
					if (NextCharacter + size <= BufferSize)
					Buffer[NextCharacter + i] = field[i];
					else
					{
						cerr << "Buffer overflow while reading field " << fieldIndex << endl;
						return FALSE; // Prevent buffer overflow
					}
				}
				else {
					Buffer[NextCharacter + i] = ' '; // Pad with spaces if field is shorter
				}
			}
			NextCharacter += size;
			fieldIndex++;
		}

		NumFieldValues = fieldIndex;
		/*return TRUE;*/
	}



	return TRUE;
}

void FixedTextBuffer::Print(ostream& os)
{
	int i, j, k;
	int pos = 0;
	for (i = 0; i < NumFieldValues; i++)
	{
		for (j = 0; j < FieldSize[i]; j++)
		{
			if (pos < BufferSize)
				os << Buffer[pos++];
			else
			{
				cerr << "Buffer overflow while printing field " << i << endl;
				return; // Prevent buffer overflow
			}
		}
		os << "|";
	}
	os << endl;
}
