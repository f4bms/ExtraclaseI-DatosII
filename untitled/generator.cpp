#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int getSizeFromString(const string& sizeStr) {
    if (sizeStr == "small") {
        return 128000000;
    } else if (sizeStr == "medium") {
        return 250000000;
    } else if (sizeStr == "large") {
        return 500000000;
    } else {
        return -1;
    }
}

string generateUniqueFilename() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return "file_" + oss.str() + ".bin";
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " -size <small|medium|large> -output <output_directory>" << endl;
        return 1;
    }

    string sizeStr;
    string outputDir;

    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (arg == "-size") {
            sizeStr = argv[i + 1];
        } else if (arg == "-output") {
            outputDir = argv[i + 1];
        } else {
            cerr << "Invalid argument: " << arg << endl;
            return 1;
        }
    }

    int size = getSizeFromString(sizeStr);
    if (size == -1) {
        cerr << "Invalid size: " << sizeStr << endl;
        return 1;
    }

    string filename = generateUniqueFilename();
    fs::path outputPath = fs::path(outputDir) / filename;

    ofstream outputFile(outputPath, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file: " << outputPath << endl;
        return 1;
    }

    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        int randomNumber = rand();
        outputFile.write(reinterpret_cast<const char*>(&randomNumber), sizeof(randomNumber));
    }

    outputFile.close();
    cout << "File " << outputPath << " generated with " << size << " random numbers." << endl;

    return 0;
}
