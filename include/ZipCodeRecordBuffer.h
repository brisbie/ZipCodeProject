#ifndef ZipCodeRecordBuffer_H
#define ZipCodeRecordBuffer_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> // For setprecision
#include <cstdlib> // For atof
#include <cmath>   // For fabs

using namespace std;

// Define a maximum length for each field based on your specifications
const int ZIP_CODE_LENGTH = 5;
const int PLACE_NAME_LENGTH = 50;
const int STATE_LENGTH = 2;
const int COUNTY_LENGTH = 50;
const int LAT_LONG_LENGTH = 10; // e.g., "-123.4567"

class ZipCodeRecordBuffer {
public:
    ZipCodeRecordBuffer() {
        // Initialize all fields to empty strings
        for (int i = 0; i < 5; ++i) {
            m_fields[i] = "";
        }
    }

    bool ReadRecord(ifstream& file) {
        string line;
        if (!getline(file, line)) {
            return false;
        }

        stringstream ss(line);
        string field;
        int field_count = 0;

        // Read and store each field, truncating if necessary
        // Order: Zip Code, Place Name, State, County, Lat, Long
        // You'll need to know the exact column order of your CSV
        while (getline(ss, field, ',') && field_count < 6) {
            // Truncate fields if they exceed the fixed length
            if (field_count == 0 && field.length() > ZIP_CODE_LENGTH) {
                m_fields[0] = field.substr(0, ZIP_CODE_LENGTH);
            } else if (field_count == 1 && field.length() > PLACE_NAME_LENGTH) {
                m_fields[1] = field.substr(0, PLACE_NAME_LENGTH);
            } else if (field_count == 2 && field.length() > STATE_LENGTH) {
                m_fields[2] = field.substr(0, STATE_LENGTH);
            } else if (field_count == 3 && field.length() > COUNTY_LENGTH) {
                m_fields[3] = field.substr(0, COUNTY_LENGTH);
            } else if (field_count >= 4 && field.length() > LAT_LONG_LENGTH) {
                m_fields[field_count] = field.substr(0, LAT_LONG_LENGTH);
            } else {
                m_fields[field_count] = field;
            }
            field_count++;
        }
        return true;
    }

    // Accessor methods to retrieve data, converting from string to the correct type
    string getZipCode() const { return m_fields[0]; }
    string getPlaceName() const { return m_fields[1]; }
    string getState() const { return m_fields[2]; }
    string getCounty() const { return m_fields[3]; }

    double getLatitude() const {
        return atof(m_fields[4].c_str());
    }

    double getLongitude() const {
        return atof(m_fields[5].c_str());
    }

private:
    string m_fields[6]; // A fixed-size array to hold the 6 fields
};

#endif // FIXED_ZIP_CODE_RECORD_BUFFER_H