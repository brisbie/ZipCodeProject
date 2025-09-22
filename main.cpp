#include <map>
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
    ifstream file("zip_code_database.csv");

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
    
    // ... code to print the table goes here ...
    
    return 0;
}