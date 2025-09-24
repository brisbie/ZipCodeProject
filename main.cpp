/**
 * @file    main.cpp
 * @authors Evan Brisbin, Jason Donkor, Ethan Fischer, Tim Stevens, Markose Mesay
 * @date    2025-09-22
 * @version 1.1
 * @brief   Main program to read ZIP code CSV and calculate geographic extremes per state.
 * @details
 *  - Opens a ZIP code CSV file and reads each record using ZipCodeRecordBuffer.
 *  - Groups records by state and updates easternmost, westernmost, northernmost, and southernmost ZIP codes.
 *  - Generates two output files:
 *      1. `runbystate.txt` — extreme ZIP codes per state.
 *      2. `runbyzip.txt` — all ZIP codes with latitude and longitude.
 */

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include "ZipCodeRecordBuffer.h"

using namespace std;

/**
 * @struct StateRecord
 * @brief Holds the four extreme ZIP codes and their coordinates for a state.
 * @details
 *  - Tracks easternmost, westernmost, northernmost, and southernmost ZIP codes.
 *  - Initialized with extreme numeric values to ensure the first record is correctly stored.
 */
struct StateRecord {
    string easternmost_zip; /**< ZIP code with largest longitude. */
    double easternmost_lon = -numeric_limits<double>::max(); /**< Longitude of easternmost ZIP. */
    string westernmost_zip; /**< ZIP code with smallest longitude. */
    double westernmost_lon = numeric_limits<double>::max();  /**< Longitude of westernmost ZIP. */
    string northernmost_zip; /**< ZIP code with largest latitude. */
    double northernmost_lat = -numeric_limits<double>::max(); /**< Latitude of northernmost ZIP. */
    string southernmost_zip; /**< ZIP code with smallest latitude. */
    double southernmost_lat = numeric_limits<double>::max(); /**< Latitude of southernmost ZIP. */
};

/**
 * @brief Main program entry point.
 * @details
 *  - Reads the ZIP code CSV.
 *  - Updates `StateRecord` map with geographic extremes.
 *  - Writes two output files:
 *      - `runbystate.txt`: state extremes.
 *      - `runbyzip.txt`: all ZIPs with coordinates.
 * @return 0 on success, 1 on file errors.
 */
int main() {
    map<string, StateRecord> all_states; /**< Map storing extreme ZIP codes for each state. */
    vector<pair<string, StateRecord>> records_by_zip; /**< Vector to store ZIP records for sorting. */
    ZipCodeRecordBuffer buffer;

    /** @brief Open the CSV file */
    ifstream file("data/us_postal_codes.csv");
    if (!file.is_open()) {
        cerr << "Error opening CSV file." << endl;
        return 1;
    }

    string header;
    getline(file, header); /**< Skip header line. */

    /** @brief Read all records and populate state extremes and ZIP vector */
    while (buffer.ReadRecord(file)) {
        string state = buffer.getState();
        string zip = buffer.getZipCode();
        double latitude = buffer.getLatitude();
        double longitude = buffer.getLongitude();

        // Store by state
        if (all_states.find(state) == all_states.end()) {
            all_states[state] = StateRecord{};
        }
        StateRecord& record = all_states[state];

        if (longitude > record.easternmost_lon) { record.easternmost_lon = longitude; record.easternmost_zip = zip; }
        if (longitude < record.westernmost_lon) { record.westernmost_lon = longitude; record.westernmost_zip = zip; }
        if (latitude > record.northernmost_lat) { record.northernmost_lat = latitude; record.northernmost_zip = zip; }
        if (latitude < record.southernmost_lat) { record.southernmost_lat = latitude; record.southernmost_zip = zip; }

        // Store for ZIP-based output
        records_by_zip.push_back(make_pair(state, record));
    }
    file.close();

    /** @section state_output Output by State
     *  Writes the extreme ZIP codes per state to `runbystate.txt`.
     */
    ofstream state_file("runbystate.txt");
    if (!state_file.is_open()) {
        cerr << "Error opening runbystate.txt" << endl;
        return 1;
    }

    /** @brief Write header */
    state_file << left << setw(8) << "State"
               << setw(15) << "Easternmost"
               << setw(15) << "Westernmost"
               << setw(15) << "Northernmost"
               << setw(15) << "Southernmost" << "\n";
    state_file << string(68, '-') << "\n";

    /** @brief Write each state's extreme ZIPs */
    for (const auto& pair : all_states) {
        const auto& r = pair.second;
        state_file << left << setw(8) << pair.first
                   << setw(15) << r.easternmost_zip
                   << setw(15) << r.westernmost_zip
                   << setw(15) << r.northernmost_zip
                   << setw(15) << r.southernmost_zip
                   << "\n";
    }
    state_file.close();

    /** @section zip_output Output by ZIP
     *  Writes all ZIP codes with latitude and longitude to `runbyzip.txt`.
     */
    ofstream zip_file("runbyzip.txt");
    if (!zip_file.is_open()) {
        cerr << "Error opening runbyzip.txt" << endl;
        return 1;
    }

    /** @brief Reopen CSV file for reading all ZIP records */
    file.open("data/us_postal_codes.csv");
    if (!file.is_open()) {
        cerr << "Error reopening CSV file." << endl;
        return 1;
    }
    getline(file, header); /**< Skip header line */

    /** @brief Write ZIP header */
    zip_file << left << setw(8) << "State"
             << setw(10) << "ZIP"
             << setw(10) << "Lat"
             << setw(10) << "Lon" << "\n";
    zip_file << string(40, '-') << "\n";

    /** @brief Write each ZIP record */
    while (buffer.ReadRecord(file)) {
        zip_file << left << setw(8) << buffer.getState()
                 << setw(10) << buffer.getZipCode()
                 << setw(10) << fixed << setprecision(4) << buffer.getLatitude()
                 << setw(10) << fixed << setprecision(4) << buffer.getLongitude()
                 << "\n";
    }

    file.close();
    zip_file.close();

    return 0;
}
