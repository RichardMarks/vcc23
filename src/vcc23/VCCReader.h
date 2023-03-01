//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_VCCREADER_H
#define VCC23_VCCREADER_H

#include <vector>
#include <string>

namespace vcc23
{
  class VCCReader
  {
  public:
    enum class LineType
    {
      Data,
      Code,
      Label,
      Empty,
    };
    typedef std::pair<LineType, std::string> LineInfo;
  
  private:
    std::vector<LineInfo> rawLines;
    std::vector<std::string> dataLines;
    std::vector<std::string> codeLines;
  
  public:
    VCCReader();
    
    void loadVCCFromFile(const std::string &fileName, bool clearAlreadyLoaded = false);
    
    [[nodiscard]] std::string getCodeLine(size_t index) const;
    
    [[nodiscard]] std::string getDataLine(size_t index) const;
    
    [[nodiscard]] LineInfo getRawLineInfo(size_t index) const;
    
    [[nodiscard]] size_t getCodeLineCount() const;
    
    [[nodiscard]] size_t getDataLineCount() const;
    
    [[nodiscard]] size_t getRawLineCount() const;
    
    std::string info();
  };
  
}

#endif //VCC23_VCCREADER_H
