// file I/O operations
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // Creating a file
    std::ofstream outFile("example.txt");
    if (outFile.is_open()) {
        outFile << "Hello, World!";
        outFile.close();
    } else {
        std::cout << "Unable to open file for writing.\n";
    }

    // Reading from a file
    std::ifstream inFile("example.txt");
    std::string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            std::cout << line << '\n';
        }
        inFile.close();
    } else {
        std::cout << "Unable to open file for reading.\n";
    }

    // Updating a file
    /*
    std::ios::in | std::ios::out | std::ios::app are file open modes:
std::ios::in opens the file for reading.
std::ios::out opens the file for writing.
std::ios::app opens the file in append mode, so data will be appended to the end of the file.
If the file does not exist, it will be created. If it does exist, 
the append operation (std::ios::app) ensures that new data will be written to the end of the file, 
preserving existing content. 
If the file cannot be opened, the std::fstream object ioFile will be in an error state, 
which can be checked using its member functions such as ioFile.fail().
    */
    std::fstream ioFile("example.txt", std::ios::in | std::ios::out | std::ios::app);
    if (ioFile.is_open()) {
        ioFile << "\nThis is an appended line.";
        ioFile.close();
    } else {
        std::cout << "Unable to open file for updating.\n";
    }

    // Deleting a file
    if (remove("example.txt") != 0) {
        std::cout << "Error deleting file.\n";
    } else {
        std::cout << "File successfully deleted.\n";
    }

    return 0;
}


