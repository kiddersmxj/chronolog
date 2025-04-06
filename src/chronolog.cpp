#include "../inc/chronolog.hpp"

int main(int argc, char** argv) {
    InitConfig();

    int opt;
    int HelpFlag = 0;
    int VersionFlag = 0;
    int CreateFlag = 0;
    int StartFlag = 0;
    int StopFlag = 0;
    int ResetFlag = 0;
    std::string Name = "";

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "create", no_argument, &CreateFlag, 'c' },
        { "name", required_argument, NULL, 'n' },
        { "start", no_argument, NULL, 's' },
        { "stop", no_argument, NULL, 'e' },
        { "reset", no_argument, NULL, 'r' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvcsen:r", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
            }
            break;
        } 

        switch (opt) {
        case 'h':
            HelpFlag = 1;
            break;
        case 'v':
            VersionFlag = 1;
            break;
        case 'n':
            Name = optarg;
            break;
        case 's':
            StartFlag = 1;
            break;
        case 'e':
            StopFlag = 1;
            break;
        case 'r':
            ResetFlag = 1;
            break;
        case '?':
            Usage();
            return EXIT_FAILURE;
        default:
            Usage();
            break;
        }
    }

    if(!Name.empty()) {
        TimerManager timer;
        TimerLogger logger;
        
        if(StartFlag) {
            timer.start(Name);
            logger.log_event(Name, "START");
        }
        else if(StopFlag) {
            timer.stop(Name);
            logger.log_event(Name, "STOP");
        }
        else if(ResetFlag) {
            timer.reset(Name);
            logger.log_event(Name, "RESET");
        }
        
        return 0;
    }

    // Act on flags
    if(HelpFlag) {
        Usage();
        return EXIT_SUCCESS;
    }
    if(VersionFlag) {
        PrintVersion();
        return EXIT_SUCCESS;
    }

    return 0;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

