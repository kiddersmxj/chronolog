#include "../inc/config.hpp"

int InitConfig() {
    // Load configuration file using the singleton Config instance
    if (!k::config::Config::getInstance().load(ConfigFilePath)) {
        std::cerr << "Failed to load config file: " << ConfigFilePath << std::endl;
        // Handle error as needed
        return 1;
    }

    KCONFIG_VAR_REQUIRED(LogPath, "log.path")
    KCONFIG_VAR_REQUIRED(LogFormat, "log.file_format")
    KCONFIG_VAR_REQUIRED(CSVHeaders, "log.csv_headers")

    return 0;
}

void Usage() {
    std::cout << UsageNotes << std::endl;
}

void Usage(std::string Message) {
    std::cout << Message << std::endl;
    std::cout << UsageNotes << std::endl;
}

void PrintVersion() {
    std::cout << ProgramName << ": version " << Version << std::endl;
}

