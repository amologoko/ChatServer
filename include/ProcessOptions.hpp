#ifndef __PROCESS_OPTIONS_HPP__
#define __PROCESS_OPTIONS_HPP__

#include <string>
#include <unordered_map>

class ProcessOptions {
public:
    ProcessOptions(int argc, char **argv);
    const std::string get(const std::string& key) {
        return _options[key];
    }

private:
    void printHelp();
    std::unordered_map<std::string, std::string> _options;
};

#endif
