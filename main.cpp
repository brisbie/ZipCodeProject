/**
 * @file    main.cpp
 * @authors Evan Brisbin, Jason Donkor, Ethan Fischer, Tim Stevens, Markose Mesay
 * @date    2025-09-22
 * @version 1.0
 * @brief   Main program to read ZIP code CSV and calculate geographic extremes per state.
 * @details
 *  - Opens a ZIP code CSV file and reads each record using ZipCodeRecordBuffer.
 *  - Groups records by state and updates easternmost, westernmost, northernmost, and southernmost ZIP codes.
 *  - Prints a formatted table of results (TODO).
 * @remark  The program currently does not print the table; printing logic should be added after processing.
 */
#include <map>
#include <string>
#include "ZipCodeRecordBuffer.h"
#include <iostream>
#include <limits> // For numeric_limits

using namespace std;

/**
 * @struct StateRecord
 * @brief Holds the four extreme ZIP codes AND their coordinates.
 * @details
 *  - Tracks easternmost, westernmost, northernmost, and southernmost ZIP codes.
 *  - Initialized with extreme numeric values to ensure first record is correctly stored.
 */
struct StateRecord {
    string easternmost_zip; /**< ZIP code with largest longitude. */
    double easternmost_lon = -numeric_limits<double>::max(); /**< Longitude of easternmost ZIP */
    string westernmost_zip; /**< ZIP code with smallest longitude. */
    double westernmost_lon = numeric_limits<double>::max();  /**< Longitude of westernmost ZIP */
    string northernmost_zip; /**< ZIP code with largest latitude. */
    double northernmost_lat = -numeric_limits<double>::max(); /**< Latitude of northernmost ZIP. */
    string southernmost_zip; /**< ZIP code with smallest latitude. */
    double southernmost_lat = numeric_limits<double>::max(); /**< Latitude of southernmost ZIP. */
};

/**
 * @brief Main program entry point.
 * @details
 *  - Reads the ZIP code CSV.
 *  - Updates StateRecord map with geographic extremes.
 *  - Prints results table.
 * @pre "us_postal_codes.csv" must exist and be accessible.
 * @post Map `all_states` contains geographic extremes for all states found in the CSV.
 * @return 0 if program succeeds, 1 if file cannot be opened.
 * @callgraph
 * @callergraph
 */
int main() {
    map<string, StateRecord> all_states; /**< [OUT] Map storing extreme ZIP codes for each state. */
    ZipCodeRecordBuffer buffer;
    ifstream file("zip_code_database.csv"); /**< [IN] CSV input file. */

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string header;
    getline(file, header); /**< Skip header line. */

    while (buffer.ReadRecord(file)) { /**< [IN, OUT] Reads record and updates buffer fields. */
        string state = buffer.getState();
        string zip = buffer.getZipCode();
        double latitude = buffer.getLatitude();
        double longitude = buffer.getLongitude();

        // Check if state record exists, if not, create it
        if (all_states.find(state) == all_states.end()) {
            all_states[state] = StateRecord{}; // Use the default constructor with initial values
        }
        
        StateRecord& record = all_states[state];

        // Update easternmost
        if (longitude > record.easternmost_lon) {
            record.easternmost_lon = longitude;
            record.easternmost_zip = zip;
        }

        // Update westernmost
        if (longitude < record.westernmost_lon) {
            record.westernmost_lon = longitude;
            record.westernmost_zip = zip;
        }

        // Update northernmost
        if (latitude > record.northernmost_lat) {
            record.northernmost_lat = latitude;
            record.northernmost_zip = zip;
        }

        // Update southernmost
        if (latitude < record.southernmost_lat) {
            record.southernmost_lat = latitude;
            record.southernmost_zip = zip;
        }
    }
    
    // @todo Print the table header and state extremes here
    // @note Printing logic not yet implemented.
    
    return 0;
}