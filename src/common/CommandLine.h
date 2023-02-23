//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_COMMANDLINE_H
#define VCC23_COMMANDLINE_H

#include <string>
#include <vector>
#include <unordered_map>

namespace common
{
  class CommandLine
  {
  private:
    std::vector<std::string> positionalArgs;
    std::unordered_map<std::string, bool> flags;
    std::unordered_map<std::string, std::string> settings;
  
  public:
    CommandLine(int argc, char *argv[]);
    [[nodiscard]] bool hasFlag(const std::string &flagName) const;
    [[nodiscard]] std::string getArg(int index) const;
    [[nodiscard]] std::string getSetting(const std::string &settingName) const;
    void required(int numArgs, const std::string *messages) const;
  };
}

#endif //VCC23_COMMANDLINE_H
