#ifndef ZIPCODE_H
#define ZIPCODE_H

#include <iostream>
#include "fixtext.h"

class Zipcode 
{
  public:
	// fields
  char ZipCode[10];
  char City[21];
  char State[3];
  char Latitude[11];
  char Longitude[11];
	//methods
	Zipcode ();
	void Clear ();
	static int InitBuffer (FixedTextBuffer &);
	int Unpack (FixedTextBuffer &);
	int Pack (FixedTextBuffer &) const;
	void Print (std::ostream &);
};

#endif
