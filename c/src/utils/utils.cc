/*
 * Copyright 2021 Grzegorz Grzybek
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstring>
#include <iostream>

#include "config.h"
#include "utils/utils.h"

namespace aoc2021 {

    static const char *WS = " \t\n\r";

    static const struct option longopts[] = {
            {"help",    no_argument,       nullptr, 'h'},
            {"version", no_argument,       nullptr, 'v'},
            {"file",    required_argument, nullptr, 'f'}
    };

    /**
     * Constructs generic option parser for given day
     * @param _day
     * @param _argc
     * @param _argv
     */
    Options::Options(const char *_day, int _argc, char *_argv[])
            : argc(_argc), argv(_argv), day_name(_day) {
        parse();
    }

    /**
     * Frees dynamic ifstream pointer
     */
    Options::~Options() {
        delete input_file;
    }

    /**
     * Parses command line arguments - mostly to find a file with puzzle data to read
     */
    void Options::parse() {
        program_name = argv[0];
        int optc, lose = 0;

        while ((optc = getopt_long(argc, argv, "hf:v", longopts, nullptr)) != -1) {
            switch (optc) {
                case 'f':
                    input_file = new std::ifstream(optarg, std::ios_base::in);
                    if (!*input_file) {
                        std::cerr << optarg << ": " << ::strerror(errno) << " (" << optarg << ")" << std::endl;
                        _valid = false;
                        exit_code = EXIT_FAILURE;
                    }
                    break;
                case 'h':
                    print_help();
                    _finish = true;
                    return;
                case 'v':
                    std::cout << PACKAGE_STRING << std::endl;
                    _finish = true;
                    return;
                default:
                    lose = 1;
            }
        }

        if (lose || optind < argc || !input_file) {
            if (optind < argc) {
                std::cerr << program_name << ": extra operand: " << argv[optind] << std::endl;
            }
            std::cerr << "Try `" << program_name << " --help' for more information." << std::endl;
            _valid = false;
            exit_code = EXIT_FAILURE;
        }
    }

    /**
     * Checks whether to continue with puzzle or to exit immediately.
     * @return
     */
    bool Options::check() const {
        return _valid && !_finish;
    }

    /**
     * If options tell that puzzle should not be solved, returns program exit code
     * @return
     */
    int Options::result() const {
        return exit_code;
    }

    /**
     * Instructs about program usage
     */
    void Options::print_help() {
        std::cout << "Usage: " << program_name << " [OPTION]...\n";

        std::cout << day_name << ".\n\n";

        std::cout << "-h, --help      display this help and exit\n";
        std::cout << "-v, --version   display version information and exit\n\n";

        std::cout << "-f, --file=FILE puzzle input (file)\n\n";

        std::cout << "Report bugs to <" << PACKAGE_BUGREPORT << ".\n";
        std::cout.flush();
    }

    /**
     * Returns managed pointer to file input stream with puzzle data
     * @return
     */
    std::ifstream *Options::file() {
        return input_file;
    }

    /**
     * Checks if given string has some text (after trimming)
     * @param line
     * @return
     */
    bool has_text(std::string &line) {
        std::string::size_type size = line.length();
        if (!size)
            return false;
        std::string::size_type i1 = line.find_first_not_of(WS);
        std::string::size_type i2 = line.find_last_not_of(WS);
        return i1 != std::string::npos && i2 != std::string::npos;
    }

    /**
     * Trims the passed string
     * @param line
     * @return
     */
    void trim(std::string &line) {
        line.erase(0, line.find_first_not_of(WS));
        line.erase(line.find_last_not_of(WS) + 1);
    }

}
