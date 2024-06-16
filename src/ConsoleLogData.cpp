#include <iostream>
#include <fstream>
#include <string>

// Class to hold all the data parsed from imperator's console_history.txt
class ConsoleLogData {
public:
    std::string date;
    std::string battleName;
    std::string location;
    std::string faction;

    ConsoleLogData(const std::string& filename) {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open the file.\n";
            return;
        }

        bool parsing = false;
        int lineIndex = 0;

        std::string line;
        while (std::getline(inputFile, line)) {
            if (line == "END") {
                break;
            }
            if (line == "STARTBUILDINGROME2MOD") {
                parsing = !parsing;
                continue;
            }

            if (parsing) {
                switch (lineIndex++) {
                    case 0:
                        date = line;
                        break;
                    case 1:
                        battleName = line;
                        break;
                    case 2:
                        location = line;
                        break;
                    case 3:
                        faction = line;
                        break;
                    default:
                        lineIndex = 0;
                        break;
                }
            }
        }

        inputFile.close();
    }

    void display() const {
        std::cout << "Date: " << date << "\n";
        std::cout << "Battle Name: " << battleName << "\n";
        std::cout << "Location: " << location << "\n";
        std::cout << "Faction: " << faction << "\n\n";
    }
};