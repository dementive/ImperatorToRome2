#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class UnitType {
public:
    std::string name;
    int count;

    UnitType(std::string n, int c) : name(n), count(c) {}
};

class UnitData {
public:
    bool attacker;
    std::string culture;
    std::string commander;
    std::string commander_culture;
    std::string commander_martial;
    std::string country_culture;
    std::vector<UnitType> units;

    UnitData() : attacker(false), culture(""), commander(""), commander_culture(""), commander_martial(""), country_culture("") {}
};

class DebugLogData {
private:
    std::string player;
    std::vector<UnitData> units;

    std::string splitAndStrip(std::string& input) {
        input.pop_back();
        size_t pos = input.find("DEBUG_");
        if (pos != std::string::npos) {
            return input.substr(pos + 6);
        }
        return ""; // Return empty std::string if "DEBUG_" not found
    }

public:
    DebugLogData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        std::string line;
        bool readingCombatData = false;
        bool unitData = false;
        UnitData currentUnitData;

        while (getline(file, line)) {
            line = splitAndStrip(line);
            if (line == "") continue;
            if (line == "END") break;
            if (line == "START") {
               readingCombatData = true;
               continue;
            }
            else if (line == "NEWUNIT") {
                if (unitData) {
                    units.push_back(currentUnitData);
                }
                currentUnitData = UnitData();
                unitData = true;
                continue;
            } 
            if (!readingCombatData) continue;

            if (line.find("player:") != std::string::npos) {
                player = line.substr(line.find("player: ") + strlen("player: "));
                std::cout << player << std::endl;
                continue;
            } else if (line.find("attacker:") != std::string::npos) {
                currentUnitData.attacker = true;
                continue;
            } else if (line.find("defender:") != std::string::npos) {
                currentUnitData.attacker = false;
                continue;
            } else if (line.find("country_culture:") != std::string::npos) {
                currentUnitData.country_culture = line.substr(line.find("country_culture: ") + strlen("country_culture: "));
                continue;
            } else if (line.find("commander_culture:") != std::string::npos) {
                currentUnitData.commander_culture = line.substr(line.find("commander_culture: ") + strlen("commander_culture: "));
                continue;
            } else if (line.find("commander_martial:") != std::string::npos) {
                currentUnitData.commander_martial = line.substr(line.find("commander_martial: ") + strlen("commander_martial: "));
                continue;
            } else if (line.find("commander:") != std::string::npos) {
                currentUnitData.commander = line.substr(line.find("commander: ") + strlen("commander: "));
                continue;
            } else {
                size_t pos = line.find(": ");
                if (pos != std::string::npos) {
                    std::string unitName = line.substr(0, pos);
                    std::string countStr = line.substr(pos + 2);
                    if (countStr.length() > 0 && atof(countStr.c_str()) > 0) {
                        currentUnitData.units.push_back(UnitType(unitName, atoi(countStr.c_str())));
                    }
                }
            }
        }
        if (unitData) {
            units.push_back(currentUnitData);
        }
    }

    void display() const {
        std::cout << "Player: " << player << std::endl;
        std::cout << "----------------" << std::endl;
        for (const auto& unit : units) {
            std::cout << "Attacker: " << (unit.attacker? "True" : "False") << std::endl;
            std::cout << "Culture: " << unit.country_culture << std::endl;
            std::cout << "Commander: " << unit.commander << std::endl;
            std::cout << "Commander Culture: " << unit.commander_culture << std::endl;
            std::cout << "Commander Martial: " << unit.commander_martial << std::endl;
            for (const auto& unitType : unit.units) {
                std::cout << unitType.name << ": " << unitType.count << std::endl;
            }
            std::cout << "----------------" << std::endl;
        }
    }
};
