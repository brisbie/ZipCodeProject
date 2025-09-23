/**
 * @file    ZipCodeRecordBuffer.h
 * @authors  Evan Brisbin, Jason Donkor, Ethan Fischer, Tim Stevens, Markose Mesay
 * @date    2025-09-22
 * @version 1.0
 * @brief   Declaration of the ZipCodeRecordBuffer class for reading ZIP code CSV records.
 * @details
 *  Provides functionality to read, store, and access U.S. ZIP code records
 *  from a CSV file. Each record includes:
 *   - ZIP code
 *   - City
 *   - State abbreviation
 *   - County
 *   - Latitude and Longitude
 *  Fields are stored in fixed-length strings with truncation applied
 *  if values exceed their maximum allowed length.
 */

#ifndef ZipCodeRecordBuffer_H
#define ZipCodeRecordBuffer_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> // For setprecision
#include <cstdlib> // For atof
#include <cmath>   // For fabs

using namespace std;

/// @brief Maximum length for each field
const int ZIP_CODE_LENGTH = 5; /**< Maximum ZIP code length. */
const int PLACE_NAME_LENGTH = 50; /**< Maximum city/place name length. */
const int STATE_LENGTH = 2; /**< Maximum state abbreviation length. */
const int COUNTY_LENGTH = 50; /**< Maximum county name length. */
const int LAT_LONG_LENGTH = 10; /**< Maximum latitude/longitude length. */

/**
 * @class ZipCodeRecordBuffer
 * @brief Buffer class for reading and storing ZIP code records from a CSV file.
 * @pre A properly formatted CSV file must be opened before calling ReadRecord().
 * @post After a successful call to ReadRecord(), internal fields contain the parsed data.
 * @remark Truncates fields longer than their maximum allowed size.
 * @see    getLatitude(), getLongitude()
 */
class ZipCodeRecordBuffer {
public:
    /**
     * @brief Default constructor.
     * Initializes all fields to empty strings.
     */
    ZipCodeRecordBuffer() {
        // Initialize all fields to empty strings
        for (int i = 0; i < 5; ++i) {
            m_fields[i] = "";
        }
    }
    /**
     * @brief Reads a single record from a CSV file.
     * @param file Input file stream containing CSV data.
     * @return True if a record was successfully read, false if EOF is reached.
     */
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
    /**
     * @brief Get the ZIP code field.
     * @return The ZIP code as a string.
     */
    string getZipCode() const { return m_fields[0]; }
    
    /**
     * @brief Get the place name field.
     * @return The place name as a string.
     */
    string getPlaceName() const { return m_fields[1]; }

    /**
     * @brief Get the state abbreviation field.
     * @return The 2-character state abbreviation.
     */
    string getState() const { return m_fields[2]; }

    /**
     * @brief Get the county field.
     * @return The county name as a string.
     */
    string getCounty() const { return m_fields[3]; }

    /**
     * @brief Get the latitude field.
     * @return The latitude as a double.
     */
    double getLatitude() const {
        return atof(m_fields[4].c_str());
    }

    /**
     * @brief Get the longitude field.
     * @return The longitude as a double.
     */
    double getLongitude() const {
        return atof(m_fields[5].c_str());
    }

private:
    string m_fields[6]; ///< A fixed-sized array of 6 fields: {Zip, Place, State, County, Lat, Long}.
};

#endif // FIXED_ZIP_CODE_RECORD_BUFFER_H