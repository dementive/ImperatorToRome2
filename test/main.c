#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

// Platform-specific implementations
#ifdef _WIN32
#define IS_RUNNING "tasklist | findstr /i "imperator.exe""
#define PID_COMPARE 0
#else
#define IS_RUNNING "pgrep imperator > /dev/null"
#define PID_COMPARE 256
#endif

// TODO - don't hardcode these, the program should find the paths at runtime or the user should supply them.
#define IMPERATOR_EXECUTABLE_CMD "/home/dm/.steam/steam/steamapps/common/ImperatorRome/binaries/imperator -debug_mode -develop"
const char IMPERATOR_LOG_PATH[] = "/home/dm/.local/share/Paradox Interactive/Imperator/console_history.txt";
const char TOTAL_WAR_EXECUTABLE_CMD[] = "xdg-open \"steam://rungameid/214950\"";

const char DEBUG_LOG_CHAR[] = "DEBUG: Launch Total War";
long int debug_log_offset = 0;
long debug_log_size = 0;

void launch_total_war() {
    printf("Launching Total War...\n");
    system(TOTAL_WAR_EXECUTABLE_CMD);
}

long get_file_size(const char *filename) {
    struct stat fileStatus;
    if (stat(filename, &fileStatus) < 0) {
        perror("Error getting file size");
        return -1;
    }
    return fileStatus.st_size;
}

void check_debug_log() {
    long log_size = get_file_size(IMPERATOR_LOG_PATH);
    if (debug_log_size == log_size) {
        // If the size hasn't changed since the last iteration there is no need to even open the file.
        return;
    } else {
        debug_log_size = log_size;
    }

    FILE *file = fopen(IMPERATOR_LOG_PATH, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fseek(file, debug_log_offset, SEEK_SET);
    char lineBuffer[1024];

    while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
        if (strstr(lineBuffer, DEBUG_LOG_CHAR) != NULL) {
            launch_total_war();
            break;
        }
    }

    debug_log_offset = ftell(file);
    fclose(file);
}

int is_running() {
    if (system(IS_RUNNING) == PID_COMPARE) {
        return 0;
    } else {
        return 1;
    }
}

void get_imperator_pid_startup() {
    int max_iters = 10; // if imperator isn't found after 1 minute just exit
    int current_iters = 0;

    while (!is_running()) {
        printf("Looking for imperator process...\n");
        current_iters += 1;
        sleep(5);
        if (current_iters >= max_iters) {
            printf("The imperator processs could not be found...exiting.\n");
            exit(1);
        }
    }
    printf("Imperator process has been found...\n");
}

int main() {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        get_imperator_pid_startup();
        while (1) {
            if (is_running()) {
                check_debug_log(); // Continuously check the log file
                sleep(1); // Sleep for a short period to avoid high CPU usage
            } else {
                printf("Imperator is no longer running...exiting\n");
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
