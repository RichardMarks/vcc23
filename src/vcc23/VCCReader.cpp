//
// Created by Richard Marks on 2/21/23.
//

#include <iostream>
#include <fstream>
#include <regex>

#include "VCCReader.h"

using namespace vcc23;

constexpr char COMMENT_CHARACTER = ';';

VCCReader::VCCReader() = default;

void VCCReader::loadVCCFromFile(const std::string &fileName, bool clearAlreadyLoaded)
{
  std::regex INSTRUCTION_REGEX("^[~.+\\-*/_^&|<>=zg`iropq]");
  std::regex LABEL_REGEX(R"(\b\w+\b\s*:)");
  
  if (clearAlreadyLoaded)
  {
    rawLines.clear();
    codeLines.clear();
    dataLines.clear();
  }
  
  std::ifstream fp(fileName, std::ios::in);
  if (!fp.is_open())
  {
    throw std::runtime_error("Unable to load - open file failed.");
  }
  
  std::string line;
  while (std::getline(fp, line))
  {
    // create a copy of the raw line to be added later
    std::string rawLine = line;
    
    // assume the line is empty until we determine the type
    LineType lineType = LineType::Empty;
    
    // Remove any leading whitespace
    line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch)
    { return !std::isspace(ch); }));
    
    if (!line.empty())
    {
      // strip comments
      auto comment_pos = line.find(COMMENT_CHARACTER);
      if (comment_pos != std::string::npos)
      {
        // std::cout << "Line has comments" << std::endl;
        line.erase(comment_pos);
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch)
                   { return !std::isspace(ch); })
                     .base(),
                   line.end());
      }
      // line may have become empty after removing the comments
      if (!line.empty())
      {
        // is this possibly a data declaration line?
        if (line[0] == 'd' || line[0] == 'D')
        {
          lineType = LineType::Data;
          dataLines.push_back(line);
          // std::cout << "Line is data" << std::endl;
        } else
        {
          if (std::regex_search(line, LABEL_REGEX))
          {
            // TODO: capture the label information
            lineType = LineType::Label;
            std::cout << "Identified a label, however labels are not implemented at this time." << std::endl;
          } else if (std::regex_search(line, INSTRUCTION_REGEX))
          {
            lineType = LineType::Code;
            codeLines.push_back(line);
            // std::cout << "Line is code" << std::endl;
          } else
          {
            std::cout << "Line not understood: " << line << std::endl;
          }
        }
      } else
      {
        // std::cout << "Line was made empty by removing comments" << std::endl;
      }
    } else
    {
      // std::cout << "Line is empty" << std::endl;
    }
    
    rawLines.emplace_back(lineType, rawLine);
  }
  
  fp.close();
}


[[nodiscard]] size_t VCCReader::getRawLineCount() const
{
  return rawLines.size();
}

[[nodiscard]] size_t VCCReader::getDataLineCount() const
{
  return dataLines.size();
}

[[nodiscard]] size_t VCCReader::getCodeLineCount() const
{
  return codeLines.size();
}

[[nodiscard]] VCCReader::LineInfo VCCReader::getRawLineInfo(size_t index) const
{
  return rawLines.at(index);
}

[[nodiscard]] std::string VCCReader::getDataLine(size_t index) const
{
  return dataLines.at(index);
}

[[nodiscard]] std::string VCCReader::getCodeLine(size_t index) const
{
  return codeLines.at(index);
}