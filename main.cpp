#include <map>
#include <iomanip>
#include <string>
#include "ZipCodeRecordBuffer.h"
#include <iostream>
#include <limits> // For numeric_limits

using namespace std;

// A struct to hold the four extreme zip codes AND their coordinates.
struct StateRecord {
    string easternmost_zip;
    double easternmost_lon = -numeric_limits<double>::max(); // Initialize with a very small number
    string westernmost_zip;
    double westernmost_lon = numeric_limits<double>::max();  // Initialize with a very large number
    string northernmost_zip;
    double northernmost_lat = -numeric_limits<double>::max();
    string southernmost_zip;
    double southernmost_lat = numeric_limits<double>::max();
};

int main() {
    map<string, StateRecord> all_states;
    ZipCodeRecordBuffer buffer;
    ifstream file("../data/us_postal_codes.csv");

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string header;
    getline(file, header);

    while (buffer.ReadRecord(file)) {
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
    file.close();

    // Print header
    cout << left << setw(8) << "State" 
        << setw(15) << "Easternmost" 
        << setw(15) << "Westernmost" 
        << setw(15) << "Northernmost" 
        << setw(15) << "Southernmost" 
        << "\n";

    cout << string(68, '-') << "\n"; // separator line

    // Print each state's extremes alphabetically
    for (const auto& pair : all_states) {
        const auto& record = pair.second;
        cout << left << setw(8) << pair.first
            << setw(15) << record.easternmost_zip
            << setw(15) << record.westernmost_zip
            << setw(15) << record.northernmost_zip
            << setw(15) << record.southernmost_zip
            << "\n";
    }

    return 0;
}
