//
// Created by Richard Marks on 2/21/23.
//

#include "CommandLine.h"
#include <iostream>

using namespace common;

CommandLine::CommandLine(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg = argv[i];
    
    if (arg[0] == '-')
    {
      if (arg[1] == '-')
      {
        // handle settings of the form --settingname=value
        std::string settingName = arg.substr(2, arg.find('=') - 2);
        std::string settingValue = arg.substr(arg.find('=') + 1);
        settings[settingName] = settingValue;
        std::cout << "setting: " << settingName << "=" << settingValue << std::endl;
      } else
      {
        // handle flags of the form -flagname
        std::string flagName = arg.substr(1);
        flags[flagName] = true;
        std::cout << "flag: " << flagName << "=true" << std::endl;
      }
    } else
    {
      // handle positional arguments
      positionalArgs.push_back(arg);
    }
  }
}

bool CommandLine::hasFlag(const std::string &flagName) const
{
  return flags.count(flagName) > 0;
}

std::string CommandLine::getArg(int index) const
{
  if (index < 0 || index >= static_cast<int>(positionalArgs.size()))
  {
    return "";
  }
  return positionalArgs[index];
}

std::string CommandLine::getSetting(const std::string &settingName) const
{
  auto it = settings.find(settingName);
  if (it == settings.end())
  {
    return "";
  }
  return it->second;
}

void CommandLine::required(int numArgs, const std::string *messages) const
{
  if (positionalArgs.size() < numArgs)
  {
    for (int i = 0; i < numArgs; i++)
    {
      if (i >= static_cast<int>(messages->size()))
      {
        throw std::runtime_error("Missing positional argument");
      }
      if (i >= static_cast<int>(positionalArgs.size()))
      {
        throw std::runtime_error(messages[i]);
      }
    }
  }
}