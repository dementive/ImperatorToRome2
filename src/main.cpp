#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include "ConsoleLogData.cpp"
#include "DebugLogData.cpp"

// Platform-specific implementations
#ifdef _WIN32
#define IS_RUNNING "tasklist | findstr /i \"imperator.exe\""
#define PID_COMPARE 0
#else
#define IS_RUNNING "pgrep imperator > /dev/null"
#define PID_COMPARE 256
#endif

// TODO - don't hardcode these, the program should find the paths at runtime or the user should supply them.
#define IMPERATOR_EXECUTABLE_CMD "/home/dm/.steam/steam/steamapps/common/ImperatorRome/binaries/imperator -debug_mode -develop"
const std::string IMPERATOR_CONSOLE_LOG_PATH = "/home/dm/.local/share/Paradox Interactive/Imperator/console_history.txt";
const std::string IMPERATOR_DEBUG_LOG_PATH = "/home/dm/.local/share/Paradox Interactive/Imperator/logs/debug.log";
const std::string TOTAL_WAR_EXECUTABLE_CMD = "xdg-open \"steam://rungameid/214950\"";
const char DEBUG_LOG_CHAR[] = "STARTBUILDINGROME2MOD";
long debug_log_size = 0;

void launch_total_war() {
    std::cout << "Launching Total War..." << std::endl;
    system(TOTAL_WAR_EXECUTABLE_CMD.c_str());
}

long get_file_size(const std::string& filename) {
    struct stat fileStatus;
    if (stat(filename.c_str(), &fileStatus) < 0) {
        perror("Error getting file size");
        return -1;
    }
    return fileStatus.st_size;
}

void clear_imperator_logs() {
    std::ofstream file(IMPERATOR_CONSOLE_LOG_PATH);
    std::ofstream file2(IMPERATOR_DEBUG_LOG_PATH);
}

void check_debug_log() {
    long log_size = get_file_size(IMPERATOR_CONSOLE_LOG_PATH);
    if (debug_log_size == log_size) {
        // If the size hasn't changed since the last iteration there is no need to even open the file.
        return;
    } else {
        debug_log_size = log_size;
    }

    std::ifstream file(IMPERATOR_CONSOLE_LOG_PATH);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file" << IMPERATOR_CONSOLE_LOG_PATH;
        return;
    }

    while (std::getline(file, line)) {
        if (line.find(DEBUG_LOG_CHAR) != std::string::npos) {
            // Parse imperator logs for battle data
            DebugLogData debug_log(IMPERATOR_DEBUG_LOG_PATH);
            ConsoleLogData console_log(IMPERATOR_CONSOLE_LOG_PATH);
            debug_log.display();
            console_log.display();

            // Create Rome 2 mod (TODO)

            sleep(5);

            // Kill imperator??? Check if CW kills ck3 if it does copy their behavior.
            //clear_imperator_logs();
            system("kill imperator");
            
            launch_total_war();
            break;
        }
    }
    file.close();
}

bool is_running() {
    return system(IS_RUNNING) == PID_COMPARE? false : true;
}

void get_imperator_pid_startup() {
    int max_iters = 10; // if imperator isn't found after 1 minute just exit
    int current_iters = 0;

    while (!is_running()) {
        std::cout << "Looking for imperator process..." << std::endl;
        current_iters += 1;
        sleep(5);
        if (current_iters >= max_iters) {
            std::cout << "The imperator process could not be found...exiting." << std::endl;
            exit(1);
        }
    }
    std::cout << "Imperator process has been found..." << std::endl;
}

int main() {
    clear_imperator_logs();
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        get_imperator_pid_startup();
        while (true) {
            if (is_running()) {
                check_debug_log(); // Continuously check the log file
                sleep(1); // Sleep for a short period to avoid high CPU usage
            } else {
                std::cout << "Imperator is no longer running...exiting" << std::endl;
                exit(0);
            }
        }
    } else { // Child process
        system(IMPERATOR_EXECUTABLE_CMD);
        perror("Failed to execute");
        exit(EXIT_FAILURE);
    }

    return 0;
}
